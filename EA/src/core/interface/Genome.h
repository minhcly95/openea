/*
 * Genome.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../EA/Type/Core.h"
#include <iostream>
#include "../../rtoc/Storable.h"

namespace ea {

using namespace std;

/**
 * The interface for representation.
 * This class provides the interface to implement custom representation.
 * It provides a pure virtual method to clone the Genome (see CloneBase()).
 *
 * However, if the custom representation doesn't require deep copy, users should
 * inherit GenomeClonable instead to have a default shallow-clone function. Otherwise,
 * users must write for themselves a cloning method.
 *
 * @see CloneBase()
 * @see GenomeClonable
 */
class Genome: public virtual Storable {
public:
	inline virtual ~Genome() {
	}

	/**
	 * Clone this Genome into another Genome object.
	 *
	 * The cloned version should be independent to the original,
	 * so any variation operator won't affect on both Genome.
	 *
	 * @return The cloned Genome.
	 */
	virtual GenomePtr CloneBase() const = 0;
	/**
	 * Print the Genome to an output stream.
	 *
	 * Child classes should implement this to provide meaningful output.
	 *
	 * @param os The output stream to be written to.
	 * @return The given output stream (to use cascading invocation).
	 */
	inline virtual ostream& Print(ostream& os) const {
		os << "<Genome>";
		return os;
	}
};

/**
 * Print a Genome to an output stream.
 *
 * This function will invoke Genome::Print() function internally.
 *
 * @param os The output stream to be written to.
 * @param genome The Genome which will be printed.
 * @return The given output stream (to use cascading invocation).
 */
inline ostream& operator <<(ostream& os, const Genome& genome) {
	return genome.Print(os);
}

/**
 * The specialization of Genome to provide default shallow-clone method.
 *
 * This class is designed by using the <a href="https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern">CRTP</a>.
 * Therefore, users should inherit this class by providing the child class as the template argument. For example:
 *
 * @code {.cpp}
 * class A : public GenomeClonable<A> { ... }
 * @endcode
 *
 * Users must notice that if the representation requires deep-copy, this class must not be inherited. Such classes must
 * inherit directly from Genome and override a version of Genome::CloneBase() for themselves.
 *
 * @tparam T The child class which inherits GenomeClonable.
 */
template<class T>
class GenomeClonable: public Genome {
public:
	/**
	 * Clone this Genome of class T into another Genome of class T.
	 * The difference between Clone() and CloneBase() is that
	 * Clone() return the object of class T directly while CloneBase() return only Genome object.
	 *
	 * @return The cloned Genome of class T.
	 */
	shared_ptr<T> Clone() const {
		return make_shared<T>(*static_cast<const T*>(this));
	}

	virtual GenomePtr CloneBase() const override
	{
		return make_shared_base(Genome, T, (*static_cast<const T*>(this)));
	}
};

} /* namespace ea */
