/*
 * UnifiedData.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Bui Quang Minh
 */

#include "UnifiedData.h"

namespace ea {

/**
 * @class UnifiedData
 * Unified data structure to store both string and Constructible object type.
 * Value of an attribute could be simple as a string or be complex as another Constructible object.
 * Therefore, an unified data structure will encapsulate the difference of nature and provide a
 * simple view to the data. To create an unified data struct, simply construct it with a string,
 * a Constructible object or a list of Constructible.
 *
 * @see UnifiedData(string)
 * @see UnifiedData(const ConstructiblePtr&)
 * @see UnifiedData(const vector<ConstructiblePtr>&)
 */

/**
 * Create an unified struct which represents a string.
 * This will also set IsStringData() to true.
 * @param pStringData The string value.
 */
UnifiedData::UnifiedData(string pStringData) :
		data(pStringData) {
}
/**
 * Create an unified struct which represents a Constructible object.
 * This will set IsObjectData() to true.
 * @param pObjData The object reference.
 */
UnifiedData::UnifiedData(const ConstructiblePtr& pObjData) :
		data(pObjData) {
}
/**
 * Create an unified struct which represents a vector of Constructible object.
 * This will set IsListData() to true.
 * @param pListData The list of objects.
 */
UnifiedData::UnifiedData(const vector<ConstructiblePtr>& pListData) :
		data(pListData) {
}

/**
 * @fn UnifiedData::UnifiedData(Ptr<T> pElemData)
 * Create an unified struct which represents an object of type T.
 * This function is an alternative version of UnifiedData(const ConstructiblePtr&) with uncasted-input.
 * @tparam T Must be a child of Constructible.
 * @param pElemData The object reference.
 */

/**
 * Whether this struct represents an EAML string data (attribute data).
 * @return true if this struct represents an attribute data (a string).
 */
bool UnifiedData::IsStringData() const {
	return data.which() == 0;
}
/**
 * Whether this struct represents an EAML object data (element data).
 * @return true if this struct represents an element data (an object).
 */
bool UnifiedData::IsObjectData() const {
	return data.which() == 1;
}
/**
 * Whether this struct represents an EAML list data.
 * @return true if this struct represents a list data.
 */
bool UnifiedData::IsListData() const {
	return data.which() == 2;
}

/**
 * @fn T UnifiedData::Get()
 * Get the data of the unified struct.
 * The type of the output is decided by T. T is not necessary to be string or \ref ConstructiblePtr.
 * It could be any type, and the output will be casted to T automatically. If there is no possible casting method,
 * an exception will be thrown.
 * @return The data of the UnifiedData.
 */

UnifiedData::~UnifiedData() {
}

} /* namespace ea */
