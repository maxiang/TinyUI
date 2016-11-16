#include "../stdafx.h"
#include "TinyNetwork.h"
#include <process.h>

namespace TinyUI
{
	namespace Network
	{
		IPAddress::IPAddress()
		{
			TRACE("IPAddress构造-0\n");
		}

		IPAddress::IPAddress(const std::vector<uint8_t>& address)
			: m_address(std::move(address))
		{
			TRACE("IPAddress构造-1\n");
		}

		IPAddress::IPAddress(const IPAddress& other)
			: m_address(std::move(other.m_address))
		{
			TRACE("IPAddress拷贝构造\n");
		}
		IPAddress::IPAddress(IPAddress&& other)
			: m_address(std::move(other.m_address))
		{
			TRACE("IPAddress移动构造\n");
		}

		IPAddress::IPAddress(const BYTE* address, DWORD size)
			: m_address(address, address + size)
		{
		}

		IPAddress::IPAddress(BYTE b0, BYTE b1, BYTE b2, BYTE b3)
		{
			m_address.reserve(4);
			m_address.push_back(b0);
			m_address.push_back(b1);
			m_address.push_back(b2);
			m_address.push_back(b3);
		}

		IPAddress::IPAddress(BYTE b0,
			BYTE b1,
			BYTE b2,
			BYTE b3,
			BYTE b4,
			BYTE b5,
			BYTE b6,
			BYTE b7,
			BYTE b8,
			BYTE b9,
			BYTE b10,
			BYTE b11,
			BYTE b12,
			BYTE b13,
			BYTE b14,
			BYTE b15)
		{
			const BYTE address[] = { b0, b1, b2,  b3,  b4,  b5,  b6,  b7, b8, b9, b10, b11, b12, b13, b14, b15 };
			m_address = std::vector<BYTE>(std::begin(address), std::end(address));
		}
		BOOL IPAddress::operator==(const IPAddress& other) const
		{
			return m_address == other.m_address;
		}

		BOOL IPAddress::operator!=(const IPAddress& other) const
		{
			return m_address != other.m_address;
		}

		BOOL IPAddress::operator<(const IPAddress& that) const
		{
			if (m_address.size() != that.m_address.size())
			{
				return m_address.size() < that.m_address.size();
			}
			return m_address < that.m_address;
		}

		IPAddress::~IPAddress()
		{
			TRACE("IPAddress析构\n");
		}
		BOOL IPAddress::IsIPv4() const
		{
			return m_address.size() == IPv4AddressSize;
		}

		BOOL IPAddress::IsIPv6() const
		{
			return m_address.size() == IPv6AddressSize;
		}

		BOOL IPAddress::IsValid() const
		{
			return IsIPv4() || IsIPv6();
		}
		BOOL IPAddress::IsZero() const
		{
			for (auto x : m_address)
			{
				if (x != 0)
				{
					return FALSE;
				}
			}
			return !IsEmpty();
		}
		BOOL IPAddress::IsEmpty() const
		{
			return m_address.empty();
		}

		std::string IPAddress::ToString() const
		{
			return "";
		}

		DWORD IPAddress::Size() const
		{
			return static_cast<DWORD>(m_address.size());
		}
		const std::vector<BYTE>& IPAddress::Address() const
		{
			return m_address;
		}
		//////////////////////////////////////////////////////////////////////////
		IPEndPoint::IPEndPoint()
			:m_port(0)
		{

		}
		IPEndPoint::~IPEndPoint()
		{

		}
		IPEndPoint::IPEndPoint(const IPAddress& address, USHORT port)
			:m_address(std::move(address)),
			m_port(port)
		{

		}
		IPEndPoint::IPEndPoint(const IPEndPoint& endpoint)
			: m_address(std::move(endpoint.m_address)),
			m_port(endpoint.m_port)
		{

		}
		IPEndPoint::IPEndPoint(IPEndPoint&& endpoint)
			: m_address(std::move(endpoint.m_address)),
			m_port(endpoint.m_port)
		{

		}
		const IPAddress& IPEndPoint::Address() const
		{
			return m_address;
		}
		USHORT IPEndPoint::Port() const
		{
			return m_port;
		}
	}
}