#pragma once
#include <iostream>
#include <string>
#include "IPAddress.h"
#include "util/Endianness.h"
//#include "IPAddressStruct.h"
//The primary form : The preferred form is x : x:x:x:x:x:x:x, 
//where the "x"s are the hexadecimal value of the eight 16 - bit pieces of the address.Two examples :
//  fe80 : 0 : 0 : 0 : 207 : 30ee : edcb : d05d
//  1080 : 0 : 0 : 0 : 1 : 700 : 200B : 417C
namespace ip_address
{
	using port_t = uint16_t;
	using port_host_byte_order_t = port_t;

	//different underlying type to allow unique operator overloading for port_network_byte_order_t and port_host_byte_order!
	enum class _port_network_byte_order_t : uint16_t;

	using port_network_byte_order_t = _port_network_byte_order_t;
	/* convert port_t to port_network_byte_order_t */
	inline NODISCARD port_network_byte_order_t to_network_byte_order(port_host_byte_order_t arg) noexcept
	{
		return static_cast<port_network_byte_order_t>(HostToNet16(arg));
	}

	/* convert _port16_t to port_host_byte_order_t */
	inline NODISCARD port_host_byte_order_t to_host_byte_order(_port_network_byte_order_t arg) noexcept
	{
		return NetToHost16(static_cast<uint16_t>(arg));
	}

	/* convert _port16_t to integer, should be avoided as it can now be interpreted as port_t and port_host_byte_order_t */
	NODISCARD constexpr uint16_t to_integer(_port_network_byte_order_t arg) noexcept
	{
		return static_cast<uint16_t>(arg);
	}

	//Can change value according to usecase. 
#define DEFAULT_IP_ENDPOINT_PORT 8080

	/*
	 * IPEndPoint an IPAddress and port number stored in host byte order.
	 */
	class IPEndPoint final : public IPAddress
	{
		friend class IPAddress;
	public:
		IPEndPoint() = default;
		~IPEndPoint() override = default;
		IPEndPoint(const IPEndPoint& ipEnd) = default;
		IPEndPoint(IPEndPoint&& ipEnd) = default;

		explicit IPEndPoint(const char* const ip) : IPAddress(ip) { }

		explicit IPEndPoint(const std::string& ip) : IPAddress(ip) { }

		IPEndPoint(const std::string& ip, const port_host_byte_order_t port) : IPAddress(ip), mPort(port) { }

		IPEndPoint(const std::string& ip, const port_network_byte_order_t port) : IPAddress(ip),
			mPort(to_host_byte_order(port)) { }

		explicit IPEndPoint(const ByteArray4& addr4, const port_host_byte_order_t port) noexcept : IPAddress(addr4),
			mPort(port) { }

		explicit IPEndPoint(const ByteArray4&& addr4, const port_host_byte_order_t port) noexcept : IPAddress(addr4),
			mPort(port) { }

		explicit IPEndPoint(const ByteArray16& addr6, const port_host_byte_order_t port) noexcept : IPAddress(addr6),
			mPort(port) { }

		explicit IPEndPoint(const ByteArray16&& addr6, const port_host_byte_order_t port) noexcept : IPAddress(addr6),
			mPort(port) { }

		explicit IPEndPoint(const ByteArray4& addr4, const port_network_byte_order_t port) noexcept : IPAddress(addr4),
			mPort(to_host_byte_order(port)) { }

		explicit IPEndPoint(const ByteArray4&& addr4, const port_network_byte_order_t port) noexcept : IPAddress(addr4),
			mPort(to_host_byte_order(port)) { }

		explicit IPEndPoint(const ByteArray16& addr6, const port_network_byte_order_t port) noexcept : IPAddress(addr6),
			mPort(to_host_byte_order(port)) { }

		explicit IPEndPoint(const ByteArray16&& addr6,
		                    const port_network_byte_order_t port) noexcept : IPAddress(addr6),
		                                                                     mPort(to_host_byte_order(port)) { }

		explicit IPEndPoint(const ByteArray4& addr4) noexcept : IPAddress(addr4) { }

		explicit IPEndPoint(const ByteArray16& addr6) noexcept : IPAddress(addr6) { }

		explicit IPEndPoint(const sockaddr_in& addr4) : IPAddress(addr4), mPort(NetToHost16(addr4.sin_port)) { }

		explicit IPEndPoint(const sockaddr_in6& addr6) : IPAddress(addr6), mPort(NetToHost16(addr6.sin6_port)) { }

		IPEndPoint(const IPAddressV4& addr4,
		           const port_host_byte_order_t port) noexcept : IPAddress(addr4), mPort(port) { }

		IPEndPoint(const IPAddressV6& addr6,
		           const port_host_byte_order_t port) noexcept : IPAddress(addr6), mPort(port) { }

		IPEndPoint(const IPAddress& addr, const port_host_byte_order_t port) : IPAddress(addr), mPort(port) { }

		IPEndPoint(const IPAddressV4& addr4, const port_network_byte_order_t port) noexcept : IPAddress(addr4),
			mPort(to_host_byte_order(port)) { }

		IPEndPoint(const IPAddressV6& addr6, const port_network_byte_order_t port) noexcept : IPAddress(addr6),
			mPort(to_host_byte_order(port)) { }

		IPEndPoint(const IPAddress& addr, const port_network_byte_order_t port) : IPAddress(addr),
			mPort(to_host_byte_order(port)) { }

		explicit IPEndPoint(const IPAddressV4& addr4) noexcept : IPAddress(addr4) { }

		explicit IPEndPoint(const IPAddressV6& addr6) noexcept : IPAddress(addr6) { }

		explicit IPEndPoint(const IPAddress& addr) : IPAddress(addr) { }

		explicit IPEndPoint(const sockaddr* addr) : IPAddress(addr) { }

		explicit IPEndPoint(const sockaddr* addr, socklen_t addrLength);
	public:
		bool operator==(const IPEndPoint& rhs) const;
		bool operator==(port_host_byte_order_t rhs) const;
		bool operator==(port_network_byte_order_t rhs) const;

		bool operator!=(const IPEndPoint& rhs) const;
		bool operator!=(port_host_byte_order_t rhs) const;
		bool operator!=(port_network_byte_order_t rhs) const;

		IPEndPoint& operator=(const IPEndPoint& rhs);
		IPEndPoint& operator=(const IPAddress& rhs);
		IPEndPoint& operator=(const IPAddressV4& rhs);
		IPEndPoint& operator=(const IPAddressV6& rhs);
		IPEndPoint& operator=(const port_host_byte_order_t& rhs);
		IPEndPoint& operator=(const port_network_byte_order_t& rhs);

		IPEndPoint& operator=(IPEndPoint&& rhs) noexcept;
		IPEndPoint& operator=(IPAddress&& rhs) noexcept;
		IPEndPoint& operator=(IPAddressV4&& rhs) noexcept;
		IPEndPoint& operator=(IPAddressV6&& rhs) noexcept;
	protected:
		//Used for sorting with IPEndPoints.
		bool operator<(const IPEndPoint& lhs) const;
	public:
		IPAddress& ipAddress() const;
		/* Port in host-byte order */
		port_host_byte_order_t getPort() const;
		/* Port in network byte order */
		port_network_byte_order_t getPortNetworkByteOrder() const;
		/*
		* @return a copy of the IPAddress.
		*/
		IPAddress getIPAddress() const;
		/*
		* 
		*/
		sockaddr_in getAddressIPv4() const;
		/*
		* 
		*/
		sockaddr_in6 getAddressIPv6() const;
		//FORMAT: x.x.x.x:port
		std::string getStringWithPort() const;
		/**
		* Clears IPAddress to 0x0, IPVersion to Unknown and sets port to DEFAULT_IP_ENDPOINT_PORT (default values)
		*/
		void clear() noexcept;
	public:
		//Use GetStringRepresentationWithPort format x.x.x.x:port
		friend std::ostream& operator<<(std::ostream& rhs, const IPEndPoint& lhs);
	private:
		port_host_byte_order_t mPort = DEFAULT_IP_ENDPOINT_PORT;
	};
}
