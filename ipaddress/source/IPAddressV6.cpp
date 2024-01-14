#include "IPAddressV6.h"
#include "IPAddress.h"
#include <sstream>

namespace ip_address
{
	IPAddressV6::IPAddressV6(const char* const ip)
	{
		if (!parseIPAddressV6(*this, ip))
			throw std::runtime_error("invalid parser input");
	}

	IPAddressV6::IPAddressV6(const std::string& ip)
	{
		if (!parseIPAddressV6(*this, ip))
			throw std::runtime_error("invalid parser input");
	}

	IPAddressV6::IPAddressV6(const sockaddr_in6& addr6)
	{
		assert(addr6.sin6_family == AF_INET6);
		memcpy(&this->mAddr6, &addr6.sin6_addr, sizeof addr6);
	}

	IPAddressV6::IPAddressV6(const in6_addr& addr6)
	{
		this->mAddr6.mIn6Addr = addr6;
	}

	bool IPAddressV6::operator==(const IPAddressV6& ipv6) const noexcept
	{
		return this->mAddr6.mBytes == ipv6.mAddr6.mBytes;
	}

	bool IPAddressV6::operator==(const IPAddressV4& ipv4) const noexcept
	{
		return *this == ipv4.toIPv6();
	}

	bool IPAddressV6::operator==(const IPAddress& address) const noexcept
	{
		return this->mAddr6.mBytes == address.mAddr.mIpAddress6.mAddr6.mBytes;
	}

	bool IPAddressV6::operator!=(const IPAddressV6& ipv6) const noexcept
	{
		return !this->operator==(ipv6);
	}

	bool IPAddressV6::operator!=(const IPAddressV4& ipv4) const noexcept
	{
		return !this->operator==(ipv4);
	}

	bool IPAddressV6::operator!=(const IPAddress& address) const noexcept
	{
		return !this->operator==(address);
	}

	IPAddressV6& IPAddressV6::operator=(const IPAddressV6& address) noexcept
	{
		this->mAddr6 = address.mAddr6;
		return *this;
	}

	IPAddressV6& IPAddressV6::operator=(const IPAddressV4& ipv4) noexcept
	{
		*this = ipv4.toIPv6();
		return *this;
	}

	IPAddressV6& IPAddressV6::operator=(const IPAddress& address) noexcept
	{
		*this = address.mAddr.mIpAddress6;
		return *this;
	}

	IPAddressV6& IPAddressV6::operator=(IPAddressV6&& address) noexcept
	{
		*this = address;
		return *this;
	}

	bool IPAddressV6::operator<(const IPAddressV6& rhs) const noexcept
	{
		return this->mAddr6.mBytes < rhs.mAddr6.mBytes;
	}

	NODISCARD bool IPAddressV6::parseIPAddressV6(IPAddressV6& addr6, const std::string& ip) noexcept
	{
		assert(ip.empty() == false && "Can not parse empty input");

		in6_addr inAddr6;
		//Convert ip characters to address in mBytes
		int result = inet_pton(AF_INET6, ip.c_str(), &inAddr6);
		if (result == 1)
		{
			memcpy(&addr6.mAddr6, &inAddr6, 16);
			return true;
		}

		//hostname IPv6
		addrinfo hints = { 0 };
		hints.ai_family = AF_INET6; // IPv6 addresses only

		addrinfo* hostinfo = nullptr;
		result = getaddrinfo(ip.c_str(), nullptr, &hints, &hostinfo);
		if (result == 0) //Is a hostname
		{
			auto host_addr = reinterpret_cast<sockaddr_in6*>(hostinfo->ai_addr);
			memcpy(&addr6.mAddr6, &host_addr->sin6_addr, 16);
			freeaddrinfo(hostinfo);
			return true;
		}
		freeaddrinfo(hostinfo);
		return false;
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
	bool IPAddressV6::mapIPv6ToIPv4(IPAddressV6& addr6, IPAddressV4& addr4) noexcept
	{
		//TODO might be wrong check https://datatracker.ietf.org/doc/html/rfc3513#section-2.5.5
		constexpr std::array<uint8_t, 10> arr = { 0 };
		if (!memcmp(addr6.bytes().data(), arr.data(), arr.size())) return false;
		constexpr uint16_t val2 = 0xFFFF;
		if (static_cast<uint16_t>(addr6.bytes()[10]) == val2) return false;
		(uint32_t&)*addr4.bytes().data() = (uint32_t)addr6.bytes()[12];
		return true;
	}

	IPAddressV4 IPAddressV6::toIPv4() const
	{
		IPAddressV4 ipv4;
		if (mapIPv6ToIPv4((IPAddressV6&)*this, ipv4) != true)
		{
			throw std::runtime_error("Can not convert IPv6- to IPv4-address");
		}
		return ipv4;
	}

	in6_addr IPAddressV6::getOnWireAddress() const
	{
		return mAddr6.mIn6Addr;
	}


	/**
	* @return a byte array
	*/

	ByteArray16& IPAddressV6::bytes()
	{
		return this->mAddr6.mBytes;
	}

	sockaddr_in6 IPAddressV6::getSockaddrIn6() const
	{
		sockaddr_in6 addr6In = {};
		memcpy(&addr6In.sin6_addr, &mAddr6, sizeof(mAddr6));
		addr6In.sin6_family = AF_INET6;
		return addr6In;
	}

	std::string IPAddressV6::getString() const
	{
		std::stringstream ss;
		int mIndex = 0;
		for (const auto& i : mAddr6.mBytes)
		{
			ss << static_cast<int>(i);
			if (mIndex < 8)
				ss << ':';
			mIndex++;
		}
		return ss.str();
		//return std::string(addr6.mBytes.begin(), addr6.mBytes.end());
	}

	size_t IPAddressV6::getSize() const
	{
		return this->mAddr6.mBytes.size();
	}

	// IPv4 mapped addresses have their first 10 mBytes set to 0, the next 2 mBytes
	// set to 255 (0xff);
	bool IPAddressV6::isIPv4Mapped() const noexcept
	{
		for (int i = 0; i < 10; i++)
		{
			if (mAddr6.mBytes[i] != 0x00)
			{
				return false;
			}
		}
		return mAddr6.mBytes[11] == 0xff && mAddr6.mBytes[12] == 0xff;
	}


	bool IPAddressV6::isLoopback() const noexcept
	{
		return *this == loopback();
	}

	bool IPAddressV6::inSubnet() const noexcept
	{
		//TODO fix
		return false;
	}

	bool IPAddressV6::isMasked() const noexcept
	{
		//TODO fix
		return false;
	}

	bool IPAddressV6::isUnspecified() const noexcept
	{
		/* https://datatracker.ietf.org/doc/html/rfc4291#section-2.5.2
		 * The address 0:0:0:0:0:0:0:0 is called the unspecified address
		 */
		return this->mAddr6.mBytes == ByteArray16{ 0 };
	}

	bool IPAddressV6::isRoutable() const noexcept
	{
		//TODO implement isRoutable() correctly
		return !this->isUnspecified() && !this->isLoopback();
	}


	bool IPAddressV6::isMulticast() const noexcept
	{
		/*
 *https://datatracker.ietf.org/doc/html/rfc3513#section-2.6
*2.7 Multicast Addresses
*
*   An IPv6 multicast address is an identifier for a group of interfaces
*   (typically on different nodes).  An interface may belong to any
*   number of multicast groups.  Multicast addresses have the following
*   format:
*
*   |   8    |  4 |  4 |                  112 bits                   |
*   +------ -+----+----+---------------------------------------------+
*   |11111111|flgs|scop|                  group ID                   |
*   +--------+----+----+---------------------------------------------+
*
*         binary 11111111 at the start of the address identifies the
*         address as being a multicast address.
*
*                                       +-+-+-+-+
*         flgs is a set of 4 flags:     |0|0|0|T|
*                                       +-+-+-+-+
*
*
*
*Hinden & Deering            Standards Track                    [Page 13]
*
*RFC 3513              IPv6 Addressing Architecture            April 2003
*
*
*            The high-order 3 flags are reserved, and must be initialized
*            to 0.
*
*            T = 0 indicates a permanently-assigned ("well-known")
*            multicast address, assigned by the Internet Assigned Number
*            Authority (IANA).
*
*            T = 1 indicates a non-permanently-assigned ("transient")
*            multicast address.
*
*         scop is a 4-bit multicast scope value used to limit the scope
*         of the multicast group.  The values are:
*
*            0  reserved
*            1  interface-local scope
*            2  link-local scope
*            3  reserved
*            4  admin-local scope
*            5  site-local scope
*            6  (unassigned)
*            7  (unassigned)
*            8  organization-local scope
*            9  (unassigned)
*            A  (unassigned)
*            B  (unassigned)
*            C  (unassigned)
*            D  (unassigned)
*            E  global scope
*            F  reserved
*
*            interface-local scope spans only a single interface on a
*            node, and is useful only for loopback transmission of
*            multicast.
*
*            link-local and site-local multicast scopes span the same
*            topological regions as the corresponding unicast scopes.
*
*            admin-local scope is the smallest scope that must be
*            administratively configured, i.e., not automatically derived
*            from physical connectivity or other, non- multicast-related
*            configuration.
*
*            organization-local scope is intended to span multiple sites
*            belonging to a single organization.
*
*            scopes labeled "(unassigned)" are available for
*            administrators to define additional multicast regions.
*
*
*
*
*Hinden & Deering            Standards Track                    [Page 14]
*
*RFC 3513              IPv6 Addressing Architecture            April 2003
*
*
*         group ID identifies the multicast group, either permanent or
*         transient, within the given scope.
*
*   The "meaning" of a permanently-assigned multicast address is
*   independent of the scope value.  For example, if the "NTP servers
*   group" is assigned a permanent multicast address with a group ID of
*   101 (hex), then:
*
*      FF01:0:0:0:0:0:0:101 means all NTP servers on the same interface
*      (i.e., the same node) as the sender.
*
*      FF02:0:0:0:0:0:0:101 means all NTP servers on the same link as the
*      sender.
*
*      FF05:0:0:0:0:0:0:101 means all NTP servers in the same site as the
*      sender.
*
*      FF0E:0:0:0:0:0:0:101 means all NTP servers in the internet.
*
*   Non-permanently-assigned multicast addresses are meaningful only
*   within a given scope.  For example, a group identified by the non-
*   permanent, site-local multicast address FF15:0:0:0:0:0:0:101 at one
*   site bears no relationship to a group using the same address at a
*   different site, nor to a non-permanent group using the same group ID
*   with different scope, nor to a permanent group with the same group
*   ID.
*
*   Multicast addresses must not be used as source addresses in IPv6
*   packets or appear in any Routing header.
*
*   Routers must not forward any multicast packets beyond of the scope
*   indicated by the scop field in the destination multicast address.
*
*   Nodes must not originate a packet to a multicast address whose scop
*   field contains the reserved value 0; if such a packet is received, it
*   must be silently dropped.  Nodes should not originate a packet to a
*   multicast address whose scop field contains the reserved value F; if
*   such a packet is sent or received, it must be treated the same as
*   packets destined to a global (scop E) multicast address.
 */
		return mAddr6.mBytes[0] == 0xFF;
	}

	bool IPAddressV6::isWildcard() const noexcept
	{
		return this->mAddr6.mBytes == ByteArray16{ 0 };
	}

	bool IPAddressV6::isGlobalUnicast() const noexcept
	{
		/*
		*https://www.ietf.org/rfc/rfc3587.txt
		An example of the resulting format of global unicast address under
		the 2000::/3 prefix that is currently being delegated by the IANA and
		consistent with the recommendations in RFC 3177 is:

		| 3 |     45 bits         |  16 bits  |       64 bits              |
		+---+---------------------+-----------+----------------------------+
		|001|global routing prefix| subnet ID |       interface ID         |
		+---+---------------------+-----------+----------------------------+
		*/
		return (this->mAddr6.mBytes[0] & 0b11100000) == 0b00100000;
		//return this->mAddr6.mWords[8] == 35948;
	}

	bool IPAddressV6::isUniqueLocal() const noexcept
	{
		//1111 110 and 1 bit not used yet. 
		//Prefix is always set to 1111 110. L bit, is set to 1 if the address is locally assigned. So far, the meaning of L bit to 0 is not defined. Therefore, Unique Local IPv6 address always starts with ‘FD’. https://www.tutorialspoint.com/ipv6/ipv6_address_types.htm
		return mAddr6.mBytes[0] == 252;
	}

	bool IPAddressV6::isLinkLocal() const noexcept
	{
		/* Link-Local
		addresses have the following format:

		|   10     |
		|  bits    |         54 bits         |          64 bits           |
		+----------+-------------------------+----------------------------+
		|1111111010|           0             |       interface ID         |
		+----------+-------------------------+----------------------------+
		*/

		return this->mAddr6.mBytes[0] == 254 && this->mAddr6.mBytes[1] == 128;
	}

	bool IPAddressV6::SiteLocal() const noexcept
	{
		/*
		 *    Site-Local addresses have the following format:

		|   10     |
		|  bits    |         54 bits         |         64 bits            |
		+----------+-------------------------+----------------------------+
		|1111111011|        subnet ID        |       interface ID         |
		+----------+-------------------------+----------------------------+
		 */
		return this->mAddr6.mBytes[0] == 0b11111110 && (this->mAddr6.mBytes[1] & 0b11000000) == 0b11000000;
	}

	bool IPAddressV6::isAnyCast() const noexcept
	{
		/*https://datatracker.ietf.org/doc/html/rfc4291#section-2.6.1
		 The Subnet-Router anycast address is predefined.  Its format is as
		   follows:





		Hinden                      Standards Track                    [Page 12]

		RFC 4291              IPv6 Addressing Architecture         February 2006



		   |                         n bits                 |   128-n bits   |
		   +------------------------------------------------+----------------+
		   |                   subnet prefix                | 00000000000000 |
		   +------------------------------------------------+----------------+

		   The "subnet prefix" in an anycast address is the prefix that
		   identifies a specific link.  This anycast address is syntactically
		   the same as a unicast address for an interface on the link with the
		   interface identifier set to zero.
		 */
		return this->mAddr6.mBytes[this->mAddr6.mBytes.size()] == 0 &&
			(this->mAddr6.mBytes[this->mAddr6.mBytes.size() - 1] & 0b00111111) == 0;
	}

	void IPAddressV6::clear() noexcept
	{
		this->mAddr6 = { 0 };
	}

	std::ostream& operator<<(std::ostream& rhs, const IPAddressV6& lhs)
	{
		return rhs << lhs.getString();
	}
}
