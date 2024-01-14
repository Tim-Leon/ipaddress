#include <gtest/gtest.h>
#include "IPEndPoint.h"
#include <thread>         
using namespace ip_address;
TEST(IPAddressV4Test, parse)
{
	ip_address::IPAddressV4 ip1("172.217.21.142");
	/* parse and string representation */
	std::string s = ip1.getString();

	EXPECT_EQ(s, "172.217.21.142");
	/* IPv4 to IPv6 mapping */
	//EXPECT_TRUE(ip1.toIPv6().isIPv4Mapped());
}

TEST(IPAddressV4Test, Loopback)
{
	/* Loopback */
	ip_address::IPAddressV4 ip5("127.0.0.1");
	EXPECT_EQ(ip_address::IPAddressV4::loopback(), ip5);
	EXPECT_TRUE(ip5 == ip_address::IPAddressV4::loopback());
}

TEST(IPAddressV4Test, Any)
{
	/* Any */
	ip_address::IPAddressV4 ip6("0.0.0.0");
	EXPECT_TRUE(ip6.isAny());
	auto aip6 = ip_address::IPAddressV4::any();
	EXPECT_TRUE(aip6.isAny());
	EXPECT_TRUE(ip6.getString() == "0.0.0.0");
}

TEST(IPAddressV6Test, parse)
{

}

TEST(IPAddressV6Test, Loopback)
{
	/* Loopback */
	ip_address::IPAddressV6 ip1("::1");
	EXPECT_EQ(ip1, ip_address::IPAddressV6::loopback());
	EXPECT_TRUE(ip1.isLoopback());
}

TEST(IPAddressV6Test, Operator)
{
	/* Opeator */
	ip_address::IPAddressV6 ip1("::1");
	ip_address::IPAddressV6 ip2("::1");
	EXPECT_TRUE(ip1 == ip2);
	EXPECT_FALSE(ip1 != ip2);
}

void func1()
{
	IPEndPoint ipend(IPAddressV4("123.123.123.123"), 1001);
	ipend.asIPv4();
}

TEST(IPEndPoint, basic)
{

	std::thread thread(func1);
	IPEndPoint ipend(IPAddressV4("123.123.123.123"), 1001);
	ipend.asIPv4();
	ASSERT_DEATH({
	ipend.asIPv6();
		}, "Error on line .* of ipv6()");


	thread.join();

}
TEST(SanityTest, Sanity)
{
	IPAddressV4 ipv4;
	EXPECT_TRUE(ipv4.bytes().size() == sizeof(uint32_t));
}
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}