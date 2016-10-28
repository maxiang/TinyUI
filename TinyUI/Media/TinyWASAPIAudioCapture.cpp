#include "../stdafx.h"
#include "TinyWASAPIAudioCapture.h"

namespace TinyUI
{
	namespace Media
	{
		TinyWASAPIAudioCapture::TinyWASAPIAudioCapture(DWORD dwFlag)
			:m_dwFlag(dwFlag),
			m_bufferFrameIndex(0),
			m_bufferSize(0)
		{
			m_audioSamplesReady.CreateEvent(FALSE, FALSE, NULL, NULL);
			m_audioStop.CreateEvent(FALSE, FALSE, NULL, NULL);
		}

		TinyWASAPIAudioCapture::~TinyWASAPIAudioCapture()
		{

		}

		BOOL TinyWASAPIAudioCapture::Open()
		{
			TinyComPtr<IMMDeviceEnumerator> enumerator;
			HRESULT hRes = enumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER);
			if (FAILED(hRes))
				return FALSE;
			hRes = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_mmDevice);
			if (FAILED(hRes))
				return FALSE;
			DWORD state = DEVICE_STATE_DISABLED;
			hRes = m_mmDevice->GetState(&state);
			if (FAILED(hRes))
				return FALSE;
			if (!(state & DEVICE_STATE_ACTIVE))
				return FALSE;
			hRes = m_mmDevice->Activate(__uuidof(IAudioClient), CLSCTX_INPROC_SERVER, NULL, (void**)&m_audioClient);
			if (FAILED(hRes))
				return FALSE;
			WAVEFORMATEX* pWFX = NULL;
			hRes = m_audioClient->GetMixFormat(&pWFX);
			if (FAILED(hRes))
				return FALSE;
			hRes = m_audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, m_dwFlag, 0, 0, pWFX, NULL);
			if (FAILED(hRes))
				return FALSE;
			hRes = m_audioClient->GetBufferSize(&m_bufferSize);
			if (FAILED(hRes))
				return FALSE;
			hRes = m_audioClient->GetDevicePeriod(&m_defaultDevicePeriod, &m_minimumDevicePeriod);
			if (FAILED(hRes))
				return FALSE;
			hRes = m_audioClient->GetStreamLatency(&m_latency);
			if (FAILED(hRes))
				return FALSE;
			if (m_dwFlag & AUDCLNT_STREAMFLAGS_LOOPBACK)
			{
				hRes = m_mmDevice->Activate(__uuidof(IAudioClient), CLSCTX_INPROC_SERVER, NULL, (void**)&m_lbClient);
				if (FAILED(hRes))
					return FALSE;
				hRes = m_lbClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST, 0, 0, pWFX, NULL);
				if (FAILED(hRes))
					return FALSE;
				hRes = m_lbClient->SetEventHandle(m_audioSamplesReady);
				if (FAILED(hRes))
					return FALSE;
			}
			if (m_dwFlag & AUDCLNT_STREAMFLAGS_EVENTCALLBACK)
			{
				hRes = m_audioClient->SetEventHandle(m_audioSamplesReady);
				if (FAILED(hRes))
					return FALSE;
			}
			hRes = m_audioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&m_audioCapture);
			if (FAILED(hRes))
				return FALSE;
			hRes = m_audioClient->GetService(__uuidof(IAudioClock), (void**)&m_audioClock);
			if (FAILED(hRes))
				return FALSE;
			hRes = m_audioClient->GetService(__uuidof(ISimpleAudioVolume), (void**)&m_audioVolume);
			if (FAILED(hRes))
				return FALSE;
			if (pWFX)
			{
				CoTaskMemFree(pWFX);
				pWFX = NULL;
			}
			return TRUE;
		}

		BOOL TinyWASAPIAudioCapture::Start()
		{
			m_audioStop.ResetEvent();
			m_task.Close(INFINITE);
			Closure s = BindCallback(&TinyWASAPIAudioCapture::OnMessagePump, this);
			if (!m_task.Submit(s))
				return FALSE;
			HRESULT hRes = m_audioClient->Start();
			if (FAILED(hRes))
				return FALSE;
			if (m_lbClient)
			{
				hRes = m_lbClient->Start();
				if (FAILED(hRes))
					return FALSE;
			}
			return TRUE;
		}

		void TinyWASAPIAudioCapture::OnMessagePump()
		{
			DWORD dwTaskIndex = 0;
			HANDLE hMM = AvSetMmThreadCharacteristics("Audio", &dwTaskIndex);
			ASSERT(hMM);
			BOOL error = FALSE;
			BOOL recording = TRUE;
			HANDLE waits[2] = { m_audioStop ,m_audioSamplesReady };
			while (recording && !error)
			{
				DWORD dwRes = WaitForMultipleObjects(2, waits, FALSE, INFINITE);
				switch (dwRes)
				{
				case WAIT_FAILED:
					error = TRUE;
					break;
				case WAIT_OBJECT_0 + 0:
					recording = FALSE;
					break;
				case WAIT_OBJECT_0 + 1:
				{
					BYTE* pData = NULL;
					UINT32 numFramesToRead = 0;
					DWORD dwFlags = 0;
					UINT64 devicePosition = 0;
					UINT64 qpcPosition = 0;
					dwRes = m_audioCapture->GetBuffer(&pData, &numFramesToRead, &dwFlags, &devicePosition, &qpcPosition);
					if (FAILED(dwRes))
						continue;
					if (m_audioClock)
					{
						m_audioClock->GetPosition(&devicePosition, &qpcPosition);
					}
					if (numFramesToRead != 0)
					{
						size_t pos = m_bufferFrameIndex * m_wfx.nBlockAlign;
						size_t num_bytes = numFramesToRead * m_wfx.nBlockAlign;
						if (dwFlags & AUDCLNT_BUFFERFLAGS_SILENT)
						{
							//memset(&capture_buffer[pos], 0, num_bytes);
						}
						else
						{
							//memcpy(&capture_buffer[pos], data_ptr, num_bytes);
						}
						m_bufferFrameIndex += numFramesToRead;
					}
					m_audioCapture->ReleaseBuffer(numFramesToRead);
				}
				break;
				default:
					error = TRUE;
					break;
				}
			}
			AvRevertMmThreadCharacteristics(hMM);
		}

		BOOL TinyWASAPIAudioCapture::Stop()
		{
			m_audioStop.SetEvent();
			HRESULT hRes = m_audioClient->Stop();
			if (FAILED(hRes))
				return FALSE;
			return TRUE;
		}

		BOOL TinyWASAPIAudioCapture::SetVolume(double volume)
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		BOOL TinyWASAPIAudioCapture::GetVolume(double* volume)
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		BOOL TinyWASAPIAudioCapture::Close()
		{
			throw std::logic_error("The method or operation is not implemented.");
		}
	}
}
