#pragma once
#include "../Common/TinyCollection.h"
#include "../Common/TinyCore.h"
#include "../Common/TinyCallback.h"
#include "../Common/TinyEvent.h"
#include <string>
#include <algorithm>
using namespace std;

#ifndef IS_POWER_OF_2
#define IS_POWER_OF_2(x)   ((x) != 0 && (((x) & ((x) - 1)) == 0))
#endif 
namespace TinyUI
{
	namespace IO
	{
		/// <summary>
		/// 无锁循环队列,单生产者单消费者,大小必须是2的整数幂,
		/// </summary>
		class TinyRingQueue
		{
			typedef struct tagFIFO
			{
				CHAR*	data;
				UINT	size;
				UINT	offsetI;
				UINT	offsetO;
			}FIFO;
		public:
			TinyRingQueue();
			~TinyRingQueue();
			BOOL	Initialize(UINT size);
			UINT	GetAvailable(BOOL bIN);
			void	Reset();
			UINT	Read(CHAR *data, UINT size);
			UINT	Write(CHAR *data, UINT size);
		private:
			UINT	ReadBytes(CHAR *data, UINT size);
			UINT	WriteBytes(CHAR *data, UINT size);
		protected:
			FIFO					m_io;
			TinyScopedArray<CHAR>	m_data;
		};
	};
}

