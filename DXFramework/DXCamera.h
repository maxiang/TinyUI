#pragma once
#include "DXFramework.h"

namespace DXFramework
{
	class DXCamera
	{
	public:
		DXCamera();
		~DXCamera();
		void SetPosition(FLOAT, FLOAT, FLOAT);
		void SetRotation(FLOAT, FLOAT, FLOAT);
		XMFLOAT3 GetPosition();
		XMFLOAT3 GetRotation();
		void UpdatePosition();
		XMMATRIX GetViewMatrix();
	private:
		FLOAT m_positionX;
		FLOAT m_positionY;
		FLOAT m_positionZ;
		FLOAT m_rotationX;
		FLOAT m_rotationY;
		FLOAT m_rotationZ;
		XMMATRIX m_viewMatrix;
	};
}