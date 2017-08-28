#include "stdafx.h"
#include "MShadowController.h"
#include "MShowApp.h"

namespace MShow
{
	MShadowController::MShadowController(MShadowView& view, MClock& clock)
		:m_view(view),
		m_bBreak(FALSE),
		m_videoFPS(25),
		m_clock(clock)
	{
		m_signal.CreateEvent();
	}

	MShadowController::~MShadowController()
	{

	}
	BOOL MShadowController::Initialize()
	{
		TinyRectangle s;
		m_view.GetClientRect(&s);
		if (!m_dx11.Initialize(m_view.Handle(), s.Size().cx, s.Size().cy))
			return FALSE;
		return TRUE;
	}
	BOOL MShadowController::SetPulgSize(const TinySize& size)
	{
		MPreviewController* pCTRL = MShowApp::GetInstance().GetController().GetPreviewController();
		if (!pCTRL)
			return FALSE;
		m_pulgSize = size;
		m_image2D.Destory();
		if (!m_image2D.Load(m_dx11, pCTRL->GetRenderView().GetHandle()))
			return FALSE;
		m_copy2D.Destory();
		if (!m_copy2D.Create(m_dx11, m_pulgSize.cx, m_pulgSize.cy, NULL, TRUE))
			return FALSE;
		return TRUE;
	}

	TinySize MShadowController::GetPulgSize() const
	{
		return m_pulgSize;
	}

	void MShadowController::SetVideoFPS(INT	videoFPS)
	{
		m_videoFPS = videoFPS;
	}

	INT	MShadowController::GetVideoFPS() const
	{
		return m_videoFPS;
	}

	MShadowView& MShadowController::GetView()
	{
		return m_view;
	}

	MClock&	MShadowController::GetClock()
	{
		return m_clock;
	}

	TinyEvent&	MShadowController::GetSignal()
	{
		return m_signal;
	}

	MPacketAllocQueue&	MShadowController::GetVideoQueue()
	{
		return m_videoQueue;
	}

	void MShadowController::OnMessagePump()
	{
		SampleTag sampleTag;
		for (;;)
		{
			if (m_bBreak)
				break;
			while (m_clock.GetBaseTime() == -1);
			TinyEvent& signal = MShow::MShowApp::GetInstance().GetController().GetPreviewController()->GetSignal();
			if (signal.Lock(1000))
			{
				OnVideo(sampleTag);
				m_clock.SetVideoPTS(MShow::MShowApp::GetInstance().GetQPCTimeMS());//������Ƶ��ʱ��
				sampleTag.timestamp = m_clock.GetVideoPTS() - m_clock.GetBaseTime();
				m_videoQueue.Push(sampleTag);
				m_signal.SetEvent();
			}
		}
		//DWORD dwMS = static_cast<DWORD>(1000 / m_videoFPS);
		//TinyTimer timer;
		//SampleTag sampleTag;
		//LONG compensate = 0;
		//for (;;)
		//{
		//	if (m_bBreak)
		//		break;
		//	while (m_clock.GetBaseTime() == -1);
		//	ZeroMemory(&sampleTag, sizeof(sampleTag));
		//	m_clock.SetVideoPTS(MShow::MShowApp::GetInstance().GetQPCTimeMS());//������Ƶ��ʱ��
		//	INT delay = dwMS - OnVideo(sampleTag) - compensate;
		//	sampleTag.timestamp = m_clock.GetVideoPTS() - m_clock.GetBaseTime();
		//	m_videoQueue.Push(sampleTag);
		//	m_signal.SetEvent();
		//	//������1ms���Ȳ���
		//	m_timeQPC.BeginTime();
		//	timer.Wait(delay < 0 ? 0 : delay, 1000);
		//	m_timeQPC.EndTime();
		//	compensate = m_timeQPC.GetMillisconds() - delay;
		//	compensate = compensate < 0 ? 0 : compensate;
		//}
	}

	DWORD MShadowController::OnVideo(SampleTag& sampleTag)
	{
		m_timeQPC.BeginTime();
		ZeroMemory(&sampleTag, sizeof(sampleTag));
		m_copy2D.Copy(m_dx11, m_image2D);
		BYTE* bits = NULL;
		UINT pitch = 0;
		if (m_copy2D.Map(m_dx11, bits, pitch, TRUE))
		{
			sampleTag.size = pitch * m_pulgSize.cy;
			if (m_videoQueue.GetAllocSize() == 0)
			{
				INT count = MAX_VIDEO_QUEUE_SIZE / sampleTag.size + 1;
				m_videoQueue.Initialize(count, sampleTag.size + 4);
			}
			sampleTag.bits = static_cast<BYTE*>(m_videoQueue.Alloc());
			memcpy_s(sampleTag.bits + 4, sampleTag.size, bits, sampleTag.size);
			m_copy2D.Unmap(m_dx11);
		}
		m_timeQPC.EndTime();
		return static_cast<DWORD>(m_timeQPC.GetMillisconds());
	}
	BOOL MShadowController::Submit()
	{
		m_bBreak = FALSE;
		return TinyTaskBase::Submit(BindCallback(&MShadowController::OnMessagePump, this));
	}
	BOOL MShadowController::Close(DWORD dwMS)
	{
		m_bBreak = TRUE;
		if (TinyTaskBase::Close(dwMS))
		{
			m_image2D.Destory();
			m_copy2D.Destory();
			m_videoQueue.RemoveAll();
			return TRUE;
		}
		return FALSE;

	}
}