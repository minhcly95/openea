/*
 * UnifiedData.h
 *
 *  Created on: Apr 18, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../Common.h"
#include "../EA/Type/Utility.h"
#include "Serializer.h"
#include <boost/variant.hpp>
#include "../core/Operator.h"
#include "../core/OperatorGroup.h"
#include "../core/SeriesOperatorGroup.h"

namespace ea {

struct UnifiedData {
private:
	boost::variant<string, ConstructiblePtr, vector<ConstructiblePtr>> data;
public:
	UnifiedData(string pStringData);
	UnifiedData(const ConstructiblePtr& pObjData);
	UnifiedData(const vector<ConstructiblePtr>& pListData);

	template<class T>
	inline UnifiedData(Ptr<T> pElemData) :
			UnifiedData(static_pointer_cast<Constructible>(pElemData)) {
		static_assert(is_base_of<Constructible, T>::value,
				"T must be a child of Constructible.");
	}

	~UnifiedData();

	bool IsStringData() const;
	bool IsObjectData() const;
	bool IsListData() const;

private:
	template<class T>
	struct Getter {
		const UnifiedData *parent;
		inline Getter(const UnifiedData *parent) :
				parent(parent) {
		}
		inline T Get() const {
			switch (parent->data.which()) {
			case 0:
				return Serializer<T>::Parse(boost::get<string>(parent->data));
			default:
				throw EA_EXCEPTION(RTOCException, WRONG_DATA_CATEGORY,
						"UnifiedData cannot cast object or list data into non-pointer type.");
			}
		}
	};
	template<class T>
	struct Getter<Ptr<T>> {
		const UnifiedData *parent;
		inline Getter(const UnifiedData *parent) :
				parent(parent) {
		}
		inline Ptr<T> Get() const {
			Ptr<T> casted;
			switch (parent->data.which()) {
			case 0:
				return Serializer<Ptr<T>>::Parse(boost::get<string>(parent->data));
			case 1:
				casted = dynamic_pointer_cast<T>(boost::get<ConstructiblePtr>(parent->data));
				if (!casted)
					throw EA_EXCEPTION(RTOCException, WRONG_OBJ_TYPE,
							"UnifiedData cannot cast the object data into the given type.");
				return casted;
			default:
				throw EA_EXCEPTION(RTOCException, WRONG_DATA_CATEGORY,
						"UnifiedData cannot cast list data into non-vector type.");
			}
		}
	};
	template<class T>
	struct Getter<vector<Ptr<T>>> {
		const UnifiedData *parent;
		inline Getter(const UnifiedData *parent) :
				parent(parent) {
		}
		inline vector<Ptr<T>> Get() const {
			Ptr<T> casted;
			vector<ConstructiblePtr> list;
			vector<Ptr<T>> castedList;
			switch (parent->data.which()) {
			case 0:
				return Serializer<vector<Ptr<T>>>::Parse(boost::get<string>(parent->data));
			case 1:
				casted = dynamic_pointer_cast<T>(boost::get<ConstructiblePtr>(parent->data));
				if (!casted)
					throw EA_EXCEPTION(RTOCException, WRONG_OBJ_TYPE,
							"UnifiedData cannot cast the data into the given value type.");
				return { casted };
			case 2:
				list = boost::get<vector<ConstructiblePtr>>(parent->data);
				castedList.resize(list.size());
				std::transform(list.begin(), list.end(), castedList.begin(), [&] (const ConstructiblePtr& uncasted) {
					Ptr<T> casted = dynamic_pointer_cast<T>(uncasted);
					if (!casted)
						throw EA_EXCEPTION(RTOCException, WRONG_OBJ_TYPE,
								"UnifiedData cannot cast the data #" + to_string(&uncasted - &list[0])
								+ " in the list into the given type.");
					return casted;
				});
				return castedList;
			default:
				throw EA_EXCEPTION(RTOCException, WRONG_DATA_CATEGORY,
						"UnifiedData cannot categorize the stored data.");
			}
		}
	};

	// Specialization for Operator and OperatorGroup
	template<class T>
	struct Getter<Operator<T>> {
		const UnifiedData *parent;
		inline Getter(const UnifiedData *parent) :
				parent(parent) {
		}
		inline Operator<T> Get() const {
			return Operator<T>(Getter<Ptr<T>>(parent).Get());
		}
	};
	template<class T>
	struct Getter<OperatorGroup<T>> {
		const UnifiedData *parent;
		inline Getter(const UnifiedData *parent) :
				parent(parent) {
		}
		inline OperatorGroup<T> Get() const {
			return OperatorGroup<T>(Getter<vector<Ptr<T>>>(parent).Get());
		}
	};
	template<class T>
	struct Getter<SeriesOperatorGroup<T>> {
		const UnifiedData *parent;
		inline Getter(const UnifiedData *parent) :
				parent(parent) {
		}
		inline SeriesOperatorGroup<T> Get() const {
			return SeriesOperatorGroup<T>(Getter<vector<Ptr<T>>>(parent).Get());
		}
	};

public:
	template<class T>
	inline T Get() const {
		return Getter<T>(this).Get();
	}
};

} /* namespace ea */
