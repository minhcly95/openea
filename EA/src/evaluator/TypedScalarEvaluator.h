/*
 * TypedEvaluator.h
 *
 *  Created on: Apr 15, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "ScalarEvaluator.h"
#include "../core/interface/Genome.h"

namespace ea {

/**
 * A ScalarEvaluator which will cast the input Genome into type T automatically.
 * It is usually the case that users know which type of Genome they are processing. Therefore,
 * to avoid putting too much responsibility of type-casting on users, this class will automatically
 * cast the input Genome of ScalarEvaluator::DoScalarEvaluate(const GenomePtr&) into a genome type of type T.
 * Users will override the new function with the same name \ref DoScalarEvaluate(InputType) instead of
 * the old one.
 *
 * If users want to create a TypedScalarEvaluator in-place without writing a new child class, see TypedFunctionalEvaluator.
 *
 * @tparam T Must be a child of Genome.
 */
template<class T>
class TypedScalarEvaluator: public ScalarEvaluator {
public:
	using InputType = const Ptr<T>&;
	using FunctionType = function<double(InputType)>;

	inline virtual ~TypedScalarEvaluator() {
	}

protected:
	/**
	 * Implementation of the evaluation method with input of type T.
	 * This is the new implementation point of the evaluation method. The input will be auto-casted
	 * to type T and the function only requires an input of type \ref InputType (= const \ref Ptr<T>&).
	 * @param pGenome The input genome of type T to be evaluated.
	 * @return A scalar value of type double representing the fitness value.
	 */
	virtual double DoScalarEvaluate(InputType pGenome) = 0;

	/**
	 * Specialized implementation to be compatible with ScalarEvaluator.
	 * This function will cast the input and call DoScalarEvaluate(InputType pGenome) internally.
	 * This function also provides an informative error message if bad cast happened.
	 * @see ScalarEvaluator::DoScalarEvaluate()
	 */
	virtual double DoScalarEvaluate(const GenomePtr& pGenome) override final {
		shared_ptr<T> casted = dynamic_pointer_cast<T>(pGenome);
		if (!casted)
			throw EA_EXCEPTION(EAException, TYPED_OPERATOR_BAD_CAST,
					GetTypeNameSafe("This TypedEvaluator") + " requires " + T().GetTypeName()
							+ " as input.");
		return DoScalarEvaluate(casted);
	}
};

/**
 * A TypedScalarEvaluator which accepts an std::function as the evaluation function.
 * TypedFunctionalEvaluator allows users creating an in-place TypedScalarEvaluator without write a new child class.
 * This class is an alternative of FunctionalEvaluator if users know the type of Genome in advance.
 *
 * @tparam T Must be a child of Genome.
 * @see FunctionalEvaluator.
 */
template<class T>
class TypedFunctionalEvaluator: public TypedScalarEvaluator<T> {
private:
	bool mMaximizer;
	typename TypedScalarEvaluator<T>::FunctionType mFunc;

protected:
	/**
	 * Implementation of TypedScalarEvaluator::DoScalarEvaluate(InputType).
	 * This function will invoke the given function at the constructor with the given genome of type T.
	 * @see TypedScalarEvaluator::DoScalarEvaluate(InputType)
	 */
	inline virtual double DoScalarEvaluate(
			typename TypedScalarEvaluator<T>::InputType pGenome) {
		return mFunc(pGenome);
	}

public:
	/**
	 * Create a TypedScalarEvaluator with the given function as the evaluation function.
	 * The given function will be invoked when Evaluate() is called.
	 * @param pFunc The evaluation function, which accepts a const \ref Ptr<T>& and returns a double.
	 * @param pMaximizer Whether this Evaluator is a maximizer (true) or a minimizer (false).
	 */
	inline TypedFunctionalEvaluator(
			typename TypedScalarEvaluator<T>::FunctionType pFunc, bool pMaximizer =
					true) :
			mMaximizer(pMaximizer), mFunc(pFunc) {
	}
	inline virtual ~TypedFunctionalEvaluator() {
	}

	inline virtual bool IsMaximizer() override {
		return mMaximizer;
	}
};

} /* namespace ea */
