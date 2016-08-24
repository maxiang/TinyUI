#pragma once
#include "FilterBase.h"
#include "FilterObserver.h"
#include "FilterObserver.h"
#include "VideoCaptureParam.h"
#include "SinkFilter.h"
#include "ScopedMediaType.h"
using namespace std;
using namespace TinyUI;

namespace Media
{
	/// <summary>
	/// ��Ƶ����
	/// </summary>
	class VideoCapture : public FilterObserver
	{
	public:
		class Name
		{
		public:
			Name();
			Name(const string& name, const string& id);
			~Name();
			const string& name() const;
			const string& id() const;
		private:
			string	m_name;
			string	m_id;
		};
		void OnFrameReceive(const BYTE* data, INT size, void*) OVERRIDE;
	public:
		VideoCapture();
		virtual ~VideoCapture();
		BOOL Initialize(const Name& name, HWND hWND);
		void Uninitialize();
		BOOL Start();
		BOOL Stop();
		BOOL Pause();
		FILTER_STATE GetState() const;
		virtual BOOL Allocate(const VideoCaptureParam& param);
		virtual void DeAllocate();
	public:
		static BOOL GetDevices(vector<Name>& names);
		static BOOL GetDeviceParams(const VideoCapture::Name& device, vector<VideoCaptureParam>& formats);
		static BOOL GetDeviceFilter(const Name& name, IBaseFilter** filter);
		static BOOL GetMediaType(IPin* pPin, REFGUID subtype, AM_MEDIA_TYPE** ppType);
	private:
		static BOOL GetPinCategory(IPin* pin, REFGUID category);
		static TinyComPtr<IPin> GetPin(IBaseFilter* filter, PIN_DIRECTION pin_dir, REFGUID category);
		static VideoPixelFormat TranslateMediaSubtypeToPixelFormat(const GUID& subType);
		void SetAntiFlickerInCaptureFilter();
	private:
		HWND								m_hWND;
		VideoCaptureParam					m_vcp;
		TinyComPtr<IGraphBuilder>			m_builder;
		TinyComPtr<IMediaControl>			m_control;
		TinyComPtr<IBaseFilter>				m_captureFilter;
		TinyComPtr<IPin>					m_captureO;
		TinyComPtr<IBaseFilter>				m_mjpgFilter;
		TinyComPtr<IPin>					m_mjpgO;//OUT
		TinyComPtr<IPin>					m_mjpgI;//IN
		TinyComPtr<IBaseFilter>				m_avFilter;
		TinyComPtr<IPin>					m_avO;//OUT
		TinyComPtr<IPin>					m_avI;//IN
		TinyComPtr<IPin>					m_sinkI;
		TinyScopedReferencePtr<SinkFilter>	m_sinkFilter;
	};
}
