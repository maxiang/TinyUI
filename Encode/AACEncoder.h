#pragma once
#include "Common/TinyCommon.h"
#include "Common/TinyCallback.h"
#include "Common/TinyEvent.h"
#include "Render/TinyGDI.h"
#include "Media/TinyMedia.h"
extern "C"
{
#include "faac.h"
#include "faaccfg.h"
}
#pragma comment(lib,"libfaac.lib")
using namespace TinyUI;
using namespace TinyUI::Media;

namespace Encode
{
#define  AAC_TIMEBASE 1024
#define  AAC_TIMEDEN  1000
	class AACEncoder
	{
		DISALLOW_COPY_AND_ASSIGN(AACEncoder)
	public:
		AACEncoder(Callback<void(BYTE*, LONG, const MediaTag&)>&& callback);
		virtual ~AACEncoder();
	public:
		BOOL	Open(const WAVEFORMATEX& waveFMT, INT audioRate = 128);
		BOOL	Encode(BYTE* bits, LONG size);
		void	Close();
		BOOL	GetSpecificInfo(vector<BYTE>& info);
		DWORD	GetOutputBytes() const;
		DWORD	GetFPS() const;
	private:
		faacEncHandle			m_aac;
		faacEncConfigurationPtr	m_config;
		DWORD					m_inputSamples;
		DWORD					m_maxOutputBytes;
		DWORD					m_dwINC;
		DWORD					m_dwPTS;
		WAVEFORMATEX			m_waveFMT;
		TinyScopedArray<BYTE>	m_bits;
		Callback<void(BYTE*, LONG, const MediaTag&)> m_callback;
	};
}
