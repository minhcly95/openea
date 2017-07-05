/*
 * TestInterfaces.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "TestInterfaces.h"

namespace ea {

namespace test {

using namespace std;

// TestRecombinator
TestRecombinator::TestRecombinator(uint pParentCount) :
		mParentCount(pParentCount) {
}

uint TestRecombinator::GetParentCount() {
	return mParentCount;
}

GenomePtr TestRecombinator::DoCombine(vector<GenomePtr>& pParents) {
	return make_shared_base(Genome, TestGenome);
}

// TestMutator
GenomePtr TestMutator::DoApply(const GenomePtr& pTarget) {
	return make_shared_base(Genome, TestGenome);
}

// TestInitializer
TestInitializer::TestInitializer(int pFakeSize) :
		mFakeSize(pFakeSize) {
}

VectorPtr<GenomePtr> TestInitializer::DoInitialize(uint pSize) {
	VectorPtr<GenomePtr> pool = make_shared<vector<GenomePtr>>();
	if (mFakeSize >= 0)
		pSize = (uint) mFakeSize;
	for (uint i = 0; i < pSize; i++)
		pool->push_back(make_shared_base(Genome, TestGenome));
	return pool;
}

void TestInitializer::SetFakeSize(int fakeSize) {
	mFakeSize = fakeSize;
}

// TestSelector
TestSelector::TestSelector(int pFakeSize) :
		mFakeSize(pFakeSize) {
}

OrganismPoolPtr TestSelector::DoSelect(
		const OrganismPoolPtr& pPool, uint pTargetSize) {
	OrganismPoolPtr pool = make_shared<OrganismPool>();
	if (mFakeSize >= 0)
		pTargetSize = (uint) mFakeSize;
	for (uint i = 0; i < pTargetSize; i++)
		pool->push_back((*pPool)[0]);
	return pool;
}

void TestSelector::SetFakeSize(int fakeSize) {
	mFakeSize = fakeSize;
}

// TestGenome
ostream& TestGenome::Print(ostream& os) const {
	os << "<TestGenome>";
	return os;
}

// TestEvaluator
TestEvaluator::TestEvaluator(double pTargetFitness) :
		mTargetFitness(make_shared<ScalarFitness>(pTargetFitness)) {
}

void TestEvaluator::SetTargetFitness(double targetFitness) {
	mTargetFitness = make_shared<ScalarFitness>(targetFitness);
}

FitnessPtr TestEvaluator::DoEvaluate(const GenomePtr& pGenome) {
	return mTargetFitness;
}

}	// namespace test

}	// namespace ea
