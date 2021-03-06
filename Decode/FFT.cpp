#include "stdafx.h"
#include "FFT.h"

namespace Decode
{
	FFT::FFT()
		:m_sampleSize(0),
		m_iterations(0)
	{

	}
	FFT::~FFT()
	{

	}
	BOOL FFT::Initialize(INT sampleSize)
	{
		m_sampleSize = sampleSize;
		m_iterations = (INT)(log((FLOAT)sampleSize) / log((FLOAT)2));
		INT iterations1 = m_iterations - 1;
		m_imags.Reset(new FLOAT[sampleSize]);
		if (!m_imags)
			return FALSE;
		m_reals.Reset(new FLOAT[sampleSize]);
		if (!m_reals)
			return FALSE;
		INT ss2 = sampleSize >> 1;
		m_mags.Reset(new FLOAT[ss2]);
		if (!m_mags)
			return FALSE;
		m_bitReverses.Reset(new INT[sampleSize]);
		if (!m_bitReverses)
			return FALSE;
		m_sintable.Reset(new FLOAT[ss2 * m_iterations]);
		if (!m_sintable)
			return FALSE;
		m_costable.Reset(new FLOAT[ss2 * m_iterations]);
		if (!m_costable)
			return FALSE;
		INT k = 0;
		INT x = 0;
		for (INT i = 1; i <= m_iterations; i++)
		{
			while (k < m_sampleSize)
			{
				for (INT j = 1; j <= ss2; j++)
				{
					FLOAT p = (FLOAT)ReverseBits(k >> iterations1, m_iterations);
					FLOAT arg = (6.283185F * p) / (FLOAT)m_sampleSize;
					m_sintable[x] = (FLOAT)sin(arg);
					m_costable[x] = (FLOAT)cos(arg);
					k++;
					x++;
				}
				k += ss2;
			}
			k = 0;
			iterations1--;
			ss2 >>= 1;
		}
		for (k = 0; k < m_sampleSize; k++)
		{
			m_bitReverses[k] = ReverseBits(k, m_iterations);
		}
		return TRUE;
	}
	FLOAT* FFT::Calculate(FLOAT* pSample, INT sampleSize)
	{
		INT ss2 = m_sampleSize >> 1;
		INT iterations1 = m_iterations - 1;
		INT wAps = sampleSize / m_sampleSize;
		INT ss = 0;
		for (INT i = 0; ss < sampleSize; i++)
		{
			m_reals[i] = pSample[ss];
			m_imags[i] = 0.0F;
			ss += wAps;
		}
		INT x = 0;
		for (INT i = 1; i <= m_iterations; i++)
		{
			for (INT k = 0; k < m_sampleSize; k += ss2)
			{
				for (INT j = 1; j <= ss2; j++)
				{
					FLOAT c = m_costable[x];
					FLOAT s = m_sintable[x];
					INT kn2 = k + ss2;
					FLOAT tr = m_reals[kn2] * c + m_imags[kn2] * s;
					FLOAT ti = m_imags[kn2] * c - m_reals[kn2] * s;
					m_reals[kn2] = m_reals[k] - tr;
					m_imags[kn2] = m_imags[k] - ti;
					m_reals[k] += tr;
					m_imags[k] += ti;
					k++;
					x++;
				}
			}
			iterations1--;
			ss2 >>= 1;
		}
		for (INT k = 0; k < m_sampleSize; k++)
		{
			INT val = m_bitReverses[k];
			if (val > k)
			{
				FLOAT real = m_reals[k];
				FLOAT imag = m_imags[k];
				m_reals[k] = m_reals[val];
				m_imags[k] = m_imags[val];
				m_reals[val] = real;
				m_imags[val] = imag;
			}
		}
		m_mags[0] = (FLOAT)sqrt(m_reals[0] * m_reals[0] + m_imags[0] * m_imags[0]) / (FLOAT)m_sampleSize;
		ss2 = m_sampleSize >> 1;
		for (INT i = 1; i < ss2; i++)
		{
			m_mags[i] = (2.0F * (FLOAT)sqrt(m_reals[i] * m_reals[i] + m_imags[i] * m_imags[i])) / (FLOAT)m_sampleSize;
		}
		return m_mags;
	}
	INT	FFT::ReverseBits(INT index, INT size)
	{
		INT reverse = 0;
		for (INT i = 0; i < size; i++)
		{
			reverse = (reverse << 1) | (index & 1);
			index >>= 1;
		}
		return reverse;
	}
}