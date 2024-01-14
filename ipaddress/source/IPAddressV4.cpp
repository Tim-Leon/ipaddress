#include "IPAddressV4.h"

#include <cassert>
#include "IPAddress.h"
#include <sstream>

namespace ip_address
{
	IPAddressV4::IPAddressV4(const char* const ip)
	{
		if (!parseIPAddressV4(*this, ip))
			throw std::runtime_error("invalid parser input");
	}

	IPAddressV4::IPAddressV4(const std::string& ip)
	{
		if (!parseIPAddressV4(*this, ip))
			throw std::runtime_error("invalid parser input");
	}


	IPAddressV4::IPAddressV4(const sockaddr_in& addr)
	{
		assert(addr.sin_family == AF_INET);
		memcpy(&this->mAddr4, &addr.sin_addr, sizeof(addr));
	}

	IPAddressV4::IPAddressV4(const in_addr& addr)
	{
		this->mAddr4.mInAddr = addr;
	}

	bool IPAddressV4::operator==(const IPAddressV4& addr4) const noexcept
	{
		return this->mAddr4.mBytes == addr4.mAddr4.mBytes;
	}

	bool IPAddressV4::operator==(const IPAddress& addr) const noexcept
	{
		if (addr.isIPv4())
		{
			return this->mAddr4.mBytes == addr.mAddr.mIpAddress4.mAddr4.mBytes;
		}
		return false;
	}

	bool IPAddressV4::operator!=(const IPAddressV4& addr4) const noexcept
	{
		return !(this->operator==(addr4));
	}

	bool IPAddressV4::operator!=(const IPAddress& addr) const noexcept
	{
		return !(this->operator==(addr));
	}

	IPAddressV4& IPAddressV4::operator=(const IPAddressV4& addr4) noexcept
	{
		this->mAddr4 = addr4.mAddr4;
		return *this;
	}

	IPAddressV4& IPAddressV4::operator=(const IPAddress& addr) noexcept
	{
		*this = addr.mAddr.mIpAddress4;
		return *this;
	}

	IPAddressV4& IPAddressV4::operator=(IPAddressV4&& addr4) noexcept
	{
		*this = addr4;
		return *this;
	}

	bool IPAddressV4::operator<(const IPAddressV4& rhs) const noexcept
	{
		return this->mAddr4.mBytes < rhs.mAddr4.mBytes;
	}


	bool IPAddressV4::parseIPAddressV4(IPAddressV4& addr4, const std::string& ip) noexcept
	{
		assert(ip.size() <= MAX_IPV4_ADDRESS_CHAR_MAX_COUNT && ip.empty() == false);

		//IPv4 4-mBytes address(A ULONG 4mBytes)
		in_addr mInAddr;
		//Convert ip characters to address in mBytes
		int result = inet_pton(AF_INET, ip.c_str(), &mInAddr);
		if (result == 1)
		{
			memcpy(&addr4.mAddr4.mBytes[0], &mInAddr, sizeof(uint32_t));
			return true;
		}

		//hostname IPv4
		addrinfo hints = { 0 };
		hints.ai_family = AF_INET; // IPv4 addresses only

		addrinfo* hostinfo = nullptr;
		result = getaddrinfo(ip.c_str(), nullptr, &hints, &hostinfo);
		if (result == 0) //Is a hostname
		{
			auto host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);
			memcpy(&addr4.mAddr4.mBytes[0], &host_addr->sin_addr, sizeof(uint32_t));

			freeaddrinfo(hostinfo);
			return true;
		}
		freeaddrinfo(hostinfo);
		return false;
	}

	IPAddressV6 IPAddressV4::toIPv6() const
	{
		IPAddressV6 ip6;
		if (!this->mapIPv4ToIPv6((IPAddressV4&)*this, ip6)) throw
			std::runtime_error("Can not convert IPv4 address to IPv6");
		return ip6;
	}

	in_addr IPAddressV4::getOnWireAddress() const
	{
		return this->mAddr4.mInAddr;
	}

	ByteArray4& IPAddressV4::bytes()
	{
		return this->mAddr4.mBytes;
	}

	sockaddr_in IPAddressV4::getSockaddrIn4() const
	{
		sockaddr_in addrIn = {};
		memcpy(&addrIn.sin_addr, &mAddr4, sizeof(mAddr4));
		addrIn.sin_family = AF_INET;
		return addrIn;
	}

	std::string IPAddressV4::getString() const
	{
		std::stringstream ss;
		int index = 0;
		for (const auto& i : mAddr4.mBytes)
		{
			ss << std::to_string(i);
			if (index < 3)
				ss << '.';
			index++;
		}
		return ss.str();
		//return std::string(addr.mBytes.begin(), addr.mBytes.end());
	}

	size_t IPAddressV4::getSize() const
	{
		return mAddr4.mBytes.size();
	}

	//"The loopback address is guaranteed to be 127.x.x.x(any will work, 0.0.1 is standard) on any IPv4 machine and ::1 on any IPv6 machine."
	bool IPAddressV4::isLoopback() const noexcept
	{
		//127.0.0.0 to 127.255.255.255 reserved for loopback addresses according to https://en.wikipedia.org/wiki/Reserved_IP_addresses
		//It is therefore only neccesery to check if it starts with 127.x.x.x.
		return mAddr4.mBytes[0] == 127;
	}

	bool IPAddressV4::isMulticast() const noexcept
	{
		//https://stackoverflow.com/questions/22015308/c-function-to-determine-if-ip-address-is-multicast-address
		//"Addresses between 224.0.0.0 and 239.255.255.255 are multicast"
		return (static_cast<uint32_t>(this->mAddr4.mBytes[0]) & 0xF0000000) == 0xE0000000;
	}

	bool IPAddressV4::isUnicast() const noexcept
	{
		//https://serverfault.com/questions/577045/how-to-determine-ip-address-is-unicast-or-anycast
		//"Addresses between 0.0.0.0 and 223.255.255.255 are unicast"

		return !this->isBroadcast() && !this->isMulticast();
	}

	bool IPAddressV4::isBroadcast() const noexcept
	{
		return *this == this->any();
	}

	bool IPAddressV4::isWildcard() const noexcept
	{
		//is the same for ipv4.... 
		return this->isBroadcast();
	}

	//https://support.microsoft.com/en-us/help/164015/understanding-tcp-ip-addressing-and-subnetting-basics
	bool IPAddressV4::inSubnet() const noexcept
	{
		//169.254.0.0�169.254.255.255 reserved for Subnet according to https://en.wikipedia.org/wiki/Reserved_IP_addresses
		return mAddr4.mBytes[0] == 169 && mAddr4.mBytes[2] == 254;
	}

	bool IPAddressV4::isLinkLocal() const noexcept
	{
		/*
		 * 169.254.0.0 - 169.254.255.255 or 169.254.0.0/16
		 * https://datatracker.ietf.org/doc/html/rfc3927
		 */

		 /* compair the first 2 bytes then check if they are equal to 169.254 */
		return (static_cast<uint16_t>(mAddr4.mBytes[0]) & 0xFFFF0000) == 0xA9FE0000;
	}

	bool IPAddressV4::isPrivate() const noexcept
	{
		/*
		* Private Addresses
		* Class A: 10.0.0.0 - 10.255.255.255
		* Class B: 172.16.0.0 - 172.31.255.255
		* Class C: 192.168.0.0 - 192.168.255.255
		*/
		return (mAddr4.mBytes[0] == 10) ||
			(mAddr4.mBytes[0] == 172 && mAddr4.mBytes[1] >= 16 && mAddr4.mBytes[1] <= 31) ||
			(mAddr4.mBytes[0] == 192 && mAddr4.mBytes[1] == 168);
	}

	inline bool IPAddressV4::inSubnetWithMask(const IPAddressV4& addr, ByteArray4 maskAddr)
	{
		//TODO implement
		//mask<uint8_t, maskAddr.size()>(addr.mAddr4.mBytes, maskAddr);
		return false;
	}

	bool IPAddressV4::isMasked() const
	{
		//TODO implement
		return false;
	}

	bool IPAddressV4::isAny() const
	{
		return *this == any();
	}

	bool IPAddressV4::isRoutableAddress() const
	{
		return !isPrivate() && !isLoopback() && !isMulticast();
	}

	//The IPv4 to IPv6 formant is specified in rfc4291
	//The format for IPv4-mapped IPv6 addresses is as followed 
	/*
		|                80 bits               | 16 |      32 bits        |
		+--------------------------------------+--------------------------+
		|0000..............................0000|FFFF|    IPv4 address     |
		+--------------------------------------+----+---------------------+
						 word[5]
		
	*/
	bool IPAddressV4::mapIPv4ToIPv6(IPAddressV4& addr4, IPAddressV6& addr6) noexcept
	{
		auto a = addr6.bytes();
		a[10] = 0xFF;
		a[11] = 0xFF;
		(uint32_t&)a[12] = (uint32_t)*addr4.bytes().data();
		//memcpy(reinterpret_cast<uint32_t*>(a[12]), addr4.bytes().data(), sizeof(addr4.bytes()));
		return true;
	}

	IPAddressV4Class IPAddressV4::getIPAddressClass() const
	{
		/*
		*  https://www.youtube.com/watch?v=vcArZIAmnYQ&list=PLSNNzog5eydt_plAtt3k_LYuIXrAS4aDZ&ab_channel=SunnyClassroom
		* X is any combination!
		*	Class A   0 - 127  0XXXXXXX
		*	Class B 128 - 191  10XXXXXX
		*	Class C 192 - 223  110XXXXX
		*	Class D 224 - 239  1110XXXX
		*	Class E 240 - 255  1111XXXX
		* Note* that even 127 is class A, even though it's loopback.
		*/
		if (CHECK_BIT(this->mAddr4.mBytes[0], 7))
			return IPAddressV4Class::B;
		if (CHECK_BIT(this->mAddr4.mBytes[0], 6))
			return IPAddressV4Class::C;
		if (CHECK_BIT(this->mAddr4.mBytes[0], 5))
			return IPAddressV4Class::D;
		if (CHECK_BIT(this->mAddr4.mBytes[0], 4))
			return IPAddressV4Class::E;
		return IPAddressV4Class::A;
	}

	void IPAddressV4::clear() noexcept
	{
		this->mAddr4 = { 0 };
	}

	std::ostream& operator<<(std::ostream& rhs, const IPAddressV4& lhs)
	{
		return rhs << lhs.getString();
	}
}
