/*
 * Fitness.h
 *
 *  Created on: Apr 24, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../../rtoc/Storable.h"
#include "../../rtoc/BinarySerializer.h"

namespace ea {

class Fitness : public Storable {
public:
	virtual ~Fitness();

	virtual int Compare(const Fitness& other) const = 0;

	virtual ostream& Print(ostream& os) const;

	bool operator <(const Fitness& other) const;
	bool operator ==(const Fitness& other) const;
	bool operator >(const Fitness& other) const;
};

bool operator <(const FitnessPtr& f1, const FitnessPtr& f2);
bool operator >(const FitnessPtr& f1, const FitnessPtr& f2);

ostream& operator <<(ostream& os, const Fitness& genome);

} /* namespace ea */
