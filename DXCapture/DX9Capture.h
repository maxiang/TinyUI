#pragma once
#include "DX.h"
#include <d3d9.h>

namespace DXCapture
{
	typedef IDirect3D9* (WINAPI*D3D9CREATEPROC)(UINT);
	typedef HRESULT(WINAPI*D3D9CREATEEXPROC)(UINT, IDirect3D9Ex**);

	HRESULT STDMETHODCALLTYPE DX9EndScene(IDirect3DDevice9 *pThis);
	HRESULT STDMETHODCALLTYPE DX9Present(IDirect3DDevice9 *pThis, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	HRESULT STDMETHODCALLTYPE DX9PresentEx(IDirect3DDevice9Ex *pThis, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
	HRESULT STDMETHODCALLTYPE DX9SwapPresent(IDirect3DSwapChain9 *swap, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
	HRESULT STDMETHODCALLTYPE DX9Reset(IDirect3DDevice9 *pThis, D3DPRESENT_PARAMETERS *params);
	HRESULT STDMETHODCALLTYPE DX9ResetEx(IDirect3DDevice9Ex *pThis, D3DPRESENT_PARAMETERS *params, D3DDISPLAYMODEEX *fullscreenData);

	DXGI_FORMAT GetDXGIFormat(D3DFORMAT format);
	/// <summary>
	/// XX9�������ݲ���
	/// </summary>
	class DX9Capture
	{
	public:
		DX9Capture(DX& dx);
		~DX9Capture();
		BOOL Initialize(HWND hWND);
		BOOL Render(IDirect3DDevice9 *device);
		void Reset();
		BOOL Setup(IDirect3DDevice9 *pThis);
		BOOL DX9GPUHook(IDirect3DDevice9 *device);
	public:
		SharedCaptureDATA				m_captureDATA;
		LPVOID							m_currentPointer;
		D3DFORMAT						m_d3dFormat;
		DXGI_FORMAT						m_dxgiFormat;
		INT								m_patchType;
		BOOL							m_bCapturing;
		BOOL							m_bTextures;
		HANDLE							m_hTextureHandle;
		HMODULE							m_hD3D9;
		TinyComPtr<ID3D10Device1>		m_d3d10;
		TinyComPtr<ID3D10Resource>		m_resource;
		TinyComPtr<IDirect3DSurface9>	m_surface;
		TinyDetour						m_dX9Release;
		TinyDetour						m_dX9EndScene;
		TinyDetour						m_dX9Reset;
		TinyDetour						m_dX9ResetEx;
		TinyDetour						m_dX9Present;
		TinyDetour						m_dX9PresentEx;
		TinyDetour						m_dX9SwapPresent;
		TinyScopedLibrary				m_d3d10_1;
		TinyScopedLibrary				m_dxgi;
		DX&								m_dx;
	};
	SELECTANY extern DX9Capture g_dx9(g_dx);
}

