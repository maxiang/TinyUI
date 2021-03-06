#pragma once
#include "MShowCommon.h"
#include "Media/TinyWave.h"
#include "Media/TinyAudioDSPCapture.h"
#include "Media/TinyMFResampler.h"
#include "Common/TinyTime.h"
#include "Common/TinyCore.h"
using namespace TinyUI;
using namespace TinyUI::Media;

namespace MShow
{
	class MAudioDSP
	{
		DISALLOW_COPY_AND_ASSIGN(MAudioDSP)
	public:
		MAudioDSP();
		~MAudioDSP();
		BOOL Initialize(Callback<void(BYTE*, LONG)>&& callback);
		BOOL Open();
		BOOL Open(const TinyWASAPIAudio::Name& capture, const TinyWASAPIAudio::Name& speaker);
		BOOL Open(const GUID& capture, const GUID& speaker);
		BOOL Start();
		BOOL Stop();
		BOOL Close();
	private:
		void OnDSP(BYTE* bits, LONG size, LPVOID lpParameter);
		void OnAudio(BYTE* bits, LONG size, LPVOID lpParameter);
		void OnTimer();
	private:
		BYTE						m_bits[4096];
		TinyLock					m_lock;
		TinyAudioDSPCapture			m_audioDSP;
		TinyMFResampler				m_resampler;
		TinyBufferArray<BYTE>		m_buffer;
		TinyTimer					m_timer;
		TinyWaveFile				m_waveFile;
		WAVEFORMATEX				m_waveFMTI;
		WAVEFORMATEX				m_waveFMTO;
		Callback<void(BYTE*, LONG)> m_callback;
	};
}

