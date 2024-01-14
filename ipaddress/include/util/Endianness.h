#pragma once
#include <cstdint>
#ifdef _MSC_VER

#include <stdlib.h>
#define BSWAP_16(x) _byteswap_ushort(x)
#define BSWAP_32(x) _byteswap_ulong(x)
#define BSWAP_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

// Mac OS X / Darwin features
#include <libkern/OSByteOrder.h>
#define BSWAP_16(x) OSSwapInt16(x)
#define BSWAP_32(x) OSSwapInt32(x)
#define BSWAP_64(x) OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define BSWAP_16(x) BSWAP_16(x)
#define BSWAP_32(x) BSWAP_32(x)
#define BSWAP_64(x) BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define BSWAP_16(x) bswap16(x)
#define BSWAP_32(x) bswap32(x)
#define BSWAP_64(x) bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define BSWAP_16(x) swap16(x)
#define BSWAP_32(x) swap32(x)
#define BSWAP_64(x) swap64(x)

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <machine/bswap.h>
#if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#define BSWAP_16(x) bswap16(x)
#define BSWAP_32(x) bswap32(x)
#define BSWAP_64(x) bswap64(x)
#endif

#else
#include <byteswap.h>
#define BSWAP_16(x) bswap_16(x)
#define BSWAP_32(x) bswap_32(x)
#define BSWAP_64(x) bswap_64(x)
#endif

namespace ip_address
{
	inline uint16_t ByteSwap(uint16_t x)
	{
		return BSWAP_16(x);
	}

	inline uint32_t ByteSwap(uint32_t x)
	{
		return BSWAP_32(x);
	}

	inline uint64_t ByteSwap(uint64_t x)
	{
		return BSWAP_64(x);
	}

	inline uint16_t HostToNet16(uint16_t x)
	{
		return ByteSwap(x);
	}

	inline uint32_t HostToNet32(uint32_t x)
	{
		return ByteSwap(x);
	}

	inline uint64_t HostToNet64(uint64_t x)
	{
		return ByteSwap(x);
	}

	inline uint16_t NetToHost16(uint16_t x)
	{
		return ByteSwap(x);
	}

	inline uint32_t NetToHost32(uint32_t x)
	{
		return ByteSwap(x);
	}

	inline uint64_t NetToHost64(uint64_t x)
	{
		return ByteSwap(x);
	}

#undef BSWAP_16
#undef BSWAP_32
#undef BSWAP_64
}
