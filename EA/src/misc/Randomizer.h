/*
 * Randomizer.h
 *
 *  Created on: Mar 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../rtoc/Constructible.h"
#include <ctime>

namespace ea {

using namespace std;

/**
 * Class to store data for randomization process such as limits.
 * This class provides a method to configure the upper limit and lower limit for RandomArrayInitializer.
 * This class can also be reused for PointResetMutation.
 * Moreover, the virtual Get() function enables the ability to derive from default classes and define new
 * random method such as biased randomizer.
 * @tparam T The output type.
 * @name{BoolRandomizer, IntRandomizer, DoubleRandomizer}
 *
 * @eaml
 * @attr{lower,
 * Type T - Required for \ref IntRandomizer and \ref DoubleRandomizer -
 * The lower bound of generated value (inclusive).}
 * @attr{upper,
 * Type T - Required for \ref IntRandomizer and \ref DoubleRandomizer -
 * The upper bound of generated value (inclusive).}
 * @endeaml
 */
template<class T>
class Randomizer: public Constructible {
public:
	EA_TYPEINFO_CUSTOM_DECL

	inline Randomizer() {
	}
	inline virtual ~Randomizer() {
	}

	/**
	 * Get a random value of type T.
	 * Default implementation will call the Random() function of type T if there is any.
	 * Otherwise it is a compile-time error (because Random() is not defined).
	 * Custom classes which want to use the Randomizer should provide a Random() function
	 * or specialize the whole Randomizer class.
	 * @return A random value of type T.
	 */
	virtual inline T Get() const {
		return T::Random();
	}
};

/**
 * Get a random boolean value (specialization of Randomizer<T>::Get() for bool).
 * This will use the static Random::generator to generate the value.
 * @return A random boolean value.
 */
template<>
inline bool Randomizer<bool>::Get() const {
	return Random::Bool();
}
EA_TYPEINFO_SPECIALIZED_DEFAULT(BoolRandomizer)

/**
 * Specialization of Randomizer for int.
 * Randomizer for int requires also a upper limit and a lower limit,
 * therefore it needs a whole class specialization.
 *
 * @name{IntRandomizer}
 *
 * @eaml
 * @attr{lower,
 * int - Required -
 * The lower bound of generated value (inclusive).}
 * @attr{upper,
 * int - Required -
 * The upper bound of generated value (inclusive).}
 * @endeaml
 */
template<>
class Randomizer<int> : public Constructible {
private:
	int mLower, mUpper;

public:
	/**
	 * Create a Randomizer for int with the given limits (inclusively).
	 * @param pLower The lower limits.
	 * @param pUpper The upprt limits.
	 */
	inline Randomizer(int pLower, int pUpper) :
			mLower(pLower), mUpper(pUpper) {
	}
	inline virtual ~Randomizer() {
	}

	/**
	 * Get a random integer value between the constructed limits inclusively.
	 * This will use the static Random::generator to generate the value.
	 * @return A random integer value.
	 */
	virtual inline int Get() const {
		static uniform_int_distribution<int> dist(mLower, mUpper);
		return dist(Random::generator);
	}

	EA_TYPEINFO_CUSTOM_INLINE {
		return *ea::TypeInfo("IntRandomizer")
			.SetConstructor<IntRandomizer, int, int>("lower", "upper");
	}
	EA_TYPEINFO_GET
};

/**
 * Specialization of Randomizer for double.
 * Randomizer for double requires also a upper limit and a lower limit,
 * therefore it needs a whole class specialization.
 *
 * @name{DoubleRandomizer}
 *
 * @eaml
 * @attr{lower,
 * double - Required -
 * The lower bound of generated value (inclusive).}
 * @attr{upper,
 * double - Required -
 * The upper bound of generated value (inclusive).}
 * @endeaml
 */
template<>
class Randomizer<double> : public Constructible {
private:
	double mLower, mUpper;

public:
	/**
	 * Create a Randomizer for double with the given limits (inclusively).
	 * @param pLower The lower limits.
	 * @param pUpper The upprt limits.
	 */
	inline Randomizer(double pLower, double pUpper) :
			mLower(pLower), mUpper(pUpper) {
	}
	inline virtual ~Randomizer() {
	}

	/**
	 * Get a random double value between the constructed limits inclusively.
	 * This will use the static Random::generator to generate the value.
	 * @return A random double value.
	 */
	virtual inline double Get() const {
		static uniform_real_distribution<double> dist(mLower, mUpper);
		return dist(Random::generator);
	}

	EA_TYPEINFO_CUSTOM_INLINE {
		return *ea::TypeInfo("DoubleRandomizer")
			.SetConstructor<DoubleRandomizer, double, double>("lower", "upper");
	}
	EA_TYPEINFO_GET
};

}	// namespace ea


