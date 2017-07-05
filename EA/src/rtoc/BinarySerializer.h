/*
 * BinarySerializer.h
 *
 *  Created on: Apr 5, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Utility.h"
#include "NameService.h"
#include "StorableDef.h"
#include <algorithm>

namespace ea {

using namespace std;

template<class T>
class BinarySerializer {
public:
	inline static void Write(ostream& pStream, const T pData) {
		pStream.write(reinterpret_cast<const char*>(&pData), sizeof(T));
	}
	inline static const T Read(istream& pStream) {
		T pData;
		pStream.read(reinterpret_cast<char*>(&pData), sizeof(T));
		return pData;
	}
};

#ifndef DOXYGEN_IGNORE
template<>
class BinarySerializer<string> {
public:
	inline static void Write(ostream& pStream, const string pData) {
		pStream.write(pData.c_str(), pData.length() + 1);
	}
	inline static const string Read(istream& pStream) {
		string pData;
		getline(pStream, pData, '\0');
		return pData;
	}
};


template<class T>
class BinarySerializer<atomic<T>> {
public:
	inline static void Write(ostream& pStream, const atomic<T>& pData) {
		BinarySerializer<T>::Write(pStream, pData);
	}
	inline static void Read(istream& pStream, const atomic<T>& pData) {
		pData = Read(pStream);
	}
	inline static const T Read(istream& pStream) {
		return BinarySerializer<T>::Read(pStream);
	}
};

template<class T>
class BinarySerializer<Ptr<T>> {
public:
	inline static void Write(ostream& pStream, const Ptr<T>& pPtr) {
		static_assert(is_base_of<Storable, T>::value,
				"BinarySerializer<Ptr>::Write<T>(): Input object must be Storable.");

		StorablePtr storable = static_pointer_cast<Storable>(pPtr);

		if (!is_final<T>::value)
			BinarySerializer<string>::Write(pStream, string(storable->GetTypeName()));

		storable->Serialize(pStream);
	}
	template<class T_ = T>
	inline static const Ptr<T> Read(istream& pStream,
			enable_if_t<!is_final<T_>::value, int> = 0) {
		static_assert(is_base_of<Storable, T>::value,
				"BinarySerializer<Ptr>::Read<T>(): Request object type must be derived from Storable.");

		string className = BinarySerializer<string>::Read(pStream);

		StorablePtr storable = dynamic_pointer_cast<Storable>(NameService::Get(className).Construct({ }));
		if (!storable)
			throw EA_EXCEPTION(RTOCException, BINARY_READ_BAD_CAST,
					"BinarySerializer<Ptr>::Read<T>(): Class \"" + className + "\" is not Storable.");

		Ptr<T> obj = dynamic_pointer_cast<T>(storable);
		if (!obj)
		  throw EA_EXCEPTION(RTOCException, BINARY_READ_BAD_CAST,
			  "BinarySerializer<Ptr>::Read<T>(): Cannot cast the object into T.");

		obj->Deserialize(pStream);
		return obj;
	}
	template<class T_ = T>
	inline static const Ptr<T> Read(istream& pStream,
			enable_if_t<is_final<T_>::value, float> = 0) {
		static_assert(is_default_constructible<T>::value,
				"BinarySerializer<Ptr>::Read<T>(): T is final but doesn't have a default constructor.");

		Ptr<T> obj = make_shared<T>();
		obj->Deserialize(pStream);
		return obj;
	}
};

template<class T>
class BinarySerializer<vector<T>> {
public:
	inline static void Write(ostream& pStream, const vector<T>& pData) {
		BinarySerializer<uint>::Write(pStream, pData.size());
		for (const T& elem : pData) {
			BinarySerializer<T>::Write(pStream, elem);
		}
	}
	inline static void Read(istream& pStream, vector<T>& pData) {
		uint size = BinarySerializer<uint>::Read(pStream);
		pData.resize(size);

		for (auto it = pData.begin(); it != pData.end(); it++)
			*it = BinarySerializer<T>::Read(pStream);
	}
	inline static const vector<T> Read(istream& pStream) {
		vector<T> data;
		Read(pStream, data);
		return data;
	}
};

template<class MapT>
class BinarySerializerMapBase {
private:
	using K = typename MapT::key_type;
	using V = typename MapT::mapped_type;

public:
	inline static void Write(ostream& pStream, const MapT& pData) {
		BinarySerializer<uint>::Write(pStream, pData.size());
		for (const typename MapT::value_type& elem : pData) {
			BinarySerializer<K>::Write(pStream, elem.first);
			BinarySerializer<V>::Write(pStream, elem.second);
		}
	}
	inline static void Read(istream& pStream, MapT& pData) {
		pData.clear();

		uint size = BinarySerializer<uint>::Read(pStream);
		for (uint i = 0; i < size; i++) {
			auto key = BinarySerializer<K>::Read(pStream);
			auto value = BinarySerializer<V>::Read(pStream);
			pData.emplace(piecewise_construct, forward_as_tuple(key), forward_as_tuple(value));
		}
	}
	inline static const MapT Read(istream& pStream) {
		MapT data;
		Read(pStream, data);
		return data;
	}
};

template<class K, class V>
class BinarySerializer<map<K, V>> : public BinarySerializerMapBase<map<K, V>> { };

template<class K, class V>
class BinarySerializer<unordered_map<K, V>> : public BinarySerializerMapBase<unordered_map<K, V>> { };
#endif

}	// namespace ea

