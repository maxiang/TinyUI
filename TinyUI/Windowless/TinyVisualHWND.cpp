#include "../stdafx.h"
#include "TinyVisualHWND.h"
#include "../Render/TinyTransform.h"
#include "../Render/TinyCanvas.h"

namespace TinyUI
{
	namespace Windowless
	{
		TinyVisualHWND::TinyVisualHWND()
			:m_document(NULL),
			m_cacheDC(NULL),
			m_bMouseTracking(FALSE)
		{

		}
		TinyVisualHWND::~TinyVisualHWND()
		{

		}
		BOOL TinyVisualHWND::Create(HWND hParent, INT x, INT y, INT cx, INT cy)
		{
			return TinyControl::Create(hParent, x, y, cx, cy, FALSE);
		}
		DWORD TinyVisualHWND::RetrieveStyle()
		{
			return (WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW & ~WS_CAPTION);
		}
		DWORD TinyVisualHWND::RetrieveExStyle()
		{
			return (WS_EX_LEFT | WS_EX_RIGHTSCROLLBAR);
		}
		LPCSTR TinyVisualHWND::RetrieveClassName()
		{
			return TEXT("TinyVisualHWND");
		}
		LPCSTR TinyVisualHWND::RetrieveTitle()
		{
			return TEXT("TinyVisualHWND");
		}
		HICON TinyVisualHWND::RetrieveIcon()
		{
			return NULL;
		}
		BOOL TinyVisualHWND::Initialize()
		{
			m_document.Reset(new TinyVisualDocument(this));
			m_cacheDC.Reset(new TinyVisualCacheDC(m_hWND));
			return m_document->Initialize();
		}
		void TinyVisualHWND::Uninitialize()
		{
			//TODO
		}
		LRESULT TinyVisualHWND::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			PAINTSTRUCT ps = { 0 };
			HDC hDC = BeginPaint(m_hWND, &ps);
			m_document->Draw(m_cacheDC, ps.rcPaint);
			EndPaint(m_hWND, &ps);
			return FALSE;
		}
		LRESULT TinyVisualHWND::OnErasebkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = TRUE;
			return FALSE;
		}
		LRESULT TinyVisualHWND::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			m_size.cx = LOWORD(lParam);
			m_size.cy = HIWORD(lParam);
			//设置桌面元素
			TinyVisual* spvis = m_document->GetParent(NULL);
			if (spvis)
				spvis->SetSize(m_size);
			m_cacheDC->SetSize(m_size.cx, m_size.cy);
			::RedrawWindow(m_hWND, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return FALSE;
		}
		LRESULT TinyVisualHWND::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			return FALSE;
		}

		LRESULT TinyVisualHWND::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			Uninitialize();
			PostQuitMessage(0);//退出应用程序
			return FALSE;
		}

		LRESULT TinyVisualHWND::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			Initialize();
			return FALSE;
		}
		LRESULT TinyVisualHWND::OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			return FALSE;
		}
		LRESULT TinyVisualHWND::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			if (!m_bMouseTracking)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.hwndTrack = m_hWND;
				tme.dwFlags = TME_LEAVE;
				tme.dwHoverTime = 10;
				m_bMouseTracking = _TrackMouseEvent(&tme);
			}
			TinyPoint pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return m_document->OnMouseMove(pos, (DWORD)wParam);
		}
		LRESULT TinyVisualHWND::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			if (m_bMouseTracking)
				m_bMouseTracking = FALSE;
			return m_document->OnMouseLeave();
		}
		LRESULT TinyVisualHWND::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			TinyPoint pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			::ScreenToClient(m_hWND, &pos);
			return m_document->OnMouseWheel(pos, GET_WHEEL_DELTA_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam));
		}
		LRESULT TinyVisualHWND::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			TinyPoint pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return m_document->OnLButtonDown(pos, (DWORD)wParam);
		}
		LRESULT TinyVisualHWND::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			TinyPoint pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return m_document->OnLButtonUp(pos, (DWORD)wParam);
		}
		LRESULT TinyVisualHWND::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			TinyPoint pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return m_document->OnRButtonDown(pos, (DWORD)wParam);
		}
		LRESULT TinyVisualHWND::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			TinyPoint pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return m_document->OnRButtonUp(pos, (DWORD)wParam);
		}
		LRESULT TinyVisualHWND::OnMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			TinyPoint pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return m_document->OnMButtonDown(pos, (DWORD)wParam);
		}
		LRESULT TinyVisualHWND::OnMButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			TinyPoint pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return m_document->OnMButtonUp(pos, (DWORD)wParam);
		}
		LRESULT TinyVisualHWND::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			return FALSE;
		}
		LRESULT TinyVisualHWND::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = FALSE;
			return FALSE;
		}
		LRESULT TinyVisualHWND::OnNCCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			bHandled = TRUE;
			if (static_cast<BOOL>(wParam))
			{
				NCCALCSIZE_PARAMS* ps = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				ps->rgrc[0].left = ps->lppos->x;
				ps->rgrc[0].top = ps->lppos->y;
				ps->rgrc[0].bottom = ps->lppos->y + ps->lppos->cy;
				ps->rgrc[0].right = ps->lppos->x + ps->lppos->cx;
			}
			return TRUE;
		}
		LRESULT TinyVisualHWND::OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			ASSERT(m_document);
			bHandled = TRUE;
			TinyPoint pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			TinyRectangle rectangle;
			this->GetWindowRect(rectangle);
			if (!rectangle.PtInRect(pos))
				return HTNOWHERE;
			INT cx = GetSystemMetrics(SM_CXBORDER);
			INT cy = GetSystemMetrics(SM_CYBORDER);
			if (pos.x >= rectangle.left && pos.x <= (rectangle.left + cx) && pos.y >= rectangle.top && pos.y <= (rectangle.top + cy))
				return HTTOPLEFT;
			if (pos.x >= rectangle.left && pos.x <= (rectangle.left + cx) && pos.y > (rectangle.top + cy) && pos.y < (rectangle.bottom - cy))
				return HTLEFT;
			if (pos.x >= rectangle.left && pos.x <= (rectangle.left + cx) && pos.y >= (rectangle.bottom - cy) && pos.y <= rectangle.bottom)
				return HTBOTTOMLEFT;
			if (pos.x >(rectangle.left + cx) && pos.x < (rectangle.right - cx) && pos.y >= rectangle.top && pos.y <= (rectangle.top + cy))
				return HTTOP;
			if (pos.x >= (rectangle.right - cx) && pos.x <= rectangle.right && pos.y >= rectangle.top && pos.y <= (rectangle.top + cy))
				return HTTOPRIGHT;
			if (pos.x >= (rectangle.right - cx) && pos.x <= rectangle.right && pos.y > (rectangle.top + cy) && pos.y < (rectangle.bottom - cy))
				return HTRIGHT;
			if (pos.x >= (rectangle.right - cx) && pos.x <= rectangle.right && pos.y >= (rectangle.bottom - cy) && pos.y <= rectangle.bottom)
				return HTBOTTOMRIGHT;
			if (pos.x >(rectangle.left + cx) && pos.x < (rectangle.right - cx) && pos.y >= (rectangle.bottom - cy) && pos.y <= rectangle.bottom)
				return HTBOTTOM;
			ScreenToClient(m_hWND, &pos);
			if (m_document->GetParent(NULL) == m_document->GetVisualByPos(pos.x, pos.y))
				return HTCAPTION;
			return HTCLIENT;
		}
	}
}
