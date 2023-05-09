//--------------------------------------------------------------------------------------
// File: MediaEnginePlayer.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "Hot3dxRotoDrawSceneRender.h"

using Microsoft::WRL::ComPtr;

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


Hot3dxRotoDraw::MediaEnginePlayer::MediaEnginePlayer(const std::shared_ptr<DX::DeviceResources>& deviceResources) noexcept :
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
    DX::ThrowIfFailed(MFStartup(MF_VERSION));

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
        DX::ThrowIfFailed(adapter->GetDesc1(&desc));

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
        DX::ThrowIfFailed(adapter->GetDesc1(&desc));

        if (desc.AdapterLuid.LowPart != adapterLuid.LowPart
            || desc.AdapterLuid.HighPart != adapterLuid.HighPart)
        {
            throw std::exception("No matching device for DirectX 12 found");
        }
    }
#endif

    ComPtr<ID3D11Device> baseDevice;
    DX::ThrowIfFailed(
        D3D11CreateDevice(
            adapter.Get(),
#if defined(NDEBUG)
            D3D_DRIVER_TYPE_UNKNOWN,
#else
            adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_WARP,
#endif
            nullptr,
            D3D11_CREATE_DEVICE_VIDEO_SUPPORT | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            baseDevice.GetAddressOf(),
            nullptr,
            nullptr
        ));

    ComPtr<ID3D10Multithread> multithreaded;
    DX::ThrowIfFailed(baseDevice.As(&multithreaded));
    multithreaded->SetMultithreadProtected(TRUE);

    DX::ThrowIfFailed(baseDevice.As(&m_device));

    // Setup Media Engine
    Microsoft::WRL::ComPtr<IMFDXGIDeviceManager> dxgiManager;
    UINT resetToken;
    DX::ThrowIfFailed(MFCreateDXGIDeviceManager(&resetToken, dxgiManager.GetAddressOf()));
    DX::ThrowIfFailed(dxgiManager->ResetDevice(m_device.Get(), resetToken));

    // Create our event callback object.
    ComPtr<MediaEngineNotify> spNotify = new MediaEngineNotify();
    if (spNotify == nullptr)
    {
        DX::ThrowIfFailed(E_OUTOFMEMORY);
    }

    spNotify->SetCallback(this);

    // Set configuration attribiutes.
    ComPtr<IMFAttributes> attributes;
    DX::ThrowIfFailed(MFCreateAttributes(attributes.GetAddressOf(), 1));
    DX::ThrowIfFailed(attributes->SetUnknown(MF_MEDIA_ENGINE_DXGI_MANAGER, reinterpret_cast<IUnknown*>(dxgiManager.Get())));
    DX::ThrowIfFailed(attributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, reinterpret_cast<IUnknown*>(spNotify.Get())));
    DX::ThrowIfFailed(attributes->SetUINT32(MF_MEDIA_ENGINE_VIDEO_OUTPUT_FORMAT, format));

    // Create MediaEngine.
    ComPtr<IMFMediaEngineClassFactory> mfFactory;
    DX::ThrowIfFailed(
        CoCreateInstance(CLSID_MFMediaEngineClassFactory,
            nullptr,
            CLSCTX_ALL,
            IID_PPV_ARGS(mfFactory.GetAddressOf())));

    DX::ThrowIfFailed(
        mfFactory->CreateInstance(0,
            attributes.Get(),
            m_mediaEngine.ReleaseAndGetAddressOf()));

    // Create MediaEngineEx
    DX::ThrowIfFailed(m_mediaEngine.As(&m_engineEx));
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
        DX::ThrowIfFailed(m_mediaEngine->Play());
        m_isPlaying = true;
        m_isFinished = false;
    }
}

void Hot3dxRotoDraw::MediaEnginePlayer::SetMuted(bool muted)
{
    if (m_mediaEngine)
    {
        DX::ThrowIfFailed(m_mediaEngine->SetMuted(muted));
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
        DX::ThrowIfFailed(E_OUTOFMEMORY);
    }

    wcscpy_s(bstrURL, cchAllocationSize, sourceUri);

    m_isPlaying = false;
    m_isInfoReady = false;
    m_isFinished = false;

    if (m_mediaEngine)
    {
        HRESULT hr = S_OK;
        hr = m_mediaEngine->SetSource(bstrURL);
        DX::ThrowIfFailed(hr);
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
        DX::ThrowIfFailed(m_mediaEngine->GetNativeVideoSize(&x, &y));

        cx = x;
        cy = y;
    }
}