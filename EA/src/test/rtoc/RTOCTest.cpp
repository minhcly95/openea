/*
 * RTOCTest.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_DYN_LINK
#include "../../pch.h"
#include <boost/test/unit_test.hpp>

#include "../../EA.h"
#include "TestConstructibles.h"
#include "ConstructibleFixture.h"

namespace ea {

namespace test {

BOOST_AUTO_TEST_SUITE(RTOCTest)

BOOST_AUTO_TEST_SUITE(ConstructibleTest)

BOOST_FIXTURE_TEST_CASE(RegisterTest, ConstructibleFixture) {
	// No registration
	BOOST_CHECK_NO_THROW(NameService::Get("Servant1Constructible"));
	BOOST_CHECK_NO_THROW(NameService::Get("Servant2Constructible"));

	// Correct registration
	BOOST_CHECK_NO_THROW(NameService::Get("MasterConstructible"));

	// Double registration
	BOOST_CHECK_THROW(NameService::Get("DoubleRegAttrConstructible"),
			RTOCException);
	BOOST_CHECK_THROW(NameService::Get("DoubleRegElemConstructible"),
			RTOCException);
}

TypeInfo::UnifiedMap Merge(TypeInfo::UnifiedMap& m1, TypeInfo::UnifiedMap& m2) {
	TypeInfo::UnifiedMap newMap(m1);
	newMap.insert(m2.begin(), m2.end());
	return newMap;
}

BOOST_FIXTURE_TEST_CASE(ConstructTest, ConstructibleFixture) {
	ConstructiblePtr servant1 =
			NameService::Get("Servant1Constructible").Construct();
	ConstructiblePtr servant2 =
			NameService::Get("Servant2Constructible").Construct();
	TypeInfo master = NameService::Get("MasterConstructible");

	TypeInfo::UnifiedMap attrMap, emptyAttrMap, wrongKeyAttrMap,
			wrongValueAttrMap, elemMap, emptyElemMap, wrongKeyElemMap,
			wrongValueElemMap;

	attrMap = { {"attr", UnifiedData("10")}};
	wrongKeyAttrMap = { {"abcd", UnifiedData("10")}};
	wrongValueAttrMap = { {"attr", UnifiedData("abcd")}};

	elemMap = { {"servant", UnifiedData(servant1)}};
	wrongKeyElemMap = { {"xyz", UnifiedData(servant1)}};
	wrongValueElemMap = { {"servant", UnifiedData(servant2)}};

	// Empty map test
	BOOST_CHECK_THROW(master.Construct(Merge(emptyAttrMap, emptyElemMap)),
			RTOCException);
	BOOST_CHECK_THROW(master.Construct(Merge(attrMap, emptyElemMap)),
			RTOCException);
	BOOST_CHECK_THROW(master.Construct(Merge(emptyAttrMap, elemMap)),
			RTOCException);

	// Wrong key
	BOOST_CHECK_THROW(master.Construct(Merge(wrongKeyAttrMap, elemMap)),
			RTOCException);
	BOOST_CHECK_THROW(master.Construct(Merge(attrMap, wrongKeyElemMap)),
			RTOCException);

	// Wrong value
	BOOST_CHECK_THROW(master.Construct(Merge(wrongValueAttrMap, elemMap)),
			RTOCException);
	BOOST_CHECK_THROW(master.Construct(Merge(attrMap, wrongValueElemMap)),
			RTOCException);

	// Correct map
	MasterConstructiblePtr masterObj;
	BOOST_REQUIRE_NO_THROW(
			masterObj = dynamic_pointer_cast<MasterConstructible>(
					master.Construct(Merge(attrMap, elemMap))));
	BOOST_REQUIRE(masterObj);
	BOOST_CHECK(masterObj->GetAttr() == 10);
	BOOST_CHECK(masterObj->GetServant() == servant1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_CASE(NameServiceTest, ConstructibleFixture) {
// Double registration
	BOOST_REQUIRE_THROW(NameService::Add<Servant1Constructible>(),
			RTOCException);
	BOOST_CHECK_THROW(NameService::Add<Servant2Constructible>(), RTOCException);
}

#define SERIALIZER_TEST(CLASS, VALUE) Serializer<CLASS>::Parse(VALUE)

#define SERIALIZER_VALUE_TEST(CLASS, STR, VALUE)\
		BOOST_CHECK(Serializer<CLASS>::Parse(STR) == VALUE)

#define SERIALIZER_NO_THROW_TEST(CLASS, STR, VALUE)\
	BOOST_CHECK_NO_THROW(SERIALIZER_TEST(CLASS, STR));\
	SERIALIZER_VALUE_TEST(CLASS, STR, VALUE)

BOOST_AUTO_TEST_CASE(SerializerTest) {
	// bool
	SERIALIZER_NO_THROW_TEST(bool, "true", true);
	SERIALIZER_NO_THROW_TEST(bool, "false", false);
	SERIALIZER_NO_THROW_TEST(bool, "  false", false);
	SERIALIZER_NO_THROW_TEST(bool, "true  ", true);
	BOOST_CHECK_THROW(SERIALIZER_TEST(bool, "0"), RTOCException);
	BOOST_CHECK_THROW(SERIALIZER_TEST(bool, "1"), RTOCException);
	BOOST_CHECK_THROW(SERIALIZER_TEST(bool, "abcd"), RTOCException);

	// int
	SERIALIZER_NO_THROW_TEST(int, "0", 0);
	SERIALIZER_NO_THROW_TEST(int, "13  ", 13);
	SERIALIZER_NO_THROW_TEST(int, "  -54", -54);
	BOOST_CHECK_THROW(SERIALIZER_TEST(int, "0.5"), RTOCException);
	BOOST_CHECK_THROW(SERIALIZER_TEST(int, "-1.2"), RTOCException);
	BOOST_CHECK_THROW(SERIALIZER_TEST(int, "true"), RTOCException);
	BOOST_CHECK_THROW(SERIALIZER_TEST(int, "abcd"), RTOCException);

	// double
	SERIALIZER_NO_THROW_TEST(double, "0", 0);
	SERIALIZER_NO_THROW_TEST(double, "13  ", 13);
	SERIALIZER_NO_THROW_TEST(double, "  34", 34);
	SERIALIZER_NO_THROW_TEST(double, " 0.5 ", 0.5);
	SERIALIZER_NO_THROW_TEST(double, " -1.2		", -1.2);
	BOOST_CHECK_THROW(SERIALIZER_TEST(double, "false"), RTOCException);
	BOOST_CHECK_THROW(SERIALIZER_TEST(double, "abcd"), RTOCException);

	// string
	SERIALIZER_NO_THROW_TEST(string, "helloworld", "helloworld");
	SERIALIZER_NO_THROW_TEST(string, "hello world", "hello world");
	SERIALIZER_NO_THROW_TEST(string, "   hello world", "hello world");
	SERIALIZER_NO_THROW_TEST(string, " hello  world ", "hello  world");
	SERIALIZER_NO_THROW_TEST(string, " hello\nworld ", "hello\nworld");
}

BOOST_AUTO_TEST_SUITE_END()

}// namespace test

}// namespace ea
