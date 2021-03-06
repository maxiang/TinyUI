#pragma once
#include "DXFramework.h"
#include "DX11.h"
#include "WICTexture.h"

namespace DXFramework
{
#ifndef WIDTHBYTES
#define WIDTHBYTES(i)           ((unsigned)((i+31)&(~31))/8) 
#endif
	/// <summary>
	/// 2D纹理
	/// </summary>
	class DX11Texture2D
	{
		DISALLOW_COPY_AND_ASSIGN(DX11Texture2D)
	public:
		DX11Texture2D();
		DX11Texture2D(ID3D11Texture2D* texture2D);
		virtual ~DX11Texture2D();
		virtual BOOL Create(DX11& dx11, ID3D11Texture2D* texture2D);
		virtual BOOL Create(DX11& dx11, INT cx, INT cy, const BYTE* bits, BOOL bReadoly);
		virtual BOOL Create(DX11& dx11, INT cx, INT cy, BOOL bShared);
		virtual BOOL SaveAs(DX11& dx11, const CHAR* pzFile, D3DX11_IMAGE_FILE_FORMAT dxgi);
		virtual BOOL Load(DX11& dx11, const BYTE* bits, DWORD dwSize);
		virtual BOOL Load(DX11& dx11, HANDLE hResource);
		virtual BOOL Load(DX11& dx11, const CHAR* pzFile);
		virtual BOOL Copy(DX11& dx11, ID3D11Texture2D* texture2D);
		virtual BOOL Copy(DX11& dx11, DX11Texture2D& texture2D);
		virtual BOOL Copy(DX11& dx11, D3D11_BOX* ps, const BYTE* bits, LONG size, UINT rowPitch, UINT depthPitch);
		virtual void Destory();
	public:
		BOOL GetDC(BOOL discard, HDC& hDC);
		BOOL ReleaseDC();
		BOOL Map(DX11& dx11, BYTE *&lpData, UINT &pitch, BOOL bReadoly = FALSE);
		void Unmap(DX11& dx11);
		HANDLE	GetHandle() const;
		ID3D11Texture2D* GetTexture2D() const;
		ID3D11ShaderResourceView* GetSRView() const;
		BOOL IsEmpty() const;
		TinySize GetSize();
	protected:
		HANDLE									m_handle;
		TinyComPtr<IDXGISurface1>				m_surface;
		TinyComPtr<ID3D11Texture2D>				m_texture2D;
		TinyComPtr<ID3D11ShaderResourceView>	m_resourceView;
		TinyComPtr<IDXGIKeyedMutex>				m_keyedMutex;
	};

#define CAPTURETYPE_MEMORYTEXTURE   1
#define CAPTURETYPE_SHAREDTEXTURE   2

#pragma pack(push, 8)
	typedef struct tagSharedCaptureDATA
	{
		UINT		CaptureType;
		DWORD		Format;
		SIZE		Size;
		BOOL		bFlip;
		BOOL		bMultisample;
		UINT		Pitch;
		DWORD		MapSize;
		HWND		HwndCapture;
	}SharedCaptureDATA;
	/// <summary>
	/// 共享纹理数据
	/// </summary>
	typedef struct tagSharedTextureDATA
	{
		LONGLONG    FrameTime;
		HANDLE      TextureHandle;
		DWORD       Texture1Offset;
		DWORD		Texture2Offset;
		DWORD		CurrentID;
	}SharedTextureDATA;
#pragma pack(pop)
}

