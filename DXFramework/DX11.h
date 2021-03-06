#pragma once
#include "DXFramework.h"

namespace DXFramework
{
	class DX11RenderView;

	class DX11 : public TinyLock
	{
		DISALLOW_COPY_AND_ASSIGN(DX11)
	public:
		DX11();
		~DX11();
		BOOL Initialize(HWND hWND, INT cx, INT cy);
		BOOL ResizeView(INT cx = 0, INT cy = 0);
		BOOL SetViewport(const TinyPoint& pos, const TinySize& size);
		void AllowBlend(BOOL bAllow, FLOAT blendFactor[4]);
		void AllowDepth(BOOL bAllow);
		void SetRenderTexture2D(DX11RenderView* render2D);
		void SetMatrixs(const TinySize& size);
		void Present();
		void Flush();
		HWND					GetHWND() const;
		BOOL					IsValid() const;
		ID3D11Device*			GetD3D() const;
		IDXGISwapChain*			GetSwap() const;
		ID3D11DeviceContext*	GetImmediateContext() const;
		DX11RenderView*			GetRender2D() const;
		XMMATRIX*				GetMatrixs();
		TinySize				GetSize() const;
	private:
		TinyComPtr<ID3D11Device>			m_d3d;
		TinyComPtr<IDXGISwapChain>			m_swap;
		TinyComPtr<ID3D11DeviceContext>		m_immediateContext;
		TinyComPtr<ID3D11RasterizerState>	m_rasterizerState;
		TinyComPtr<ID3D11BlendState>		m_enableBlendState;
		TinyComPtr<ID3D11BlendState>		m_disableBlendState;
		TinyComPtr<ID3D11DepthStencilState>	m_enableDepthState;
		TinyComPtr<ID3D11DepthStencilState>	m_disableDepthState;
		TinyScopedPtr<DX11RenderView>		m_background2D;
		DX11RenderView*						m_render2D;
		HWND								m_hWND;
		XMMATRIX							m_matrixs[3];
	};
}

