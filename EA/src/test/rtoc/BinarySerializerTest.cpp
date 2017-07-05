/*
 * BinarySerializerTest.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_DYN_LINK
#include "../../pch.h"
#include <boost/test/unit_test.hpp>

#include "../../EA.h"
#include "TestConstructibles.h"
#include "../core/TestInterfaces.h"

namespace ea {

namespace test {

BOOST_AUTO_TEST_SUITE(RTOCTest)

BOOST_AUTO_TEST_SUITE(BinarySerializerTest)

#define SIZE_TEST(CLASS, VALUE, SIZE) {\
		ostringstream os;\
		BinarySerializer<CLASS>::Write(os, VALUE);\
		BOOST_CHECK(os.str().size() == SIZE);\
	}

BOOST_AUTO_TEST_CASE(PrimitiveTypeTest) {
	SIZE_TEST(bool, false, 1);
	SIZE_TEST(int, 0, 4);
	SIZE_TEST(uint, 0, 4);
	SIZE_TEST(llong, 0, 8);
	SIZE_TEST(ullong, 0, 8);
	SIZE_TEST(float, 0, 4);
	SIZE_TEST(double, 0, 8);

	string str = "Hello World!";
	SIZE_TEST(string, str, str.length() + 1);
}

BOOST_AUTO_TEST_CASE(StorableTest) {
	NameService::Add<TestGenome>();

	shared_ptr<TestGenome> genome = make_shared<TestGenome>();
	genome->mValue = 20;
	ostringstream os;
	BinarySerializer<shared_ptr<TestGenome>>::Write(os, genome);
	BOOST_CHECK(os.str().size() == genome->GetTypeName().length() + 5);
	BOOST_CHECK(memcmp(os.str().c_str(), "TestGenome\0", 11) == 0);

	istringstream is(os.str());
	auto genome_copy = BinarySerializer<shared_ptr<TestGenome>>::Read(is);
	BOOST_CHECK(genome_copy->mValue == 20);
}

BOOST_AUTO_TEST_CASE(OrganismPtrTest) {
	Ptr<TestGenome> genome = make_shared<TestGenome>();
	Ptr<TestEvaluator> evaluator = make_shared<TestEvaluator>();
	OrganismPtr org = evaluator->Evaluate(genome);
	ostringstream os;
	BinarySerializer<OrganismPtr>::Write(os, org);
	BOOST_CHECK(
			os.str().size()
					== genome->GetTypeName().length()
							+ ScalarFitness::TypeInfo().GetTypeName().length()
							+ 6 + 9);
}

#define SIZE_VECTOR_TEST(CLASS, VALUE, LENGTH, SIZE) {\
		ostringstream os;\
		BinarySerializer<vector<CLASS>>::Write(os, vector<CLASS>(LENGTH, VALUE));\
		BOOST_CHECK(os.str().size() == SIZE);\
	}

BOOST_AUTO_TEST_CASE(VectorTest) {
	SIZE_VECTOR_TEST(bool, false, 100, 104);
	SIZE_VECTOR_TEST(int, 0, 100, 404);
	SIZE_VECTOR_TEST(uint, 0, 100, 404);
	SIZE_VECTOR_TEST(llong, 0, 100, 804);
	SIZE_VECTOR_TEST(ullong, 0, 100, 804);
	SIZE_VECTOR_TEST(float, 0, 100, 404);
	SIZE_VECTOR_TEST(double, 0, 100, 804);

	string str = "Hello World!";
	SIZE_VECTOR_TEST(string, str, 100, (str.length() + 1) * 100 + 4);

	Ptr<TestGenome> genome = make_shared<TestGenome>();
	SIZE_VECTOR_TEST(shared_ptr<TestGenome>, genome, 100,
			(genome->GetTypeName().length() + 5) * 100 + 4);

	Ptr<TestEvaluator> evaluator = make_shared<TestEvaluator>();
	OrganismPtr org = evaluator->Evaluate(genome);
	SIZE_VECTOR_TEST(OrganismPtr, org, 100,
			(genome->GetTypeName().length()
					+ ScalarFitness::TypeInfo().GetTypeName().length() + 6
					+ 9) * 100 + 4);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}// namespace test

}// namespace ea

