#pragma once
#include <d3d9.h>
#include <D3D10_1.h>
#include <DXGI.h>
#include "D3DCommon.h"
#include "D3DDetour.h"
#include "D3DCore.h"
#include "D3DThunk.h"
#include "SharedMemory.h"
#include <string>
using namespace std;

namespace D3D
{
	typedef HRESULT(WINAPI *CREATEDXGIFACTORY1PROC)(REFIID riid, void **ppFactory);

#define TEXTURE_MEMORY          TEXT("Local\\TextureMemory")
#define NUM_BUFFERS				3
#define ZERO_ARRAY				{0, 0, 0}
#define BEGIN_CAPTURE_EVENT		TEXT("BeginCapture")
#define END_CAPTURE_EVENT       TEXT("EndCapture")
#define CAPTURE_READY_EVENT     TEXT("CaptureReady")
#define CAPTURETYPE_MEMORY      1
#define CAPTURETYPE_SHAREDTEX   2
	typedef struct tagCaptureEntry
	{
		UINT		CaptureType;
		DWORD		Format;
		UINT		Width;
		UINT		Height;
		BOOL		bFlip;
		UINT		Pitch;
		DWORD		MapSize;
		HWND		HwndCapture;
	}SharedTexture;

#pragma pack(push, 8)

	typedef struct tagSharedTextureData
	{
		LONGLONG    FrameTime;
		HANDLE      TextureHandle;
	}SharedTextureData;

#pragma pack(pop)
	/// <summary>
	/// D3D9
	/// </summary>
	class D3D9Capture
	{
	public:
		D3D9Capture();
		~D3D9Capture();
		BOOL Initialize(HWND hWND);
		BOOL D3D9Draw(IDirect3DDevice9 *device);
		void D3DReset();
		BOOL D3D9GPUHook(IDirect3DDevice9 *device);
		BOOL InitializeSharedMemoryGPUCapture(SharedTextureData **texData);
		static D3D9Capture& Instance();
	private:
		static HRESULT STDMETHODCALLTYPE D3D9EndScene(IDirect3DDevice9 *device);
		static HRESULT STDMETHODCALLTYPE D3D9Present(IDirect3DDevice9 *device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
		static HRESULT STDMETHODCALLTYPE D3D9PresentEx(IDirect3DDevice9Ex *device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
		static HRESULT STDMETHODCALLTYPE D3D9SwapPresent(IDirect3DSwapChain9 *swap, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
		static HRESULT STDMETHODCALLTYPE D3D9Reset(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *params);
		static HRESULT STDMETHODCALLTYPE D3D9ResetEx(IDirect3DDevice9Ex *device, D3DPRESENT_PARAMETERS *params, D3DDISPLAYMODEEX *fullscreenData);
	public:
		SharedTexture				m_d3dSharedTexture;
		SharedTextureData*			m_pSharedTextureData;
		D3DFORMAT					m_d3dFormat;
		DXGI_FORMAT					m_dxgiFormat;
		HINSTANCE					m_hD3D9;
		HINSTANCE					m_hD3D10_1;
		HINSTANCE					m_hDXGI;
		HANDLE						m_pSharedHandle;
		BOOL						m_bCapturing;
		BOOL						m_bTextures;
		BOOL						m_bUseSharedTextures;
		DWORD						m_dwCurrentCapture;
		IDirect3DDevice9*			m_pGameDevice;
		CLock						m_lock;
		CPerformanceTimer			m_timer;
		CComPtr<ID3D10Device1>		m_d3d10Device1;
		CComPtr<ID3D10Resource>		m_d3d10Resource;
		CComPtr<IDirect3DSurface9>	m_d3d9TextureGame;
		CSharedMemory				m_textureMemery;
		CD3DDetour					m_d3d9EndScene;
		CD3DDetour					m_d3d9Reset;
		CD3DDetour					m_d3d9ResetEx;
		CD3DDetour					m_d3d9Present;
		CD3DDetour					m_d3d9PresentEx;
		CD3DDetour					m_d3d9SwapPresent;
		CEvent						m_beginCapture;
		CEvent						m_endCapture;
		INT							m_patchType;
		LONGLONG					m_lastTime;
	};
}