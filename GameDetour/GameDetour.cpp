// GameDetour.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "GameDetour.h"
#include "DX9Capture.h"
#include "DXGICapture.h"
#include "OpenGLCapture.h"
using namespace DXCapture;

namespace GameDetour
{
	GameCapture::GameCapture()
		:m_hWNDD3D(NULL),
		m_bDX9Detour(FALSE),
		m_bDXGIDetour(FALSE)
	{

	}
	GameCapture::~GameCapture()
	{

	}
	BOOL GameCapture::Attach(HMODULE hModule)
	{
		LOG(INFO) << "GameCapture::Attach\n";
		m_hInstance = hModule;
		return m_task.Submit(BindCallback(&GameCapture::OnMessagePump, this));
	}
	void GameCapture::Detach()
	{
		LOG(INFO) << "GameCapture::Detach\n";
		m_bDX9Detour = m_bDXGIDetour = FALSE;
		if (m_hWNDD3D != NULL)
		{
			::DestroyWindow(m_hWNDD3D);
			m_hWNDD3D = NULL;
		}
		PostQuitMessage(0);
		m_task.Close(INFINITE);
	}
	BOOL GameCapture::BeginCapture()
	{
		if (!g_dx.Initialize())
			return FALSE;
		LOG(INFO) << "g_dx.Initialize() OK\n";
		LOG(INFO) << "m_bDX9Detour:" << m_bDX9Detour << "\n";
		if (!m_bDX9Detour)
		{
			LOG(INFO) << "Begin g_dx9.Initialize\n";
			m_bDX9Detour = g_dx9.Initialize(m_hWNDD3D);
		}
		return m_bDX9Detour;
	}
	void GameCapture::EndCapture()
	{
		g_dx.Uninitialize();
	}
	void GameCapture::OnMessagePump()
	{
		WNDCLASS wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.style = CS_OWNDC;
		wc.hInstance = m_hInstance;
		wc.lpfnWndProc = (WNDPROC)DefWindowProc;
		wc.lpszClassName = D3D_WINDOWCLASS;
		if (RegisterClass(&wc))
		{
			m_hWNDD3D = CreateWindowEx(0,
				D3D_WINDOWCLASS,
				TEXT("D3D Caption Window"),
				WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
				0, 0,
				1, 1,
				NULL,
				NULL,
				m_hInstance,
				NULL
			);
		}
		if (m_hWNDD3D != NULL)
		{
			BeginCapture();
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			EndCapture();
		}
	}
}

