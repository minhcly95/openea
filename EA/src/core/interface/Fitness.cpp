/*
 * Fitness.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../../pch.h"
#include "Fitness.h"

namespace ea {

using namespace std;

/**
 * @class Fitness
 * The interface for general fitness.
 * This class only serves as the base class of fitness specialization. It also defines a
 * comparison method between Fitness object (see Compare() function) and useful overloaded operators.
 *
 * For implementation of Fitness for single scalar fitness value, see ScalarFitness.
 *
 * @see ScalarFitness
 */

/**
 * @fn int Fitness::Compare(const Fitness& other)
 * Compare between two Fitness objects.
 * This function must be overriden in child classes to provide a proper way to compare two fitness values.
 *
 * @param other The object that **this** will be compared to.
 * @return The result of comparison. Negative if this is worse than other,
 * positive if this is better and 0 if neither better nor worse.
 *
 * @see operator <()
 * @see operator >()
 * @see operator ==()
 */

Fitness::~Fitness() {
}

/**
 * Whether **this** is worse than other.
 * This function will test the Compare() function for negative value.
 *
 * @param other The object that **this** will be compared to.
 * @return true if **this** is worse than other.
 */
bool Fitness::operator <(const Fitness& other) const {
	return this->Compare(other) < 0;
}
/**
 * Whether **this** is as good as other.
 * This function will test the Compare() function for 0.
 *
 * @param other The object that **this** will be compared to.
 * @return true if **this** is as good as other.
 */
bool Fitness::operator ==(const Fitness& other) const {
	return this->Compare(other) == 0;
}
/**
 * Whether **this** is better than other.
 * This function will test the Compare() function for positive value.
 *
 * @param other The object that **this** will be compared to.
 * @return true if **this** is better than other.
 */
bool Fitness::operator >(const Fitness& other) const {
	return this->Compare(other) > 0;
}

/**
 * Version of Fitness::operator<() which works on pointers.
 *
 * @param f1 The first object to be compared.
 * @param f2 The second object to be compared.
 * @return true if f1 is worse than f2.
 */
bool operator <(const FitnessPtr& f1, const FitnessPtr& f2) {
	return *f1 < *f2;
}

/**
 * Version of Fitness::operator>() which works on pointers.
 *
 * @param f1 The first object to be compared.
 * @param f2 The second object to be compared.
 * @return true if f1 is better than f2.
 */
bool operator >(const FitnessPtr& f1, const FitnessPtr& f2) {
	return *f1 > *f2;
}

ostream& Fitness::Print(ostream& os) const {
	os << "<Fitness>";
	return os;
}

ostream& operator <<(ostream& os, const Fitness& fitness) {
	return fitness.Print(os);
}

} /* namespace ea */

