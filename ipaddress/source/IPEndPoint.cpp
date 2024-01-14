#include "IPEndPoint.h"

namespace ip_address
{
	IPEndPoint::IPEndPoint(const sockaddr* addr, socklen_t addrLength)
	{
		assert(addr != nullptr);
		assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);
		if (addr->sa_family == AF_INET)
		{
			const auto* addr4 = reinterpret_cast<const sockaddr_in*>(&addr);
			memcpy(&this->mAddr.mIpAddress4.mAddr4.mBytes[0], &addr4->sin_addr, sizeof(in_addr));
			this->mPort = ntohs(addr4->sin_port);
			this->mVersion = IPVersion::kIPv4;
		}
		else if (addr->sa_family == AF_INET6)
		{
			const auto* addr6 = reinterpret_cast<const sockaddr_in6*>(&addr);
			memcpy(&this->mAddr.mIpAddress6.mAddr6.mBytes[0], &addr6->sin6_addr, sizeof(in6_addr));
			this->mPort = ntohs(addr6->sin6_port);
			this->mVersion = IPVersion::kIPv6;
		}
	}

	bool IPEndPoint::operator==(const IPEndPoint& rhs) const
	{
		if (this->mPort != rhs.mPort)
			return false;
		if (static_cast<IPAddress>(*this) != static_cast<IPAddress>(rhs))
			return false;
		return true;
	}

	bool IPEndPoint::operator==(const port_host_byte_order_t rhs) const
	{
		return this->mPort == rhs;
	}

	bool IPEndPoint::operator==(const port_network_byte_order_t rhs) const
	{
		return this->mPort == to_host_byte_order(rhs);
	}

	bool IPEndPoint::operator!=(const IPEndPoint& rhs) const
	{
		return !this->operator==(rhs);
	}

	bool IPEndPoint::operator!=(const port_host_byte_order_t rhs) const
	{
		return !this->operator==(rhs);
	}

	bool IPEndPoint::operator!=(const port_network_byte_order_t rhs) const
	{
		return !this->operator==(rhs);
	}

	IPEndPoint& IPEndPoint::operator=(const IPEndPoint& rhs)
	{
		this->mPort = rhs.mPort;
		std::copy(this->mAddr.mIpAddress6.mAddr6.mBytes.begin(), this->mAddr.mIpAddress6.mAddr6.mBytes.end(),
		          rhs.mAddr.mIpAddress6.mAddr6.mBytes.begin());
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(const IPAddress& rhs)
	{
		this->ipAddress() = rhs;
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(const IPAddressV4& rhs)
	{
		this->mAddr.mIpAddress4 = rhs;
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(const IPAddressV6& rhs)
	{
		this->mAddr.mIpAddress6 = rhs;
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(const port_host_byte_order_t& rhs)
	{
		this->mPort = rhs;
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(const port_network_byte_order_t& rhs)
	{
		this->mPort = to_host_byte_order(rhs);
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(IPEndPoint&& rhs) noexcept
	{
		*this = rhs;
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(IPAddress&& rhs) noexcept
	{
		this->ipAddress() = rhs;
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(IPAddressV4&& rhs) noexcept
	{
		*this = IPAddress(rhs);
		return *this;
	}

	IPEndPoint& IPEndPoint::operator=(IPAddressV6&& rhs) noexcept
	{
		this->mAddr.mIpAddress6 = rhs;
		this->mVersion = IPVersion::kIPv6;
		return *this;
	}

	bool IPEndPoint::operator<(const IPEndPoint& lhs) const
	{
		//IPv4 first then IPv6 address.
		if (static_cast<int>(this->mVersion) != static_cast<int>(lhs.mVersion))
		{
			return static_cast<int>(this->mVersion) < static_cast<int>(lhs.mVersion);
		}
		if (this->mAddr.mIpAddress6.mAddr6.mBytes < lhs.mAddr.mIpAddress6.mAddr6.mBytes)
			return true;
		return this->mPort < lhs.mPort;
	}

	IPAddress& IPEndPoint::ipAddress() const
	{
		return const_cast<IPEndPoint&>(*this);
	}

	port_host_byte_order_t IPEndPoint::getPort() const
	{
		return mPort;
	}

	port_network_byte_order_t IPEndPoint::getPortNetworkByteOrder() const
	{
		return to_network_byte_order(mPort);
	}

	IPAddress IPEndPoint::getIPAddress() const
	{
		IPAddress ip(this->mAddr.mIpAddress6);
		ip.mVersion = this->getVersion();
		return ip;
	}

	sockaddr_in IPEndPoint::getAddressIPv4() const
	{
		assert(mVersion == IPVersion::kIPv4);
		sockaddr_in addrIn = {};
		memcpy(&addrIn.sin_addr, &mAddr.mIpAddress4, sizeof(mAddr.mIpAddress4));
		addrIn.sin_port = to_integer(to_network_byte_order(mPort)); // host to network-byte order
		addrIn.sin_family = AF_INET;
		return addrIn;
	}

	sockaddr_in6 IPEndPoint::getAddressIPv6() const
	{
		assert(mVersion == IPVersion::kIPv6 || mVersion == IPVersion::kIPv4);
		sockaddr_in6 addr6In = {};
		memcpy(&addr6In.sin6_addr, &mAddr.mIpAddress6, sizeof(mAddr.mIpAddress6));
		addr6In.sin6_port = to_integer(to_network_byte_order(mPort)); // host to network-byte order
		addr6In.sin6_family = AF_INET6;
		return addr6In;
	}

	std::string IPEndPoint::getStringWithPort() const
	{
		std::string str = getString();
		str.append(":" + std::to_string(mPort));
		return str;
	}


	void IPEndPoint::clear() noexcept
	{
		static_assert(sizeof(this->mAddr.mIpAddress6) > sizeof(this->mAddr.mIpAddress4)); //sanity check for memset
		memset(&mAddr.mIpAddress6.mAddr6.mBytes[0], 0x0, sizeof(mAddr.mIpAddress6));
		this->mVersion = IPVersion::kUnknown;
		this->mPort = DEFAULT_IP_ENDPOINT_PORT;
	}

	std::ostream& operator<<(std::ostream& rhs, const IPEndPoint& lhs)
	{
		return rhs << lhs.getStringWithPort();
	}
}
