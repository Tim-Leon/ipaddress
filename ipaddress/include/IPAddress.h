#pragma once
#include "IPAddressV4.h"
#include "IPAddressV6.h"

namespace ip_address
{
	/*
		holds either ipv4 or ipv6 address with unknown address getVersion.
	*/
	struct mNetworkAddress
	{
		union
		{
			IPAddressV4 addr4;
			IPAddressV6 addr6 = {};
		};
	};

	class IPEndPoint;
	/**
	*	IPAddress class containing either ipv4 or ipv6 address.
	*/
	class IPAddress
	{
		friend class IPEndPoint;
		friend class IPAddressV6;
		friend class IPAddressV4;
	public:
		IPAddress() = default;
		virtual ~IPAddress() = default;
		IPAddress(const IPAddress& addr) noexcept { *this = addr; }
		IPAddress(IPAddress&& addr) noexcept { *this = addr; }

		explicit IPAddress(const IPAddressV4& addr4) noexcept;
		explicit IPAddress(const IPAddressV6& addr6) noexcept;

		explicit IPAddress(IPAddressV4&& addr4) noexcept;
		explicit IPAddress(IPAddressV6&& addr6) noexcept;

		explicit IPAddress(const char* ip);
		explicit IPAddress(const std::string& ip);

		explicit IPAddress(const ByteArray4&& addr4) noexcept;
		explicit IPAddress(const ByteArray4& addr4) noexcept;

		explicit IPAddress(const ByteArray16&& addr6) noexcept;
		explicit IPAddress(const ByteArray16& addr6) noexcept;

		explicit IPAddress(const sockaddr_in& addr4);
		explicit IPAddress(const sockaddr_in6& addr6);
		explicit IPAddress(const in_addr& addr4);
		explicit IPAddress(const in6_addr& addr6);

		explicit IPAddress(const sockaddr* addr);
		IPAddress(const sockaddr* addr, socklen_t addrLength);
	public:
		//Comparison operators
		bool operator==(const IPEndPoint& rhs) const noexcept;
		bool operator==(const IPAddress& ipAddr) const noexcept;
		bool operator==(const IPAddressV4& ipAddr4) const noexcept;
		bool operator==(const IPAddressV6& ipAddr6) const noexcept;
		//Comparison operators
		bool operator!=(const IPEndPoint& rhs) const noexcept;
		bool operator!=(const IPAddress& ipAddr) const noexcept;
		bool operator!=(const IPAddressV4& ipAddr4) const noexcept;
		bool operator!=(const IPAddressV6& ipAddr6) const noexcept;
		//Assignment operators
		IPAddress& operator=(IPEndPoint& rhs) noexcept;
		IPAddress& operator=(const IPAddress& ipAddr) noexcept;
		IPAddress& operator=(const IPAddressV4& ipAddr4) noexcept;
		IPAddress& operator=(const IPAddressV6& ipAddr6) noexcept;
		//Move operators
		IPAddress& operator=(IPAddress&& ipAddr) noexcept;
		IPAddress& operator=(IPAddressV4&& ipAddr4) noexcept;
		IPAddress& operator=(IPAddressV6&& ipAddr6) noexcept;

	public:
		/*
		 * parse an IP address of either an IPv4- or IPv6-address string into an IPAddress object.
		 * ...
		 * @param addr [out] result after parsing IP string
		 * @param ip [in] string to be parsed
		 * @return true if it succeeded
		 */
		NODISCARD static bool parseIPAddress(IPAddress& addr, const std::string& ip) noexcept;
		/*
		* Clears IPAddress to 0 and IPVersion to unknown
		*/
		void clear() noexcept;
		/*
		* Return the fanmily of the address. The return value will be return Unknown if the IPVersion was not IPv4 or IPv6. IPv4OrIPv6, kIPv4AndIPv6 and are never returned by this method.
		*/
		NODISCARD IPVersion getVersion() const noexcept;
		/*
		* 
		*/
		NODISCARD uint32_t scope() const noexcept;
		/*
		* @return true if IPVersion is kIPv6.
		*/
		NODISCARD bool isIPv6() const noexcept;
		/*
		* @return true if IPVersion is kIPv4. 
		*/
		NODISCARD bool isIPv4() const noexcept; 
		/*
		* 
		*/
		IPAddressV6& asIPv6() const;

		IPAddressV4& asIPv4() const;

		std::string getString() const;
	public:
		/*
		* Only IPv4 addresses can be broadcast. Broadcast is an address with all bytes as 0.
		*/
		NODISCARD bool isBroadcast() const noexcept;
		/*
		 * Wildcard is an address with all bytes as 0.
		 */
		NODISCARD bool isWildcard() const noexcept;
		/*
		* 
		*/
		NODISCARD bool isAny() const noexcept;
		/*
		* An unicast address is neither a wildcard, broadcast or multicast address.
		*/
		NODISCARD bool isUnicast() const noexcept;
	public:
		friend std::ostream& operator<<(std::ostream& rhs, const IPAddress& lhs);
	protected:
		union IPAddressStorage
		{
			mutable IPAddressV6 mIpAddress6;
			mutable IPAddressV4 mIpAddress4;
		} mAddr = {};

		IPVersion mVersion = IPVersion::kUnknown;
	};
}
