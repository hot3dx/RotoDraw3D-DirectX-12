#pragma once

#include <ppltasks.h>

#include <wrl.h>
#include <wrl\client.h>
#include <wrl\implements.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include "Common\d3dx12.h"
#include <d3d11_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
// Uses NuGet.org package WinPixEventRuntime 1.0.200127001
// and Microsoft.UI.Xaml.2.5.0-prerelease.200812001
#include <packages\WinPixEventRuntime.1.0.240308001\Include\WinPixEventRuntime\pix3.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include <agile.h>
#include <concrt.h>
#include <collection.h>
#include "App.xaml.h"
#include <synchapi.h>
#include <strsafe.h>
#ifndef EXTERNAL_VCR102
#define EXTERNAL_VCR102 false
#endif
#ifndef EXTERNAL_VCR001
#define EXTERNAL_VCR001 false
#endif

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

#include <algorithm>
#include <atomic>
#include <array>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
//#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <malloc.h>
#include <stddef.h>
#include <cstdint>

#define XAUDIO2_HELPER_FUNCTIONS 1
#include <x3daudio.h>
#include <xaudio2fx.h>
#include <xaudio2.h>
#include <xapo.h>
#include <xapobase.h>
#include <xapofx.h>
#include <hrtfapoapi.h>

#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <mfmediaengine.h>
#include <mferror.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;
using namespace DirectX;

#include <AudioFileReaderXaml12.h>
#include <XAudio2HelpersXaml12.h>
#include "OmnidirectionalSound.h"
#include "CardioidSound.h"
#include "CustomDecay.h"
#include <AudioXaml12.h>
#include <CommonStatesXaml12.h>
#include <DirectXHelpersXaml12.h>
#include <DDSTextureLoaderXaml12.h>
#include <DescriptorHeapXaml12.h>
#include <EffectsXaml12.h>
#include <GeometricPrimitiveXaml12.h>
#include <GraphicsMemoryXaml12.h>
#include <ModelXaml12.h>
#include <PrimitiveBatchXaml12.h>
#include <MyResourceUploadBatchXaml12.h>
#include <RenderTargetStateXaml12.h>
#include <SimpleMathXaml12.h>
#include <SpriteBatchXaml12.h>
#include <SpriteFontXaml12.h> 
#include <VertexTypesXaml12.h>

using namespace DirectX::DXTKXAML12;

#define SINGLETHREADED FALSE

#define HRTF_2PI    6.283185307f

#ifndef RGBA_FRACTAL
#define RGBA_FRACTAL 0.00390625F
#endif


#ifndef RGB_MAKE

#define RGBA_GETALPHA(rgb)      ((rgb) >> 24)
#define RGBA_GETRED(rgb)        (((rgb) >> 16) & 0xff)
#define RGBA_GETGREEN(rgb)      (((rgb) >> 8) & 0xff)
#define RGBA_GETBLUE(rgb)       ((rgb) & 0xff)
#define RGBA_MAKE(r, g, b, a)   ((D3DRGBA) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

/*
 * Format of CI colors is
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |    alpha      |         color index           |   fraction    |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define CI_GETALPHA(ci)    ((ci) >> 24)
#define CI_GETINDEX(ci)    (((ci) >> 8) & 0xffff)
#define CI_GETFRACTION(ci) ((ci) & 0xff)
#define CI_ROUNDINDEX(ci)  CI_GETINDEX((ci) + 0x80)
#define CI_MASKALPHA(ci)   ((ci) & 0xffffff)
#define CI_MAKE(a, i, f)    (((a) << 24) | ((i) << 8) | (f))

 /*
  * Format of RGBA colors is
  *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  *  |    alpha      |      red      |     green     |     blue      |
  *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  */
#define RGBA_GETALPHA(rgb)      ((rgb) >> 24)
#define RGBA_GETRED(rgb)        (((rgb) >> 16) & 0xff)
#define RGBA_GETGREEN(rgb)      (((rgb) >> 8) & 0xff)
#define RGBA_GETBLUE(rgb)       ((rgb) & 0xff)
#define RGBA_MAKE(r, g, b, a)   ((D3DRGBA) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

  /* D3DRGB and D3DRGBA may be used as initialisers for D3DRGBAs
   * The float values must be in the range 0..1
   */
#define D3DRGB(r, g, b) \
    (0xff000000L | ( ((long)((r) * 255)) << 16) | (((long)((g) * 255)) << 8) | (long)((b) * 255))
#define D3DRGBA(r, g, b, a) \
    (   (((long)((a) * 255)) << 24) | (((long)((r) * 255)) << 16) \
    |   (((long)((g) * 255)) << 8) | (long)((b) * 255) \
    )

   /*
    * Format of RGB colors is
    *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    *  |    ignored    |      red      |     green     |     blue      |
    *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
#define RGB_GETRED(rgb)         (((rgb) >> 16) & 0xff)
#define RGB_GETGREEN(rgb)       (((rgb) >> 8) & 0xff)
#define RGB_GETBLUE(rgb)        ((rgb) & 0xff)
#define RGBA_SETALPHA(rgba, x) (((x) << 24) | ((rgba) & 0x00ffffff))
#define RGB_MAKE(r, g, b)       ((D3DRGBA) (((r) << 16) | ((g) << 8) | (b)))
#define RGBA_TORGB(rgba)       ((D3DRGBA) ((rgba) & 0xffffff))
#define RGB_TORGBA(rgb)        ((D3DRGBA) ((rgb) | 0xff000000))

#endif