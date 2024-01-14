#pragma once
#include "util/Config.h"
#include <type_traits>

namespace ip_address
{
	/*
	 * Note IPVersion has overwritten assignment operator to support kIPv4OrIPv6 and kIPv4AndIPv6
	 */
	enum class IPVersion
	{
		kUnknown,
		kIPv4 = AF_INET,
		kIPv6 = AF_INET6,
		kIPv4OrIPv6,
		kIPv4AndIPv6,
	};

	using IPFamily = IPVersion;

	using ip_version_ut = std::underlying_type<IPVersion>::type;
	NODISCARD constexpr bool operator==(IPVersion lhs, IPVersion rhs)
	{
		if (static_cast<ip_version_ut>(lhs) == static_cast<ip_version_ut>(rhs)) return true;
		if (static_cast<ip_version_ut>(lhs) == static_cast<ip_version_ut>(IPVersion::kIPv4OrIPv6)
			|| static_cast<ip_version_ut>(rhs) == static_cast<ip_version_ut>(IPVersion::kIPv4OrIPv6))
		{
			if (static_cast<ip_version_ut>(lhs) == static_cast<ip_version_ut>(IPVersion::kIPv4)
				|| static_cast<ip_version_ut>(lhs) == static_cast<ip_version_ut>(IPVersion::kIPv6))
				return true;
			if (static_cast<ip_version_ut>(rhs) == static_cast<ip_version_ut>(IPVersion::kIPv4)
				|| static_cast<ip_version_ut>(rhs) == static_cast<ip_version_ut>(IPVersion::kIPv6))
				return true;
		}
		return false;
	}

	NODISCARD constexpr bool operator!=(const IPVersion lhs, const IPVersion rhs)
	{
		return !(lhs == rhs);
	}

	constexpr const char* ToString(IPVersion x)
	{
		switch (x)
		{
		case IPVersion::kIPv4: return "IPv4";
		case IPVersion::kIPv6: return "IPv6";
		case IPVersion::kIPv4AndIPv6: return "IPv4AndIPv6";
		case IPVersion::kIPv4OrIPv6: return "IPv4OrIPv6";
		case IPVersion::kUnknown: return "Unknown";
		default: return "Invalid";
		}
	}
}
