#include "stdafx.h"
#include "ChatFrame.h"

ChatFrame::ChatFrame()
{
}


ChatFrame::~ChatFrame()
{
}

LRESULT ChatFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	PostQuitMessage(0);
	return FALSE;
}

BOOL ChatFrame::Create(HWND hParent, INT x, INT y, INT cx, INT cy)
{
	return TinyControl::Create(hParent, x, y, cx, cy, FALSE);
}
DWORD ChatFrame::RetrieveStyle()
{
	return (WS_VISIBLE | WS_OVERLAPPEDWINDOW);
}

DWORD ChatFrame::RetrieveExStyle()
{
	return (WS_EX_TOPMOST);
}

LPCSTR ChatFrame::RetrieveClassName()
{
	return TEXT("ChatFrame");
}

LPCSTR ChatFrame::RetrieveTitle()
{
	return TEXT("ChatFrame");
}

HICON ChatFrame::RetrieveIcon()
{
	return NULL;
}

void ChatFrame::OnFinish()
{
	LONG size = 0;
	m_waveFile.Read(m_bits, 4096, &size);
	m_audio.Fill(m_bits, size);
}

LRESULT ChatFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	m_waveFile.Open("D:\\���� - �Ҵ�����.wav");
	//m_audio.SetCallback(BindCallback(&ChatFrame::OnFinish, this));
	m_audio.Open(m_waveFile.GetFormat());
	m_bits.Reset(new BYTE[4096]);
	m_audio.Start();
	LONG size = 0;
	for (;;)
	{
		m_waveFile.Read(m_bits, 4096, &size);
		m_audio.Fill(m_bits, size, INFINITE);
	}
	//m_request.Open("http://10.110.48.109:65449/6696864323");
	//Network::TinyHTTPResponse* response = m_request.GetResponse();

	/*m_analyserBAR.Create(m_hWND, 10, 10, 298, 92);*/
	/*m_analyserBAR.Create(m_hWND, 10, 10, 300, 180);
	m_analyserBAR.SetText("����");
	m_audioDSP.Initialize();*/
	//m_wavePlayer.Submit(m_analyserBAR.Handle());
	/*DISPLAY_DEVICE dd;
	ZeroMemory(&dd, sizeof(dd));
	dd.cb = sizeof(dd);
	DWORD dwCount = 0;
	string RDP_EMD = "RDP Encoder Mirror Driver";
	while (EnumDisplayDevices(NULL, dwCount, &dd, EDD_GET_DEVICE_INTERFACE_NAME))
	{
		dwCount++;

		DEVMODE dm;
		ZeroMemory(&dm, sizeof(DEVMODE));
		dm.dmSize = sizeof(DEVMODE);
		dm.dmDriverExtra = 0;
		if (EnumDisplaySettingsEx(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm, EDS_ROTATEDMODE))
		{
			m_capture = new RDPCapture(dd, dm);
		}

		if (RDP_EMD == string(dd.DeviceString))
		{
			TinyRectangle rectangle;
			::GetClientRect(m_hWND, &rectangle);
			m_capture->Init(rectangle.left, rectangle.top, rectangle.Width(), rectangle.Height());
		}
	}*/
	/*TinyRectangle rectangle;
	::GetWindowRect(m_hWND, &rectangle);
	TinyClientDC dc(m_hWND);
	TinyImage image;
	image.Open("D:\\girl-2.png");
	TinyMemDC memDC2(dc, TO_CX(rectangle), TO_CY(rectangle));
	TinyMemDC memDC1(memDC2, image);
	memDC1.Render()
	BLENDFUNCTION blendFunction;
	blendFunction.AlphaFormat = AC_SRC_ALPHA;
	blendFunction.BlendFlags = 0;
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.SourceConstantAlpha = 255;
	HDC hDC = GetDC(NULL);
	SIZE size = rectangle.Size();
	POINT pos = { 0,0 };
	UpdateLayeredWindow(m_hWND, hDC, &pos, &size, memDC, &pos, 0, &blendFunction, 2);
	ReleaseDC(NULL, hDC);*/
	return FALSE;
}

LRESULT ChatFrame::OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	//SAFE_DELETE(m_capture);
	return FALSE;
}

LRESULT ChatFrame::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	PAINTSTRUCT ps = { 0 };
	HDC hDC = BeginPaint(m_hWND, &ps);
	EndPaint(m_hWND, &ps);
	return FALSE;
}

LRESULT ChatFrame::OnErasebkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT ChatFrame::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT ChatFrame::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT ChatFrame::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT ChatFrame::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT ChatFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}
