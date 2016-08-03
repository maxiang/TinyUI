#include "../stdafx.h"
#include "TinyTextHost.h"

namespace TinyUI
{
	EXTERN_C const IID IID_ITextServices =
	{
		0x8d33f740,
		0xcf58,
		0x11ce,
		{ 0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5 }
	};

	EXTERN_C const IID IID_ITextHost =
	{
		0xc5bdd8d0,
		0xd26e,
		0x11ce,
		{ 0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5 }
	};


	LONG DXtoHimetricX(LONG dx, LONG xPerInch)
	{
		return (LONG)MulDiv(dx, HIMETRIC_PER_INCH, xPerInch);
	}
	LONG DYtoHimetricY(LONG dy, LONG yPerInch)
	{
		return (LONG)MulDiv(dy, HIMETRIC_PER_INCH, yPerInch);
	}

	TinyTextHost::TinyTextHost()
		:m_hInstance(NULL)
	{
		m_sizelExtent.cx = m_sizelExtent.cy = 0;
	}
	TinyTextHost::~TinyTextHost()
	{
		m_ts.Release();
		if (m_hInstance)
		{
			::FreeLibrary(m_hInstance);
			m_hInstance = NULL;
		}
	}
	BOOL TinyTextHost::Initialize(HWND hWND)
	{
		ASSERT(hWND);
		m_hWND = hWND;
		m_hInstance = ::LoadLibrary(TEXT("msftedit.dll"));
		if (m_hInstance)
		{
			pfnCreateTextServices ps = (pfnCreateTextServices)::GetProcAddress(m_hInstance, "CreateTextServices");
			if (ps != NULL)
			{
				IUnknown* pUnknown = NULL;
				if (ps(NULL, static_cast<ITextHost*>(this), &pUnknown) == S_OK)
				{
					pUnknown->QueryInterface(IID_ITextServices, (void**)&m_ts);
					pUnknown->Release();
					m_ts->OnTxInPlaceDeactivate();
					m_ts->OnTxUIDeactivate();
					m_ts->TxSendMessage(WM_KILLFOCUS, 0, 0, 0);
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	HDC TinyTextHost::TxGetDC()
	{
		ASSERT(m_hWND);
		HDC hDC = ::GetDC(m_hWND);
		return hDC;
	}

	INT TinyTextHost::TxReleaseDC(HDC hdc)
	{
		ASSERT(m_hWND);
		return ::ReleaseDC(m_hWND, hdc);
	}

	BOOL TinyTextHost::TxShowScrollBar(INT fnBar, BOOL fShow)
	{
		return S_OK;
	}

	BOOL TinyTextHost::TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags)
	{
		return S_OK;
	}

	BOOL TinyTextHost::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)
	{
		return S_OK;
	}

	BOOL TinyTextHost::TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw)
	{
		return S_OK;
	}

	void TinyTextHost::TxInvalidateRect(LPCRECT prc, BOOL fMode)
	{
		::InvalidateRect(m_hWND, prc, fMode);
	}

	void TinyTextHost::TxViewChange(BOOL fUpdate)
	{

	}

	BOOL TinyTextHost::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)
	{
		ASSERT(m_hWND);
		BOOL bRes = ::CreateCaret(m_hWND, hbmp, xWidth, yHeight);
		return bRes;
	}

	BOOL TinyTextHost::TxShowCaret(BOOL fShow)
	{
		ASSERT(m_hWND);
		if (fShow)
			return ::ShowCaret(m_hWND);
		else
			return ::HideCaret(m_hWND);
	}

	BOOL TinyTextHost::TxSetCaretPos(INT x, INT y)
	{
		return ::SetCaretPos(x, y);
	}

	BOOL TinyTextHost::TxSetTimer(UINT idTimer, UINT uTimeout)
	{
		return S_OK;
	}

	void TinyTextHost::TxKillTimer(UINT idTimer)
	{

	}

	void TinyTextHost::TxScrollWindowEx(INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll)
	{

	}

	void TinyTextHost::TxSetCapture(BOOL fCapture)
	{
		if (fCapture)
		{
			SetCapture(m_hWND);
		}
		else
		{
			ReleaseCapture();
		}
	}

	void TinyTextHost::TxSetFocus()
	{

	}

	void TinyTextHost::TxSetCursor(HCURSOR hcur, BOOL fText)
	{
		SetCursor(hcur);
	}

	BOOL TinyTextHost::TxScreenToClient(LPPOINT lppt)
	{
		return E_NOTIMPL;
	}

	BOOL TinyTextHost::TxClientToScreen(LPPOINT lppt)
	{
		return E_NOTIMPL;
	}

	HRESULT TinyTextHost::TxActivate(LONG * plOldState)
	{
		return S_OK;
	}

	HRESULT TinyTextHost::TxDeactivate(LONG lNewState)
	{
		return S_OK;
	}

	HRESULT TinyTextHost::TxGetClientRect(LPRECT prc)
	{
		GetClientRect(m_hWND, prc);
		return S_OK;
	}

	HRESULT TinyTextHost::TxGetViewInset(LPRECT prc)
	{
		prc->left = prc->right = prc->top = prc->bottom = 0;
		return S_OK;
	}

	HRESULT TinyTextHost::TxGetCharFormat(const CHARFORMATW **ppCF)
	{
		return S_OK;
	}

	HRESULT TinyTextHost::TxGetParaFormat(const PARAFORMAT **ppPF)
	{
		return S_OK;
	}

	COLORREF TinyTextHost::TxGetSysColor(int nIndex)
	{
		return GetSysColor(nIndex);
	}

	HRESULT TinyTextHost::TxGetBackStyle(TXTBACKSTYLE *pstyle)
	{
		*pstyle = TXTBACK_OPAQUE;
		return S_OK;
	}

	HRESULT TinyTextHost::TxGetMaxLength(DWORD *plength)
	{
		return E_NOTIMPL;
	}

	HRESULT TinyTextHost::TxGetScrollBars(DWORD *pdwScrollBar)
	{
		return E_NOTIMPL;
	}

	HRESULT TinyTextHost::TxGetPasswordChar(_Out_ TCHAR *pch)
	{
		return E_NOTIMPL;
	}

	HRESULT TinyTextHost::TxGetAcceleratorPos(LONG *pcp)
	{
		return E_NOTIMPL;
	}

	HRESULT TinyTextHost::TxGetExtent(LPSIZEL lpExtent)
	{
		*lpExtent = m_sizelExtent;
		return S_OK;
	}

	HRESULT TinyTextHost::OnTxCharFormatChange(const CHARFORMATW * pCF)
	{
		return E_NOTIMPL;
	}

	HRESULT TinyTextHost::OnTxParaFormatChange(const PARAFORMAT * pPF)
	{
		return E_NOTIMPL;
	}

	HRESULT TinyTextHost::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits)
	{
		DWORD bits = TXTBIT_MULTILINE | TXTBIT_RICHTEXT | TXTBIT_WORDWRAP;
		*pdwBits = bits & dwMask;
		return S_OK;
	}

	HRESULT TinyTextHost::TxNotify(DWORD iNotify, void *pv)
	{
		return E_NOTIMPL;
	}

	HIMC TinyTextHost::TxImmGetContext()
	{
		return NULL;
	}

	void TinyTextHost::TxImmReleaseContext(HIMC himc)
	{

	}

	HRESULT TinyTextHost::TxGetSelectionBarWidth(LONG *lSelBarWidth)
	{
		*lSelBarWidth = 0;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE TinyTextHost::QueryInterface(REFIID riid, void **ppvObject)
	{
		if (IsEqualIID(riid, IID_ITextHost) || IsEqualIID(riid, IID_IUnknown))
		{
			*ppvObject = this;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
		AddRef();
		return NOERROR;
	}

	ULONG STDMETHODCALLTYPE TinyTextHost::AddRef(void)
	{
		return InterlockedIncrement(&this->m_cRef);
	}

	ULONG STDMETHODCALLTYPE TinyTextHost::Release(void)
	{
		if (InterlockedDecrement(&this->m_cRef))
		{
			return this->m_cRef;
		}
		delete this;
		return NOERROR;
	}

}

