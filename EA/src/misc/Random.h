/*
 * Random.h
 *
 *  Created on: Apr 12, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../Common.h"

namespace ea {

using namespace std;

class Random {
private:
	static uniform_real_distribution<double> sRate;
	static normal_distribution<double> sNormal;

public:
	static default_random_engine generator;

	static void Seed(llong seed);
	static void SeedByNow();

	static inline double Rate() {
		return sRate(generator);
	}
	static inline double Normal(double sigma = 1) {
	return sNormal(generator) * sigma;
	}

	template<class T>
	static inline pair<T, T> Pair(T lower, T upper) {
		uniform_int_distribution<T> dist(lower, upper);

		T first = dist(Random::generator), second;
		do
			second = dist(Random::generator);
		while (second == first);

		return make_pair(first, second);
	}

	template<class T>
	static inline pair<T, T> OrderedPair(T lower, T upper) {
		pair<T, T> pair = Pair(lower, upper);

		if (pair.first > pair.second)
			swap(pair.first, pair.second);

		return pair;
	}

	static inline bool Bool() {
		return generator() & 1;
	}
};

} /* namespace ea */


