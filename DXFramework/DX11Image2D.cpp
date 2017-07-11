#include "stdafx.h"
#include "DX11Image2D.h"
#include <algorithm>
#include <limits>
#pragma comment(lib,"gdiplus.lib")

namespace DXFramework
{
	IMPLEMENT_DYNAMIC(DX11Image2D, DX11Element2D);

	DX11Image2D::DX11Image2D()
	{
	}
	DX11Image2D::~DX11Image2D()
	{

	}
	BOOL DX11Image2D::Create(DX11& dx11, ID3D11Texture2D* texture2D)
	{
		if (!Initialize(dx11))
			return FALSE;
		if (m_texture.Create(dx11, texture2D))
		{
			SetSize(m_texture.GetSize());
			SetScale(m_size);
			return TRUE;
		}
		return FALSE;
	}
	BOOL DX11Image2D::Create(DX11& dx11, const TinySize& size, BYTE* bits, BOOL bReadonly)
	{
		if (!Initialize(dx11))
			return FALSE;
		if (m_texture.Create(dx11, size.cx, size.cy, bits, bReadonly))
		{
			SetSize(m_texture.GetSize());
			SetScale(m_size);
			return TRUE;
		}
		return FALSE;
	}
	BOOL DX11Image2D::Create(DX11& dx11, const TinySize& size, BOOL bShared)
	{
		if (!Initialize(dx11))
			return FALSE;
		if (m_texture.Create(dx11, size.cx, size.cy, bShared))
		{
			SetSize(m_texture.GetSize());
			SetScale(m_size);
			return TRUE;
		}
		return FALSE;
	}
	void DX11Image2D::Destory()
	{
		m_vertexBuffer.Release();
		m_indexBuffer.Release();
		m_texture.Destory();
	}
	BOOL DX11Image2D::BitBlt(DX11& dx11, const BYTE* bits, LONG size, LONG linesize)
	{
		if (!bits || m_texture.IsEmpty() || size != (m_size.cx * m_size.cy * 4))
			return FALSE;
		HDC hDC = NULL;
		if (!m_texture.GetDC(FALSE, hDC))
			return FALSE;
		LONG _linesize = m_size.cx * 4;
		BITMAPINFO bmi = { 0 };
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = m_size.cx;
		bmi.bmiHeader.biHeight = m_size.cy;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = _linesize * m_size.cy;
		BYTE* pBits = NULL;
		HBITMAP hBitmap = ::CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&pBits), NULL, 0);
		INT rowcopy = (_linesize < linesize) ? _linesize : linesize;
		if (linesize == rowcopy)
		{
			memcpy(pBits, bits, rowcopy * m_size.cy);
		}
		else
		{
			for (INT y = 0; y < m_size.cy; y++)
			{
				memcpy(pBits + (UINT)y * _linesize, bits + (UINT)y * linesize, rowcopy);
			}
		}
		TinyUI::TinyMemDC mdc(hDC, hBitmap);
		::BitBlt(hDC, 0, 0, m_size.cx, m_size.cy, mdc, 0, 0, SRCCOPY);
		SAFE_DELETE_OBJECT(hBitmap);
		return m_texture.ReleaseDC();
	}

	BOOL DX11Image2D::BitBlt(DX11& dx11, const TinyRectangle& dst, HBITMAP hBitmapSrc, const TinyPoint& src)
	{
		if (m_texture.IsEmpty())
			return FALSE;
		HDC hDC = NULL;
		if (!m_texture.GetDC(TRUE, hDC))
			return FALSE;
		TinyUI::TinyMemDC mdc(hDC, hBitmapSrc);
		::BitBlt(hDC, dst.left, dst.top, dst.Width(), dst.Height(), mdc, src.x, src.y, SRCCOPY | CAPTUREBLT);
		return m_texture.ReleaseDC();
	}
	BOOL DX11Image2D::BitBlt(DX11& dx11, const TinyRectangle& dst, HDC hDCSrc, const TinyPoint& src)
	{
		if (m_texture.IsEmpty())
			return FALSE;
		HDC hDC = NULL;
		if (!m_texture.GetDC(TRUE, hDC))
			return FALSE;
		::BitBlt(hDC, dst.left, dst.top, dst.Width(), dst.Height(), hDCSrc, src.x, src.y, SRCCOPY | CAPTUREBLT);
		return m_texture.ReleaseDC();
	}
	BOOL DX11Image2D::Copy(DX11& dx11, ID3D11Texture2D* texture2D)
	{
		if (!texture2D)
			return FALSE;
		return m_texture.Copy(dx11, texture2D);
	}
	BOOL DX11Image2D::Copy(DX11& dx11, DX11Texture2D* texture2D)
	{
		if (!texture2D)
			return FALSE;
		return Copy(dx11, texture2D->GetTexture2D());
	}

	BOOL DX11Image2D::Copy(DX11& dx11, DX11Image2D* image2D)
	{
		if (!image2D)
			return FALSE;
		return Copy(dx11, image2D->GetTexture2D());
	}

	BOOL DX11Image2D::Map(DX11& dx11, BYTE *&lpData, UINT &pitch)
	{
		return m_texture.Map(dx11, lpData, pitch);
	}
	void DX11Image2D::Unmap(DX11& dx11)
	{
		m_texture.Unmap(dx11);
	}
	BOOL DX11Image2D::Copy(DX11& dx11, D3D11_BOX* ps, const BYTE* bits, LONG size)
	{
		if (m_texture.IsEmpty() || !bits || size <= 0)
			return FALSE;
		return m_texture.Copy(dx11, ps, bits, size, m_size.cx * 4, m_size.cx * m_size.cy * 4);
	}
	BOOL DX11Image2D::Copy(DX11& dx11, const BYTE* bits, LONG size, UINT stride)
	{
		if (m_texture.IsEmpty() || !bits || size <= 0)
			return FALSE;
		BYTE* bitso = NULL;
		UINT linesize = 0;
		UINT rowcopy = (stride < linesize) ? stride : linesize;
		if (m_texture.Map(dx11, bitso, linesize))
		{
			UINT rowcopy = (stride < linesize) ? stride : linesize;
			if (linesize == rowcopy)
			{
				memcpy(bitso, bits, linesize * m_size.cy);
			}
			else
			{
				for (INT y = 0; y < m_size.cy; y++)
				{
					memcpy(bitso + (UINT)y * linesize, bits + (UINT)y * stride, rowcopy);
				}
			}
			m_texture.Unmap(dx11);
			return TRUE;
		}
		return FALSE;
	}
	BOOL DX11Image2D::SaveAs(DX11& dx11, const CHAR* pzName)
	{
		return m_texture.SaveAs(dx11, pzName, JPG);
	}
	BOOL DX11Image2D::GetDC(BOOL discard, HDC& hDC)
	{
		return m_texture.GetDC(discard, hDC);
	}
	BOOL DX11Image2D::ReleaseDC()
	{
		return m_texture.ReleaseDC();
	}
	BOOL DX11Image2D::Load(DX11& dx11, HANDLE hResource)
	{
		if (!hResource)
			return FALSE;
		if (!Initialize(dx11))
			return FALSE;
		if (m_texture.Load(dx11, hResource))
		{
			SetSize(m_texture.GetSize());
			TinyRectangle s = { 0 };
			GetClientRect(dx11.GetHWND(), &s);
			SetScale(s.Size());
			return TRUE;
		}
		return FALSE;
	}
	BOOL DX11Image2D::Load(DX11& dx11, const CHAR* pzFile)
	{
		if (!pzFile)
			return FALSE;
		if (!Initialize(dx11))
			return FALSE;
		if (m_texture.Load(dx11, pzFile))
		{
			SetSize(m_texture.GetSize());
			SetScale(m_size);
			return TRUE;
		}
		return FALSE;
	}
	BOOL DX11Image2D::Load(DX11& dx11, const BYTE* bits, DWORD dwSize)
	{
		if (!Initialize(dx11))
			return FALSE;
		if (m_texture.Load(dx11, bits, dwSize))
		{
			SetSize(m_texture.GetSize());
			SetScale(m_size);
			return TRUE;
		}
		return FALSE;
	}
	BOOL DX11Image2D::Initialize(DX11& dx11)
	{
		D3D11_BUFFER_DESC	vertexBufferDesc = { 0 };
		D3D11_BUFFER_DESC	indexBufferDesc = { 0 };
		D3D11_SUBRESOURCE_DATA	vertexData = { 0 };
		D3D11_SUBRESOURCE_DATA	indexData = { 0 };
		INT vertexCount = GetIndexCount();
		INT indexCount = GetIndexCount();
		TinyScopedArray<VERTEXTYPE> vertices(new VERTEXTYPE[vertexCount]);
		TinyScopedArray<ULONG> indices(new ULONG[indexCount]);
		ZeroMemory(vertices.Ptr(), (sizeof(VERTEXTYPE) * vertexCount));
		for (INT i = 0; i < indexCount; i++)
		{
			indices[i] = i;
		}
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(VERTEXTYPE) * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;
		vertexData.pSysMem = vertices.Ptr();
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
		HRESULT hRes = dx11.GetD3D()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (hRes != S_OK)
			return FALSE;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(ULONG) * indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexData.pSysMem = indices.Ptr();
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;
		hRes = dx11.GetD3D()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (hRes != S_OK)
			return FALSE;
		m_vertices.Reset(new VERTEXTYPE[vertexCount]);
		return TRUE;
	}
	BOOL DX11Image2D::Update(DX11& dx11, FLOAT ratioX, FLOAT ratioY)
	{
		FLOAT left = 0.0F;
		FLOAT right = 0.0F;
		FLOAT top = 0.0F;
		FLOAT bottom = 0.0F;
		D3D11_VIEWPORT vp;
		ZeroMemory(&vp, sizeof(vp));
		UINT count = 1;
		dx11.GetImmediateContext()->RSGetViewports(&count, &vp);
		XMFLOAT2 scale(static_cast<FLOAT>(GetScale().cx) * ratioX, static_cast<FLOAT>(GetScale().cy) * ratioY);
		XMFLOAT2 pos(static_cast<FLOAT>(GetPosition().x) * ratioX, static_cast<FLOAT>(GetPosition().y) * ratioY);
		XMFLOAT2 size(vp.Width, vp.Height);
		left = (FLOAT)((size.x / 2) * -1) + pos.x;
		right = left + scale.x;
		top = (FLOAT)(size.y / 2) - pos.y;
		bottom = top - scale.y;
		INT vertexCount = GetIndexCount();
		m_vertices[0].position = XMFLOAT3(left, top, 0.0F);
		m_vertices[0].texture = XMFLOAT2(0.0F, 0.0F);
		m_vertices[1].position = XMFLOAT3(right, bottom, 0.0F);
		m_vertices[1].texture = XMFLOAT2(1.0F, 1.0f);
		m_vertices[2].position = XMFLOAT3(left, bottom, 0.0F);
		m_vertices[2].texture = XMFLOAT2(0.0F, 1.0f);
		m_vertices[3].position = XMFLOAT3(left, top, 0.0F);
		m_vertices[3].texture = XMFLOAT2(0.0F, 0.0F);
		m_vertices[4].position = XMFLOAT3(right, top, 0.0F);
		m_vertices[4].texture = XMFLOAT2(1.0F, 0.0F);
		m_vertices[5].position = XMFLOAT3(right, bottom, 0.0F);
		m_vertices[5].texture = XMFLOAT2(1.0F, 1.0F);
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hRes = dx11.GetImmediateContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (hRes != S_OK)
			return FALSE;
		memcpy(mappedResource.pData, (void*)m_vertices.Ptr(), sizeof(VERTEXTYPE) * vertexCount);
		dx11.GetImmediateContext()->Unmap(m_vertexBuffer, 0);
		return TRUE;
	}

	BOOL DX11Image2D::Allocate(DX11& dx11)
	{
		return TRUE;
	}

	void DX11Image2D::Deallocate(DX11& dx11)
	{

	}

	DX11Texture2D* DX11Image2D::GetTexture2D()
	{
		return &m_texture;
	}
	BOOL DX11Image2D::IsEmpty() const
	{
		return m_texture.IsEmpty();
	}
	BOOL DX11Image2D::Process(DX11& dx11)
	{
		UINT stride = sizeof(VERTEXTYPE);
		UINT offset = 0;
		dx11.GetImmediateContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
		dx11.GetImmediateContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		dx11.GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return TRUE;
	}

	INT DX11Image2D::GetIndexCount() const
	{
		return 6;
	}
}