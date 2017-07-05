/*
 * StorableDef.h
 *
 *  Created on: Apr 5, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Core.h"
#include "Constructible.h"

namespace ea {

using namespace std;

class Storable : public Constructible {
public:
	virtual ~Storable();

	void Serialize(ostream& pStream) const;
	void Deserialize(istream& pStream);

protected:
	template <class T>
	static void Write(ostream& pStream, const T& pData);
	template <class T>
	static const T Read(istream& pStream);
	template <class T>
	static void Read(istream& pStream, T& pData);

	inline virtual void DoSerialize(ostream& pStream) const { };
	inline virtual void DoDeserialize(istream& pStream) { };
};

} /* namespace ea */


