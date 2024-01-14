#pragma once
#include <array>
#include <cassert>
#include <vector>
#include "IPAddressV6.h"
#define MAX_IPV4_ADDRESS_CHAR_MAX_COUNT 15 //This includes separation with dots but without them its only 12 characters
#define MAX_IPV4_ADDRESS_SIZE_BYTES 4

namespace ip_address
{
	/*
	* Contains the types of IPv4 address classes and is defined by the first octale in an IP-address.
	*/
	enum class IPAddressV4Class
	{
		A,
		B,
		C,
		//Multicast
		D,
		//Experimental
		E,
	};

	constexpr const char* ToString(IPAddressV4Class x)
	{
		switch (x)
		{
		case IPAddressV4Class::A: return "A";
		case IPAddressV4Class::B: return "B";
		case IPAddressV4Class::C: return "C";
		case IPAddressV4Class::D: return "D";
		default: return "Unknown";
		}
	};

	using ByteArray3 = std::array<uint8_t, 3>;
	using ByteArray4 = std::array<uint8_t, 4>;
	/*
		Class representing IPv4-addresses in host byte order
	*/
	class IPAddressV4 final
	{
		friend class IPAddressV6;
		friend class IPAddress;
		friend class IPEndPoint;
	public:
		IPAddressV4() = default;
		~IPAddressV4() = default;
		IPAddressV4(const IPAddressV4& addr4) noexcept = default;
		IPAddressV4(IPAddressV4&& addr4) noexcept = default;

		explicit IPAddressV4(const char* ip);
		explicit IPAddressV4(const std::string& ip);
		explicit IPAddressV4(const ByteArray4& ip) noexcept { this->mAddr4.mBytes = ip; };
		explicit IPAddressV4(const ByteArray4&& ip) noexcept { this->mAddr4.mBytes = ip; };
		explicit IPAddressV4(const sockaddr_in& addr);
		explicit IPAddressV4(const in_addr& addr);
		/* 127.0.0.1 */
		static const IPAddressV4 loopback() { return IPAddressV4(ByteArray4({ 127, 0, 0, 1 })); }
		/*
		* Any is 0.0.0.0 and is a non-routable meta-address used to designate an invalid, unknown or non-applicable target.
		* It can also be used to specifie "any IPv4 address at all" when binded to a listening socket.
		*/
		static IPAddressV4 any() { return IPAddressV4(ByteArray4{ 0, 0, 0, 0 }); }

		static IPAddressV4 none() { return IPAddressV4(ByteArray4{ 255, 255, 255, 255 }); }
	public:
		bool operator==(const IPAddressV4& addr4) const noexcept;
		bool operator==(const IPAddress& addr) const noexcept;

		bool operator!=(const IPAddressV4& addr4) const noexcept;
		bool operator!=(const IPAddress& addr) const noexcept;

		IPAddressV4& operator=(const IPAddressV4& addr4) noexcept;
		IPAddressV4& operator=(const IPAddress& addr) noexcept;

		IPAddressV4& operator=(IPAddressV4&& addr4) noexcept;
	protected:
		/* used when sorting IPv4 addresses */
		bool operator<(const IPAddressV4& rhs) const noexcept;
	public:
		/*
		 * parse an IPv4 address string into an IPAddressV4 object.
		 * ...
		 * @param addr4 [out] result after parsing ip string
		 * @param ip [in] string to be parsed
		 * @return true if it succeeded
		 */
		NODISCARD static bool parseIPAddressV4(IPAddressV4& addr4, const std::string& ip) noexcept;

		/*
		* map IPv4 address over to an IPv6 address
		* ...
		* @param addr4 [in] IPv4 address to be converted to IPv6
		* @param addr6 [out] IPv6 mapped address
		*/
		NODISCARD static bool mapIPv4ToIPv6(IPAddressV4& addr4, IPAddressV6& addr6) noexcept;

		/*
		* Converts an ipv4 address to ipv6.
		*/
		NODISCARD IPAddressV6 toIPv6() const;
		/*
		* @return ipv4 as long int.
		*/
		NODISCARD uint64_t toLong() const;
		/*
		* @return in_addr an IPv4 Internet address in 'on-wire' format structure.
		*/
		NODISCARD in_addr getOnWireAddress() const;
		/*
		* @return ipv4 as a byte array.
		*/
		ByteArray4& bytes();
		/*
		* Specific sockaddr for IPv4
		*/
		NODISCARD sockaddr_in getSockaddrIn4() const;
		/*
		* @return a string that contains each byte in an IP-address separated by a '.' e.g 192.168.1.66
		*/
		NODISCARD std::string getString() const;
		/**
		* @return size of IPAddressV4 in bytes
		*/
		NODISCARD size_t getSize() const;
		/*
		* looback address is an address used for communicating with localhost.
		* For IPv4, loopback is 127.0.0.1.
		* For IPv6, loopback is ::1.
		*/
		NODISCARD bool isLoopback() const noexcept;
		/*
		* Addresses between 224.0.0.0 and 239.255.255.255 are multicast
		*/
		NODISCARD bool isMulticast() const noexcept;
		/*
		* An IPv4 unicast address is neither a broadcast or multicast address.
		*/
		NODISCARD bool isUnicast() const noexcept;
		/*
		* Only IPv4 addresses can be broadcast. Broadcast is an address with all bytes as 0.
		*/
		NODISCARD bool isBroadcast() const noexcept;
		/*
		 * Wildcard is an address with all bytes as 0.
		 */
		NODISCARD bool isWildcard() const noexcept;
		/*
		* IPv4 allows for a variation of the network and host segments of an IP address, known as subnetting, can be used to physically and logically design a network.
		* Subnetwork addresses enhance local routing capabilities, while reducing the number of network addresses required.
		*/
		NODISCARD bool inSubnet() const noexcept;
		/*
		* Link local is used when the host cannot find a DHCP server or are running into communication problems between the DHCP
		* DHCP means the host can not access the internet but can still communicate with LAN devices.
		* Link local packets are not "Not Forwarded" meaning
		*/
		NODISCARD bool isLinkLocal() const noexcept;
		/*
		* Private IP addresses can not connect directly to the internet.
		* Access to the internet must be brokered by a router or other such devices that supports NAT
		*/
		NODISCARD bool isPrivate() const noexcept;
		/*
		*
		*/
		NODISCARD bool inSubnetWithMask(const IPAddressV4& addr, ByteArray4 maskAddr);
		/*
		*
		*/
		NODISCARD bool mask(const IPAddressV4& mask);
		/*
		*
		*/
		NODISCARD bool isMasked() const;
		/*
		* Any is 0.0.0.0 and is a non-routable meta-address used to designate an invalid, unknown or non-applicable target.
		* It can also be used to specifie "any IPv4 address at all" when binded to a listening socket.
		*/
		NODISCARD bool isAny() const;
		/*
		 * Rotatable addresses are all addresses that are neither private addresses, loopback, multicast or experimental blocks.
		 */
		NODISCARD bool isRoutableAddress() const;
		/* Return the IPv4 address class type IPAddressV4Class A, B, C, D or E */
		NODISCARD IPAddressV4Class getIPAddressClass() const;
		/* Clears IPAddress to 0 */
		void clear() noexcept;
	public:
		friend std::ostream& operator<<(std::ostream& rhs, const IPAddressV4& lhs);
	private:
		union IPAddressStorageV4
		{
			ByteArray4 mBytes;
			in_addr mInAddr;

			struct
			{
				ByteArray3 mNetworkAddress;
				uint8_t mHostAddress;
			};
		} mAddr4 = { 0 };
	};

	namespace details
	{
		template <typename T, size_t Size>
		std::array<T, Size> mask(const std::array<T, Size>& x, const std::array<T, Size>& y)
		{
			static_assert(Size > 0, "Can not mask empty array");
			std::array<T, Size> res = { {0} };
			const size_t size = x.size();
			for (size_t i = 0; i < size; i++)
			{
				res[i] = x[i] & y[i];
			}
			return res;
		}

		template <typename T>
		std::vector<T> mask(const std::vector<T>& x, const std::vector<T>& y)
		{
			static_assert(x.size() == y.size() && "Can not mask vector with different size");
			static_assert(!x.empty() > 0 && "Can not mask empty vector");
			std::vector<T> res;
			res.resize(x.size());
			const size_t size = x.size();
			for (size_t i = 0; i < size; i++)
			{
				res[i] = x[i] & y[i];
			}
			return res;
		}

		inline std::string mask(const std::string& x, const std::string& y)
		{
			assert(x.size() == y.size() && "Can not mask string with different size");
			assert(!x.empty() && "Can not mask empty string");

			std::string res;
			res.resize(x.size());
			const size_t size = x.size();
			for (size_t i = 0; i < size; i++)
			{
				res[i] = x[i] & y[i];
			}
			return res;
		}
	}
}
