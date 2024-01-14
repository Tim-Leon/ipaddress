#pragma once
#include <array>
#include <ostream>
#include <string>
#include "IPVersion.h"

namespace ip_address
{
	/*
	*	<-INFO->
	*	Technically, an IPv6 address is case-insensitive, but RFC 5952, A Recommendation for IPv6 Address Text Representation,
	*	Section 4.3 Lowercase says: The characters "a", "b", "c", "d", "e", and "f" in an IPv6 address MUST be represented in lowercase
	*	Resource http://v6decode.com/ https://datatracker.ietf.org/doc/html/rfc3513#section-2.6
	*
	*	Unicast is one to one
	*	Multicast is ont to many
	*	Anycast is one to anyone of a group
	*
	*
	*	2.4 Address Type Identification
	https://datatracker.ietf.org/doc/html/rfc3513#section-2.6
   The type of an IPv6 address is identified by the high-order bits of
   the address, as follows:

   Address type         Binary prefix        IPv6 notation   Section
   ------------         -------------        -------------   -------
   Unspecified          00...0  (128 bits)   ::/128          2.5.2
   Loopback             00...1  (128 bits)   ::1/128         2.5.3
   Multicast            11111111             FF00::/8        2.7
   Link-local unicast   1111111010           FE80::/10       2.5.6
   Site-local unicast   1111111011           FEC0::/10       2.5.6
   Global unicast       (everything else)
	*/
#define MAX_IPV6_ADDRESS_SIZE_BYTES 16
#define MAX_IPV6_ADDRESS_CHAR_MAX_COUNT 39 //includes separation with colon 
	using ByteArray16 = std::array<uint8_t, 16>;
	using WordArray8 = std::array<uint16_t, 8>;
	class IPAddress;
	class IPAddressV4;
	//https://www.cisco.com/c/en/us/support/docs/ip/routing-information-protocol-rip/13788-3.html

	/*
	 * IPAddressV6
	 */
	class IPAddressV6 final
	{
		friend class IPAddressV4;
		friend class IPAddress;
		friend class IPEndPoint;
	public:
		IPAddressV6() = default;
		~IPAddressV6() = default;
		IPAddressV6(const IPAddressV6& addr6) noexcept = default;
		IPAddressV6(IPAddressV6&& addr6) noexcept = default;
		explicit IPAddressV6(const char* ip);
		explicit IPAddressV6(const std::string& ip);
		explicit IPAddressV6(const ByteArray16& ip) noexcept { this->mAddr6.mBytes = ip; };
		explicit IPAddressV6(const ByteArray16&& ip) noexcept { this->mAddr6.mBytes = ip; };
		explicit IPAddressV6(const sockaddr_in6& addr6);
		explicit IPAddressV6(const in6_addr& addr6);

		static const IPAddressV6 unspecified() { return IPAddressV6(ByteArray16{ 0 }); }
		//Is the same as ::1 and that can be used instead. 
		static const IPAddressV6 loopback()
		{
			return IPAddressV6(ByteArray16({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }));
		}

		static const IPAddressV6 defaultRoute() { return IPAddressV6("::"); }
	public:
		bool operator==(const IPAddressV6& ipv6) const noexcept;
		bool operator==(const IPAddressV4& ipv4) const noexcept;
		bool operator==(const IPAddress& addr) const noexcept;

		bool operator!=(const IPAddressV6& ipv6) const noexcept;
		bool operator!=(const IPAddressV4& ipv4) const noexcept;
		bool operator!=(const IPAddress& addr) const noexcept;

		IPAddressV6& operator=(const IPAddressV6& ipv6) noexcept;
		IPAddressV6& operator=(const IPAddressV4& ipv4) noexcept;
		IPAddressV6& operator=(const IPAddress& addr) noexcept;

		IPAddressV6& operator=(IPAddressV6&& ipv6) noexcept;
	protected:
		/* used when sorting IPv6 addresses */
		bool operator<(const IPAddressV6& rhs) const noexcept;
	public:
		/*
		 * parse an IPV6 string into a IPAdddressV6 object
		 * ...
		 * @param addr6 [out] result after parsing ip string
		 * @param ip [in] string to be parsed
		 * @return true if successful
		 */
		NODISCARD static bool parseIPAddressV6(IPAddressV6& addr6, const std::string& ip) noexcept;
		/*
		* map an IPv6 mapped IPv4 address back to IPv4. It can ONLY map an IPv6 address if its an IPv4 mapped IPv6 address!
		* ...
		* @param addr4 [in] IPv4 address to be converted to IPv6
		* @param addr6 [out] IPv6 mapped address
		*/
		NODISCARD static bool mapIPv6ToIPv4(IPAddressV6& addr6, IPAddressV4& addr4) noexcept;
		/*
		 * Convert IPAddressV6 to an IPAddressV4.
		 * This conversation can only be used if the address was an IPAddressV4 that was converted over to an IPAddressV6.
		 */
		NODISCARD IPAddressV4 toIPv4() const;
		/*
		 * @return in6_addr also known as on-wire format
		 */
		NODISCARD in6_addr getOnWireAddress() const;
		/**
		* @return a byte array
		*/
		ByteArray16& bytes();

		//Specific sockaddr for IPv6
		NODISCARD sockaddr_in6 getSockaddrIn6() const;
		/*
		* 
		*/
		NODISCARD std::string getString() const;
		/**
		* @return size of an IPAddressV6 in bytes
		*/
		NODISCARD size_t getSize() const;
		/*
		* An IPv6 multicast address is an identifier for a group of interfaces (typically on different nodes).
		*/
		NODISCARD bool isMulticast() const noexcept;
		/*
		* Wildcard is an IPv6 with the whole address set as 0.
		*/
		NODISCARD bool isWildcard() const noexcept;
		/*
		*
		*/
		NODISCARD bool isGlobalUnicast() const noexcept;
		/*
		*
		*/
		NODISCARD bool isUniqueLocal() const noexcept;
		/*
		*
		*/
		NODISCARD bool isLinkLocal() const noexcept;
		/*
		*
		*/
		NODISCARD bool SiteLocal() const noexcept;
		/*
		*
		*/
		NODISCARD bool isAnyCast() const noexcept;
		/*
		* Checks if the address is an IPv4 to IPv6 mapped address.
		*/
		NODISCARD bool isIPv4Mapped() const noexcept;
		/*
		* A loopbacck is an address to the local host and has the format "::/128".
		*/
		NODISCARD bool isLoopback() const noexcept;
		/*
		*
		*/
		NODISCARD bool inSubnet() const noexcept;
		/*
		*
		*/
		NODISCARD bool isMasked() const noexcept;
		/*
		* ::/128
		*/
		NODISCARD bool isUnspecified() const noexcept;
		/*
		*
		*/
		NODISCARD bool isRoutable() const noexcept;
		/* Clears IPv6 address to 0 */
		void clear() noexcept;
	public:
		friend std::ostream& operator<<(std::ostream& rhs, const IPAddressV6& lhs);
	private:
		union IPAddressStorageV6
		{
			ByteArray16 mBytes;
			WordArray8 mWords;

			struct
			{
				union
				{
					struct
					{
						uint8_t mPrefix : 7;
						uint8_t mLocalBit : 1;
						std::array<uint8_t, 5> mGlobalId;
						uint16_t mSubnetId;
					};

					uint64_t mVal;
				} mNetworkInterface; //64 bit
				uint64_t mInterfaceId; //64 bit
			} ipv6;

			in6_addr mIn6Addr;
		} mAddr6 = { 0 };
	};
}
