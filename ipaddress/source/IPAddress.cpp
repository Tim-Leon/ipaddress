#include "IPAddress.h"
#include <sstream>
#include "IPEndPoint.h"
namespace ip_address
{
	IPAddress::IPAddress(const char* ip)
	{
		if (!parseIPAddress(*this, ip))
		{
			throw std::runtime_error("invalid parser input");
		}
	}

	IPAddress::IPAddress(const std::string& ip)
	{
		if (!parseIPAddress(*this, ip))
		{
			throw std::runtime_error("invalid parser input");
		}
	}

	IPAddress::IPAddress(const ByteArray4&& addr4) noexcept : mVersion(IPVersion::kIPv4)
	{
		mAddr.mIpAddress4.mAddr4.mBytes = addr4;
	}

	IPAddress::IPAddress(const ByteArray4& addr4) noexcept : mVersion(IPVersion::kIPv4)
	{
		mAddr.mIpAddress4.mAddr4.mBytes = addr4;
	}

	IPAddress::IPAddress(const ByteArray16&& addr6) noexcept : mVersion(IPVersion::kIPv6)
	{
		mAddr.mIpAddress6.mAddr6.mBytes = addr6;
	}

	IPAddress::IPAddress(const ByteArray16& addr6) noexcept : mVersion(IPVersion::kIPv6)
	{
		mAddr.mIpAddress6.mAddr6.mBytes = addr6;
	}

	IPVersion IPAddress::getVersion() const noexcept
	{
		return mVersion;
	}

	bool IPAddress::isIPv6() const noexcept
	{
		return mVersion == IPVersion::kIPv6 ? true : false;
	}

	bool IPAddress::isIPv4() const noexcept
	{
		return mVersion == IPVersion::kIPv4 ? true : false;
	}

	IPAddressV6& IPAddress::asIPv6() const
	{
		assert(this->mVersion == IPVersion::kIPv6 && "Can not represent an ipv4 as an ipv6, use IPv4ToIPv6Map() instead");
		return mAddr.mIpAddress6;
	}

	IPAddressV4& IPAddress::asIPv4() const
	{
		assert(this->mVersion == IPVersion::kIPv4 && "Can not represnet an ipv6 as an ipv4, use IPv4ToIPv6Map() instead");
		return mAddr.mIpAddress4;
	}

	std::string IPAddress::getString() const
	{
		assert(this->mVersion != IPVersion::kUnknown);
		std::stringstream ss;
		if (this->isIPv4())
		{
			int mIndex = 0;
			for (auto& i : mAddr.mIpAddress4.mAddr4.mBytes)
			{
				ss << static_cast<int>(i);
				if (mIndex < 3)
					ss << '.';
				mIndex++;
			}
			return ss.str();
			//return std::string(addr.ipAddress4.addr.mBytes.begin(), addr.ipAddress4.addr.mBytes.end());
		}
		if (this->isIPv6())
		{
			int mIndex = 0;
			for (auto& i : mAddr.mIpAddress6.mAddr6.mBytes)
			{
				ss << static_cast<int>(i);
				if (mIndex < 8)
					ss << ':';
				mIndex++;
			}
			return ss.str();

			//return std::string(addr.ipAddress6.addr6.mBytes.begin(), addr.ipAddress6.addr6.mBytes.end());
		}
		throw std::runtime_error("invalid parser input");
	}

	bool IPAddress::isBroadcast() const noexcept
	{
		return isIPv4() && this->asIPv4().isBroadcast();
	}

	bool IPAddress::isWildcard() const noexcept
	{
		return this->isIPv4() && this->asIPv4().isWildcard() || this->isIPv6() && this->asIPv6().isWildcard();
	}

	bool IPAddress::parseIPAddress(IPAddress& addr, const std::string& ip) noexcept
	{
		if (IPAddressV4::parseIPAddressV4(addr.mAddr.mIpAddress4, ip))
		{
			addr.mVersion = IPVersion::kIPv4;
			return true;
		}
		else if (IPAddressV6::parseIPAddressV6(addr.mAddr.mIpAddress6, ip)) {
			addr.mVersion = IPVersion::kIPv6;
			return true;
		}
		return false;
	}

	void IPAddress::clear() noexcept
	{
		static_assert(sizeof(this->mAddr.mIpAddress6) > sizeof(this->mAddr.mIpAddress4.mAddr4));
		//safety check for memset
		memset(&mAddr.mIpAddress6.mAddr6.mBytes[0], 0x0, sizeof(mAddr.mIpAddress6));
		this->mVersion = IPVersion::kUnknown;
	}

	IPAddress::IPAddress(const sockaddr_in& addr4) : mVersion(IPVersion::kIPv4)
	{
		assert(addr4.sin_family == AF_INET);
		memcpy(&this->mAddr, &addr4.sin_addr, sizeof(addr4));
	}

	IPAddress::IPAddress(const sockaddr_in6& addr6) : mVersion(IPVersion::kIPv6)
	{
		assert(addr6.sin6_family == AF_INET6);
		memcpy(&this->mAddr, &addr6.sin6_addr, sizeof(addr6));
	}

	IPAddress::IPAddress(const in_addr& addr4) : mVersion(IPVersion::kIPv4)
	{
		memcpy(&this->mAddr.mIpAddress4.mAddr4.mBytes[0], &addr4, sizeof(addr4));
	}

	IPAddress::IPAddress(const in6_addr& addr6) : mVersion(IPVersion::kIPv6)
	{
		memcpy(&this->mAddr.mIpAddress6.mAddr6.mBytes[0], &addr6, sizeof(addr6));
	}

	IPAddress::IPAddress(const IPAddressV4& addr4) noexcept : mVersion(IPVersion::kIPv4)
	{
		this->mAddr.mIpAddress4 = addr4;
	}

	IPAddress::IPAddress(const IPAddressV6& addr6) noexcept : mVersion(IPVersion::kIPv6)
	{
		this->mAddr.mIpAddress6 = addr6;
	}

	inline IPAddress::IPAddress(IPAddressV4&& addr4) noexcept : mVersion(IPVersion::kIPv4)
	{
		this->mAddr.mIpAddress4 = addr4;
	}

	inline IPAddress::IPAddress(IPAddressV6&& addr6) noexcept : mVersion(IPVersion::kIPv6)
	{
		this->mAddr.mIpAddress6 = addr6;
	}

	IPAddress::IPAddress(const sockaddr* addr)
	{
		assert(addr != nullptr);
		assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);
		if (addr->sa_family == AF_INET)
		{
			memcpy(&this->mAddr.mIpAddress4.mAddr4.mBytes[0], addr->sa_data, sizeof(in_addr));
			this->mVersion = IPVersion::kIPv4;
			//const struct sockaddr_in* addr = reinterpret_cast<const struct sockaddr_in*>(addr);
		}
		else if (addr->sa_family == AF_INET6)
		{
			//TO DO: test, warning might read 16 out of 14 mBytes warning
			memcpy(&this->mAddr.mIpAddress6.mAddr6.mBytes[0], &addr->sa_data, sizeof(in6_addr));
			this->mVersion = IPVersion::kIPv6;
		}
	}

	//TO DO: Fix so that addr getVersion is chosen based on addrLengt(socklen_t)
	IPAddress::IPAddress(const sockaddr* addr, socklen_t addrLength)
	{
		assert(addr != nullptr);
		assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);
		if (addr->sa_family == AF_INET)
		{
			memcpy(&this->mAddr.mIpAddress4.mAddr4.mBytes[0], &addr, sizeof(in_addr));
			//const struct sockaddr_in* addr = reinterpret_cast<const struct sockaddr_in*>(addr);
			this->mVersion = IPVersion::kIPv4;
		}
		else if (addr->sa_family == AF_INET6)
		{
			memcpy(&this->mAddr.mIpAddress6.mAddr6.mBytes[0], &addr, sizeof(in6_addr));
			this->mVersion = IPVersion::kIPv6;
		}
	}

	IPAddress& IPAddress::operator=(IPEndPoint& rhs) noexcept
	{
		*this = rhs.ipAddress();
		return *this;
	}

	IPAddress& IPAddress::operator=(const IPAddress& ipAddr) noexcept
	{
		this->mVersion = ipAddr.mVersion;
		this->mAddr.mIpAddress6 = ipAddr.mAddr.mIpAddress6;
		return *this;
	}

	IPAddress& IPAddress::operator=(const IPAddressV4& ipAddr4) noexcept
	{
		this->clear();
		this->mAddr.mIpAddress4 = ipAddr4;
		return *this;
	}

	IPAddress& IPAddress::operator=(const IPAddressV6& ipAddr6) noexcept
	{
		this->clear();
		this->mAddr.mIpAddress6 = ipAddr6;
		return *this;
	}


	IPAddress& IPAddress::operator=(IPAddress&& ipAddr) noexcept
	{
		*this = ipAddr;
		return *this;
	}

	IPAddress& IPAddress::operator=(IPAddressV4&& ipAddr4) noexcept
	{
		this->mAddr.mIpAddress4 = ipAddr4;
		this->mVersion = IPVersion::kIPv4;
		return *this;
	}

	IPAddress& IPAddress::operator=(IPAddressV6&& ipAddr6) noexcept
	{
		this->mAddr.mIpAddress6 = ipAddr6;
		this->mVersion = IPVersion::kIPv6;
		return *this;
	}

	bool IPAddress::operator==(const IPEndPoint& rhs) const noexcept
	{
		return *this == rhs.ipAddress();
	}

	bool IPAddress::operator==(const IPAddress& ipAddr) const noexcept
	{
		if (this->isIPv4() && ipAddr.isIPv4())
			return this->mAddr.mIpAddress4 == ipAddr.mAddr.mIpAddress4;
		if (this->isIPv6() && ipAddr.isIPv6())
			return this->mAddr.mIpAddress6 == ipAddr.mAddr.mIpAddress6;
		return false; //IPAddress did not match getVersion
	}

	bool IPAddress::operator==(const IPAddressV4& ipAddr4) const noexcept
	{
		if (this->isIPv4())
		{
			return this->mAddr.mIpAddress4 == ipAddr4;
		}
		return false;
	}

	bool IPAddress::operator==(const IPAddressV6& ipAddr6) const noexcept
	{
		if (this->isIPv6())
		{
			return this->mAddr.mIpAddress6 == ipAddr6;
		}
		return false;
	}

	bool IPAddress::operator!=(const IPEndPoint& rhs) const noexcept
	{
		return !this->operator==(rhs);
	}

	bool IPAddress::operator!=(const IPAddress& ipAddr) const noexcept
	{
		return !this->operator==(ipAddr);
	}

	bool IPAddress::operator!=(const IPAddressV4& ipAddr4) const noexcept
	{
		return !this->operator==(ipAddr4);
	}

	bool IPAddress::operator!=(const IPAddressV6& ipAddr6) const noexcept
	{
		return !this->operator==(ipAddr6);
	}

	std::ostream& operator<<(std::ostream& rhs, const IPAddress& lhs)
	{
		assert(lhs.mVersion != IPVersion::kUnknown);
		const auto ipVers = lhs.getVersion();
		if (ipVers == IPVersion::kIPv4)
		{
			rhs << lhs.asIPv4();
		}
		else if (ipVers == IPVersion::kIPv6)
		{
			rhs << lhs.asIPv6();
		}
		return rhs;
	}
}
