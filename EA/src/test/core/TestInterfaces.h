/*
 * TestInterfaces.h
 *
 *  Created on: Mar 21, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA.h"

#define NO_FAKE_SIZE -1

namespace ea {

namespace test {

class TestRecombinator: public Recombinator {
public:
	EA_TYPEINFO_DEFAULT(TestRecombinator)

	TestRecombinator(uint pParentCount = 2);
	inline virtual ~TestRecombinator() {
	}

	virtual uint GetParentCount() override;

private:
	uint mParentCount;
	virtual GenomePtr DoCombine(vector<GenomePtr>& pParents) override;
};

class TestMutator: public Mutator {
public:
	EA_TYPEINFO_DEFAULT(TestMutator)

	inline TestMutator() {
	}
	inline virtual ~TestMutator() {
	}

private:
	virtual GenomePtr DoApply(const GenomePtr& pTarget) override;
};

class TestInitializer: public Initializer {
public:
	EA_TYPEINFO_DEFAULT(TestInitializer)

	TestInitializer(int pFakeSize = NO_FAKE_SIZE);
	inline virtual ~TestInitializer() {
	}

	void SetFakeSize(int fakeSize);

private:
	int mFakeSize;
	virtual VectorPtr<GenomePtr> DoInitialize(uint pSize) override;
};

class TestSelector: public Selector {
public:
	EA_TYPEINFO_DEFAULT(TestSelector)

	TestSelector(int pFakeSize = NO_FAKE_SIZE);
	inline virtual ~TestSelector() {
	}

	void SetFakeSize(int fakeSize);

private:
	int mFakeSize;
	virtual OrganismPoolPtr DoSelect(OrganismPoolPtr const& pPool, uint pTargetSize) override;
};

class TestEvaluator: public IndividualEvaluator {
public:
	EA_TYPEINFO_DEFAULT(TestEvaluator)

	TestEvaluator(double pTargetFitness = 0);
	inline virtual ~TestEvaluator() {
	}

	void SetTargetFitness(double targetFitness);

private:
	FitnessPtr mTargetFitness;
	virtual FitnessPtr DoEvaluate(const GenomePtr& pGenome) override;
};

class TestGenome: public GenomeClonable<TestGenome> {
public:
	EA_TYPEINFO_DEFAULT(TestGenome)

	int mValue;

	inline TestGenome() :
			mValue(10) {
	}
	inline virtual ~TestGenome() {
	}

	virtual ostream& Print(ostream& os) const override;

	inline virtual void DoSerialize(ostream& pStream) const override {
		Write<int>(pStream, mValue);
	}
	inline virtual void DoDeserialize(istream& pStream) override {
		mValue = Read<int>(pStream);
	}
};

class TestHook: public Hook {
public:
	EA_TYPEINFO_DEFAULT(TestHook)

	uint generational, start, end, initial;

	inline TestHook() :
			generational(0), start(0), end(0), initial(0) {
	}
	inline virtual ~TestHook() {
	}

private:
	inline virtual void DoGenerational() override {
		generational++;
	}
	inline virtual void DoInitial() override {
		initial++;
	}
	inline virtual void DoStart() override {
		start++;
	}
	inline virtual void DoEnd() override {
		end++;
	}


};

}	// namespace test

}	// namespace ea


