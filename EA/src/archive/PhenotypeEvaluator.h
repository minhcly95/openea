/*
 * PhenotypeEvaluator.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "TypedScalarEvaluator.h"

namespace ea {

template<class P, class G>
class PhenotypeDecoder {
public:
	using Phenotype = P;
	using PhenotypePtr = Ptr<P>;
	using Genotype = G;
	using GenotypePtr = Ptr<G>;

	static PhenotypePtr Decode(const GenotypePtr& pGenome);
};

template<class P>
class PhenotypeEvaluator: public IndividualEvaluator {
public:
	using InputType = const Ptr<P>&;
	using FunctionType = function<FitnessPtr(InputType)>;

	inline virtual ~PhenotypeEvaluator() {
	}

private:
	virtual FitnessPtr DoEvaluate(InputType pPheno) = 0;
	virtual FitnessPtr DoEvaluate(IndividualEvaluator::InputType pGenome) override {
		Ptr<P> pheno = PhenotypeDecoder<P, Genome>::Decode(pGenome);
		return DoEvaluate(pheno);
	}
};

template<class P, class G>
class TypedPhenotypeEvaluator: public TypedScalarEvaluator<G> {
public:
	using InputType = const Ptr<P>&;
	using FunctionType = function<FitnessPtr(InputType)>;

	inline virtual ~TypedPhenotypeEvaluator() {
	}

private:
	virtual FitnessPtr DoEvaluate(InputType pPheno) = 0;
	virtual FitnessPtr DoEvaluate(typename TypedScalarEvaluator<G>::InputType pGenome) override {
		Ptr<P> pheno = PhenotypeDecoder<P, G>::Decode(pGenome);
		return DoEvaluate(pheno);
	}
};

}
