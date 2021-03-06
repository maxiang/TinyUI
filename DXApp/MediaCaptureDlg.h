#pragma once
#include "Control/TinyCustomDialog.h"
#include "Control/TinyButton.h"
#include "Control/TinyComboBox.h"
#include "VideoCapture.h"
#include "VideoCaptureParam.h"
#include "AudioCapture.h"
#include "AudioCaptureParam.h"
using namespace TinyUI;

namespace DXApp
{
	class MediaCaptureDlg : public TinyCustomDialog
	{
		DECLARE_DYNAMIC(MediaCaptureDlg)
	public:
		MediaCaptureDlg();
		virtual	~MediaCaptureDlg();
	public:
		LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
		LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
	public:
		DShow::AudioCapture::Name*  GetAudioName();
		DShow::VideoCapture::Name*	GetVideoName();
		DShow::AudioCaptureParam*	GetAudioParam();
		DShow::VideoCaptureParam*	GetVideoParam();
	private:
		void OnVideoSelectChange(INT);
		void OnVideoCapSelectChange(INT);
		void OnAudioSelectChange(INT);
		void OnAudioCapSelectChange(INT);
		void GetVideoDevices();
		void GetAudioDevices();
		void OnRefreshClick(void*, INT);
	private:
		TinyButton	 m_button;
		TinyComboBox m_video;
		TinyComboBox m_videoCap;
		TinyComboBox m_audio;
		TinyComboBox m_audioCap;
		TinyScopedPtr<Delegate<void(void*, INT)>> m_onRefreshClick;
		TinyScopedPtr<Delegate<void(INT)>> m_onVideoSelectChange;
		TinyScopedPtr<Delegate<void(INT)>> m_onAudioSelectChange;
		TinyScopedPtr<Delegate<void(INT)>> m_onVideoCapSelectChange;
		TinyScopedPtr<Delegate<void(INT)>> m_onAudioCapSelectChange;
		vector<DShow::AudioCapture::Name> m_audioNames;
		vector<DShow::VideoCapture::Name> m_videoNames;
		vector<DShow::AudioCaptureParam> m_audioParams;
		vector<DShow::VideoCaptureParam> m_videoParams;
		DShow::AudioCapture::Name* m_audioName;
		DShow::VideoCapture::Name* m_videoName;
		DShow::AudioCaptureParam* m_audioParam;
		DShow::VideoCaptureParam* m_videoParam;
	};
}


