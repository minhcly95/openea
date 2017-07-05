/*
 * Random.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: Bui Quang Minh
 */

#include "Random.h"
#include <chrono>

namespace ea {

/**
 * @class Random
 * Static class which provides a static random generator.
 * Instead of using multiple random generators at multiple places, using only one generator throughout
 * the system will enable the ability of resimulating an evolutional process if the seed is known.
 * Such static Random class also serves for convenience purposes.
 */

/**
 * The static random generator.
 * This random generator is created with type of std::default_random_engine.
 * The generator is automatically seeded by the current time using the high resolution clock.
 * For function which requires a random generator such as std::shuffle(), please use this
 * generator to ensure the resimulation ability.
 */
default_random_engine Random::generator(std::chrono::system_clock::now().time_since_epoch().count());
uniform_real_distribution<double> Random::sRate(0, 1);
normal_distribution<double> Random::sNormal(0, 1);

/**
 * Seed the Random::generator by the given number.
 * @param seed The seed for the Random::generator.
 */
void Random::Seed(llong seed) {
	generator = default_random_engine(seed);
}

/**
 * Seed the Random::generator by the current time using the high resolution clock.
 * This is the easiest way to obtain a random initial seed.
 */
void Random::SeedByNow() {
	generator = default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
}

/**
 * @fn double Random::Rate()
 * Generate a random rate between 0.0 and 1.0 inclusively.
 * This function will use the static Random::generator to generate the number.
 * @return A random rate between 0.0 and 1.0 inclusively.
 */

/**
 * @fn bool Random::Bool()
 * Generate a random boolean value.
 * This function will use the static Random::generator to generate the value.
 * @return A random boolean value which has 50% of drawing true and 50% of drawing false.
 */

/**
 * @fn pair<T, T> Random::Pair(T lower, T upper)
 * Generate a random pair of integers.
 * This function will use the static Random::generator to generate the values.
 * The return result is a pair of random integer numbers.
 * The values will be bound by the given limits inclusively.
 * The two numbers are guaranteed to be different.
 *
 * @tparam T The type of the result (must be an integer type).
 * @param lower The lower bound of the values.
 * @param upper The upper bound of the values.
 * @return A pair of random values.
 */

/**
 * @fn pair<T, T> Random::OrderedPair(T lower, T upper)
 * Generate a random pair of integers with the first one always smaller.
 * Similar to Pair() function but
 * the first number is guaranteed to be smaller than the second one.
 *
 * @tparam T The type of the result (must be an integer type).
 * @param lower The lower bound of the values.
 * @param upper The upper bound of the values.
 * @return A pair of random values whose first number is always smaller.
 */

} /* namespace ea */

