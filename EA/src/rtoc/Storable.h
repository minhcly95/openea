/*
 * Storable.h
 *
 *  Created on: Apr 5, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "StorableDef.h"
#include "BinarySerializer.h"

namespace ea {

using namespace std;

template<class T>
inline void Storable::Write(ostream& pStream, const T& pData) {
	BinarySerializer<T>::Write(pStream, pData);
}

template<class T>
inline const T Storable::Read(istream& pStream) {
	return BinarySerializer<T>::Read(pStream);
}

template<class T>
inline void Storable::Read(istream& pStream, T& pData) {
	BinarySerializer<T>::Read(pStream, pData);
}

} /* namespace ea */


