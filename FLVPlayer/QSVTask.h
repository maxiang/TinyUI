#pragma once
#include "Media/TinySoundPlayer.h"
#include "IO/TinyTaskBase.h"
#include "Common/TinyTime.h"
#include "FLVReader.h"
#include "PacketQueue.h"
#include "QSV.h"
#include "x264Decode.h"
#include "DX11Graphics2D.h"

using namespace Decode;
using namespace TinyUI;
using namespace TinyUI::IO;
using namespace TinyUI::Media;
using namespace DXFramework;

namespace FLVPlayer
{
	class QSVTask : public TinyTaskBase
	{
	public:
		QSVTask();
		~QSVTask();
	public:
		BOOL	Initialize(HWND hWND);
		BOOL	Submit();
		BOOL	Close(DWORD dwMs) OVERRIDE;
	private:
		void		OnMessagePump();
		mfxStatus	OnMessagePump1();
		mfxStatus	OnMessagePump2();
		mfxStatus	OnInvoke();
	private:
		FILE*							m_hFile;
		FLVReader						m_reader;
		DX11Graphics2D					m_graphics;
		DX11Image2D						m_video2D;
		QSV								m_qsv;
		x264Decode						m_x264;
		AVFrame*						m_pNV12;
		TinyScopedPtr<BYTE>				m_bits;
		TinyBufferArray<BYTE>			m_buffer;
	};
}


