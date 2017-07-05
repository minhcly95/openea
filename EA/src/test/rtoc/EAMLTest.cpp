/*
 * EAMLTest.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_DYN_LINK
#include "../../pch.h"
#include <boost/test/unit_test.hpp>

#include "../../EA.h"
#include "ConstructibleFixture.h"

namespace ea {

namespace test {

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(RTOCTest)

BOOST_AUTO_TEST_SUITE(EAMLTest)

template<class T, class U>
bool CheckType(const shared_ptr<U>& input) {
	shared_ptr<T> output = dynamic_pointer_cast<T>(input);
	return bool(output);
}

#define RESOURCE(FILE)  (std::string("src/test/rtoc/resources/") + FILE).c_str()

#define WRONG_TEMPLATE(FILE)\
	BOOST_REQUIRE_NO_THROW(reader = make_shared<EAMLReader>(RESOURCE(std::string("templates/") + FILE)));\
	BOOST_CHECK_THROW(reader->Build(), ConfigFileException);

#define CORRECT_TEMPLATE(FILE)\
	BOOST_REQUIRE_NO_THROW(reader = make_shared<EAMLReader>(RESOURCE(std::string("templates/") + FILE)));\
	BOOST_CHECK_NO_THROW(reader->Build());

BOOST_FIXTURE_TEST_CASE(TemplateTest, ConstructibleFixture) {
	EAMLReaderPtr reader;

	// Missing key elements
	WRONG_TEMPLATE("WrongTemplate0.eaml")
	WRONG_TEMPLATE("WrongTemplate1.eaml")
	WRONG_TEMPLATE("WrongTemplate2.eaml")
	WRONG_TEMPLATE("WrongTemplate3.eaml")
	CORRECT_TEMPLATE("Template4.eaml")
	WRONG_TEMPLATE("WrongTemplate5.eaml")
	WRONG_TEMPLATE("WrongTemplate6.eaml")

	// Not enough recombinator
	WRONG_TEMPLATE("WrongTemplate7.eaml")

	// Missing secondary elements
	WRONG_TEMPLATE("WrongTemplate8.eaml")
	WRONG_TEMPLATE("WrongTemplate9.eaml")
	WRONG_TEMPLATE("WrongTemplate10.eaml")
	WRONG_TEMPLATE("WrongTemplate11.eaml")
	CORRECT_TEMPLATE("Template12.eaml")

	// Correct template
	BOOST_REQUIRE_NO_THROW(
			reader = make_shared<EAMLReader>(RESOURCE("templates/CorrectTemplate.eaml")));

	PopulationPtr pop;
	BOOST_REQUIRE_NO_THROW(pop = reader->Build());
	BOOST_CHECK(CheckType<TestEvaluator>(pop->GetEvaluator()));

	BOOST_CHECK(CheckType<TestMutator>(pop->GetMutator(0)->GetMutator()));
	BOOST_CHECK(pop->GetMutator(0)->GetRate() == 0.4);

	BOOST_CHECK(
			CheckType<TestRecombinator>(
					pop->GetRecombinator(0)->GetRecombinator()));
	BOOST_CHECK(
			CheckType<TestSelector>(pop->GetRecombinator(0)->GetSelector()));
	BOOST_CHECK(pop->GetRecombinator(0)->GetRatio() == 0.7);

	BOOST_CHECK(CheckType<TestSelector>(pop->GetSurvivalSelector()));
}

#define WRONG_RECONSTRUCT(FILE, ERROR)\
	BOOST_REQUIRE_NO_THROW(reader = make_shared<EAMLReader>(RESOURCE(std::string("reconstruct/") + FILE)));\
	BOOST_CHECK_THROW(reader->Build(), ERROR);

BOOST_TEST_DECORATOR(*utf::depends_on("RTOCTest/EAMLTest/TemplateTest"))
BOOST_FIXTURE_TEST_CASE(ReconstructTest, ConstructibleFixture) {
	EAMLReaderPtr reader;

	// Attribute and element missing
	WRONG_RECONSTRUCT("WrongReconstruct0.eaml", RTOCException)
	WRONG_RECONSTRUCT("WrongReconstruct1.eaml", RTOCException)
	WRONG_RECONSTRUCT("WrongReconstruct2.eaml", ConfigFileException)

	// Wrong element class
	WRONG_RECONSTRUCT("WrongReconstruct3.eaml", RTOCException)
	WRONG_RECONSTRUCT("WrongReconstruct4.eaml", RTOCException)

	// Correct reconstruct
	BOOST_REQUIRE_NO_THROW(
			reader = make_shared<EAMLReader>(RESOURCE("reconstruct/CorrectReconstruct.eaml")));

	PopulationPtr pop;
	BOOST_REQUIRE_NO_THROW(pop = reader->Build());
	BOOST_CHECK(CheckType<MasterConstructible>(pop->GetEvaluator()));

	MasterConstructiblePtr master = dynamic_pointer_cast<MasterConstructible>(
			pop->GetEvaluator());
	BOOST_REQUIRE(bool(master));

	BOOST_CHECK(master->GetAttr() == 20);
	BOOST_CHECK(CheckType<Servant1Constructible>(master->GetServant()));
}

BOOST_TEST_DECORATOR(*utf::depends_on("RTOCTest/EAMLTest/ReconstructTest"))
BOOST_FIXTURE_TEST_CASE(VariableTest, ConstructibleFixture) {
	EAMLReaderPtr reader;

	reader = make_shared<EAMLReader>(
			RESOURCE("reconstruct/CorrectReconstruct.eaml"));

	PopulationPtr pop;
	pop = reader->Build( { { "newSize", "25" }, { "newValue", "35" } });
	BOOST_CHECK(pop->GetSize() == 25);

	MasterConstructiblePtr master = dynamic_pointer_cast<MasterConstructible>(
			pop->GetEvaluator());

	BOOST_CHECK(master->GetAttr() == 35);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}// namespace test

}// namespace ea
