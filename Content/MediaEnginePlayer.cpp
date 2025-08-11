//--------------------------------------------------------------------------------------
// File: MediaEnginePlayer.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Common/DirectXHelper.h"
#include "Hot3dxRotoDrawSceneRender.h"
#include "Common/DeviceResources.h"

using Microsoft::WRL::ComPtr;
using namespace DX;

namespace Hot3dxRotoDraw
{
    class MediaEngineNotify : public IMFMediaEngineNotify
    {
        long m_cRef;
        IMFNotify* m_pCB;

    public:
        MediaEngineNotify() noexcept :
            m_cRef(1),
            m_pCB(nullptr)
        {
        }

        STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
        {
            if (__uuidof(IMFMediaEngineNotify) == riid)
            {
                *ppv = static_cast<IMFMediaEngineNotify*>(this);
            }
            else
            {
                *ppv = nullptr;
                return E_NOINTERFACE;
            }

            AddRef();

            return S_OK;
        }

        STDMETHODIMP_(ULONG) AddRef()
        {
            return InterlockedIncrement(&m_cRef);
        }

        STDMETHODIMP_(ULONG) Release()
        {
            LONG cRef = InterlockedDecrement(&m_cRef);
            if (cRef == 0)
            {
                delete this;
            }
            return cRef;
        }

        void SetCallback(IMFNotify* pCB)
        {
            m_pCB = pCB;
        }

        // EventNotify is called when the Media Engine sends an event.
        STDMETHODIMP EventNotify(DWORD meEvent, DWORD_PTR param1, DWORD)
        {
            if (meEvent == MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE)
            {
                SetEvent(reinterpret_cast<HANDLE>(param1));
            }
            else
            {
                m_pCB->OnMediaEngineEvent(meEvent);
            }

            return S_OK;
        }
    };


Hot3dxRotoDraw::MediaEnginePlayer::MediaEnginePlayer(const std::shared_ptr<DeviceResources>& deviceResources) noexcept :
    m_bkgColor{},
    m_isPlaying(false),
    m_isInfoReady(false),
    m_deviceResources(deviceResources),
    m_isFinished(false)
{
}

Hot3dxRotoDraw::MediaEnginePlayer::~MediaEnginePlayer()
{
    Shutdown();

    MFShutdown();
}

void Hot3dxRotoDraw::MediaEnginePlayer::Initialize(IDXGIFactory4* dxgiFactory, ID3D12Device* device, DXGI_FORMAT format)
{
    // Initialize Media Foundation (see Main.cpp for code to handle Windows N Editions)
    ThrowIfFailedHot(MFStartup(MF_VERSION));

    // Create our own device to avoid threading issues
    auto adapterLuid = device->GetAdapterLuid();

    ComPtr<IDXGIAdapter1> adapter;
    for (UINT adapterIndex = 0;
        DXGI_ERROR_NOT_FOUND != dxgiFactory->EnumAdapters1(
            adapterIndex,
            adapter.ReleaseAndGetAddressOf());
        ++adapterIndex)
    {
        DXGI_ADAPTER_DESC1 desc;
        ThrowIfFailedHot(adapter->GetDesc1(&desc));

        if (desc.AdapterLuid.LowPart == adapterLuid.LowPart
            && desc.AdapterLuid.HighPart == adapterLuid.HighPart)
        {
            // Found the same adapter as our DX12 device
            break;
        }
    };

#if defined(NDEBUG)
    if (!adapter)
    {
        throw std::exception("No matching device for DirectX 12 found");
    }
#else
    if (!adapter)
    {
        if (FAILED(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))))
        {
            throw std::exception("WARP12 not available. Enable the 'Graphics Tools' optional feature");
        }

        DXGI_ADAPTER_DESC1 desc;
        ThrowIfFailedHot(adapter->GetDesc1(&desc));

        if (desc.AdapterLuid.LowPart != adapterLuid.LowPart
            || desc.AdapterLuid.HighPart != adapterLuid.HighPart)
        {
            throw std::exception("No matching device for DirectX 12 found");
        }
    }
#endif

    D3D_DRIVER_TYPE driveType;
    adapter.Get();

#if defined(NDEBUG)
    driveType = D3D_DRIVER_TYPE_UNKNOWN;
#else
    adapter ? driveType = D3D_DRIVER_TYPE_UNKNOWN : driveType = D3D_DRIVER_TYPE_WARP;
#endif

    // This flag adds support for surfaces with a different color channel ordering
    // than the API default. It is required for compatibility with Direct2D.
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    if (SdkLayersAvailableD3D11())
    {
        // If the project is in a debug build, enable debugging via SDK Layers with this flag.
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    }
#endif

    // This array defines the set of DirectX hardware feature levels this app will support.
    // Note the ordering should be preserved.
    // Note that HoloLens supports feature level 11.1. The HoloLens emulator is also capable
    // of running on graphics cards starting with feature level 10.0.
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D11_RASTERIZER_DESC rasterDesc{
            D3D11_FILL_SOLID,
            D3D11_CULL_NONE,
            false,
            0,
            0.0f,
            0.0f,
            true,
            false,
            true,
            true };


    // Create the Direct3D 11 API device object and a corresponding context.
    ComPtr<ID3D11Device> baseDevice;
    //ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;

    // Retrieve the adapter specified by the holographic space.
    ThrowIfFailedHot(
        dxgiFactory->EnumAdapterByLuid(
            adapterLuid,
            IID_PPV_ARGS(&adapter)
        )
    );
    const D3D_DRIVER_TYPE driverType = adapter == nullptr ? D3D_DRIVER_TYPE_HARDWARE : D3D_DRIVER_TYPE_UNKNOWN;
    const HRESULT hr = D3D11CreateDevice(
        adapter.Get(),        // Either nullptr, or the primary adapter determined by Windows Holographic.
        driverType,                 // Create a device using the hardware graphics driver.
        0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
        creationFlags,              // Set debug and Direct2D compatibility flags.
        featureLevels,              // List of feature levels this app can support.
        ARRAYSIZE(featureLevels),   // Size of the list above.
        D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Runtime apps.
        &baseDevice,                    // Returns the Direct3D device created.
        nullptr,//&m_d3dFeatureLevel,         // Returns feature level of device created.
        &context                    // Returns the device immediate context.
    );

    if (FAILED(hr))
    {
        // If the initialization fails, fall back to the WARP device.
        // For more information on WARP, see:
        // http://go.microsoft.com/fwlink/?LinkId=286690
        ThrowIfFailedHot(
            D3D11CreateDevice(
                nullptr,              // Use the default DXGI adapter for WARP.
                D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
                0,
                creationFlags,
                featureLevels,
                ARRAYSIZE(featureLevels),
                D3D11_SDK_VERSION,
                &baseDevice,
                nullptr,//&m_d3dFeatureLevel,
                &context
            )
        );
    }


    // Store pointers to the Direct3D device and immediate context.
    ThrowIfFailedHot(
        baseDevice.As(&m_device)
    );

    // ThrowIfFailedHot(
     //    context.As(&m_d3dContext)
    // );

     // Acquire the DXGI interface for the Direct3D device.
    ComPtr<IDXGIDevice3> dxgiDevice;
    ThrowIfFailedHot(
        m_device.As(&dxgiDevice)
    );

    // Wrap the native device using a WinRT interop object.
    //m_d3dInteropDevice = CreateDirect3DDevice(dxgiDevice.Get());

    // Cache the DXGI adapter.
    // This is for the case of no preferred DXGI adapter, or fallback to WARP.
    ComPtr<IDXGIAdapter> dxgiAdapter;
    ThrowIfFailedHot(
        dxgiDevice->GetAdapter(&dxgiAdapter)
    );
    ThrowIfFailedHot(
        dxgiAdapter.As(&adapter)
    );

    ComPtr<ID3D10Multithread> multithreaded;
    ThrowIfFailedHot(
        baseDevice->QueryInterface(IID_PPV_ARGS(&multithreaded))
    );
    multithreaded->SetMultithreadProtected(TRUE);
    ThrowIfFailedHot(baseDevice.As(&multithreaded));
    ThrowIfFailedHot(baseDevice.As(&m_device));

    // Setup Media Engine
    Microsoft::WRL::ComPtr<IMFDXGIDeviceManager> dxgiManager;
    UINT resetToken;
    ThrowIfFailedHot(MFCreateDXGIDeviceManager(&resetToken, dxgiManager.GetAddressOf()));
    ThrowIfFailedHot(dxgiManager->ResetDevice(m_device.Get(), resetToken));

    // Create our event callback object.
    ComPtr<MediaEngineNotify> spNotify = new MediaEngineNotify();
    if (spNotify == nullptr)
    {
        ThrowIfFailedHot(E_OUTOFMEMORY);
    }

    spNotify->SetCallback(this);

    // Set configuration attribiutes.
    ComPtr<IMFAttributes> attributes;
    ThrowIfFailedHot(MFCreateAttributes(attributes.GetAddressOf(), 1));
    ThrowIfFailedHot(attributes->SetUnknown(MF_MEDIA_ENGINE_DXGI_MANAGER, reinterpret_cast<IUnknown*>(dxgiManager.Get())));
    ThrowIfFailedHot(attributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, reinterpret_cast<IUnknown*>(spNotify.Get())));
    ThrowIfFailedHot(attributes->SetUINT32(MF_MEDIA_ENGINE_VIDEO_OUTPUT_FORMAT, format));

    // Create MediaEngine.
    ComPtr<IMFMediaEngineClassFactory> mfFactory;
    ThrowIfFailedHot(
        CoCreateInstance(CLSID_MFMediaEngineClassFactory,
            nullptr,
            CLSCTX_ALL,
            IID_PPV_ARGS(mfFactory.GetAddressOf())));

    ThrowIfFailedHot(
        mfFactory->CreateInstance(0,
            attributes.Get(),
            m_mediaEngine.ReleaseAndGetAddressOf()));

    // Create MediaEngineEx
    ThrowIfFailedHot(m_mediaEngine.As(&m_engineEx));
  }
}

void Hot3dxRotoDraw::MediaEnginePlayer::Shutdown()
{
    if (m_mediaEngine)
    {
        m_mediaEngine->Shutdown();
    }
}

void Hot3dxRotoDraw::MediaEnginePlayer::Play()
{
    if (m_isPlaying)
        return;

    if (m_mediaEngine)
    {
        ThrowIfFailedHot(m_mediaEngine->Play());
        m_isPlaying = true;
        m_isFinished = false;
    }
}

void Hot3dxRotoDraw::MediaEnginePlayer::SetMuted(bool muted)
{
    if (m_mediaEngine)
    {
        ThrowIfFailedHot(m_mediaEngine->SetMuted(muted));
    }
}

void Hot3dxRotoDraw::MediaEnginePlayer::SetSource(_In_z_ const wchar_t* sourceUri)
{
    static BSTR bstrURL = nullptr;

    if (bstrURL != nullptr)
    {
        ::CoTaskMemFree(bstrURL);
        bstrURL = nullptr;
    }

    size_t cchAllocationSize = 1 + ::wcslen(sourceUri);
    bstrURL = reinterpret_cast<BSTR>(::CoTaskMemAlloc(sizeof(wchar_t) * (cchAllocationSize)));

    if (bstrURL == 0)
    {
        ThrowIfFailedHot(E_OUTOFMEMORY);
    }

    wcscpy_s(bstrURL, cchAllocationSize, sourceUri);

    m_isPlaying = false;
    m_isInfoReady = false;
    m_isFinished = false;

    if (m_mediaEngine)
    {
        HRESULT hr = S_OK;
        hr = m_mediaEngine->SetSource(bstrURL);
        ThrowIfFailedHot(hr);
    }

}

bool Hot3dxRotoDraw::MediaEnginePlayer::TransferFrame(HANDLE textureHandle, MFVideoNormalizedRect rect, RECT rcTarget)
{
    if (m_mediaEngine != nullptr && m_isPlaying)
    {
            LONGLONG pts;
            m_mediaEngine->OnVideoStreamTick(&pts);
                 
            Microsoft::WRL::ComPtr<ID3D11Resource> mediaTexture;
            HRESULT hr = m_device->OpenSharedResource1(textureHandle, IID_PPV_ARGS(mediaTexture.GetAddressOf()));
            if (SUCCEEDED(hr))
            {
              if (m_mediaEngine->TransferVideoFrame(mediaTexture.Get(), &rect, &rcTarget, &m_bkgColor) == S_OK)
                    return true;
            }
    }
    return false;
}

void Hot3dxRotoDraw::MediaEnginePlayer::OnMediaEngineEvent(uint32_t meEvent)
{
    switch (meEvent)
    {
    case MF_MEDIA_ENGINE_EVENT_LOADEDMETADATA:
        m_isInfoReady = true;
        break;

    case MF_MEDIA_ENGINE_EVENT_CANPLAY:

        // Here we auto-play when ready...
        Play();

        break;

    case MF_MEDIA_ENGINE_EVENT_PLAY:
        break;

    case MF_MEDIA_ENGINE_EVENT_PAUSE:
        break;

    case MF_MEDIA_ENGINE_EVENT_ENDED:
        m_isFinished = true;
        break;

    case MF_MEDIA_ENGINE_EVENT_TIMEUPDATE:
        break;

    case MF_MEDIA_ENGINE_EVENT_ERROR:
#ifdef _DEBUG
        if (m_mediaEngine)
        {
            ComPtr<IMFMediaError> error;
            m_mediaEngine->GetError(&error);
            USHORT errorCode = error->GetErrorCode();
            char buff[128] = {};
            sprintf_s(buff, "ERROR: Media Foundation Event Error %u", errorCode);
            OutputDebugStringA(buff);
        }
#endif
        break;
    }
}

void Hot3dxRotoDraw::MediaEnginePlayer::GetNativeVideoSize(uint32_t& cx, uint32_t& cy)
{
    cx = cy = 0;
    if (m_mediaEngine && m_isInfoReady)
    {
        DWORD x, y;
        ThrowIfFailedHot(m_mediaEngine->GetNativeVideoSize(&x, &y));

        cx = x;
        cy = y;
    }
}
