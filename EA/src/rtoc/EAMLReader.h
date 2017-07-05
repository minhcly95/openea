/*
 * EAMLReader.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Utility.h"
#include "Serializer.h"

#include <tinyxml2.h>
#include <string>
#include <sstream>
#include <unordered_set>
#include <boost/algorithm/string.hpp>

namespace ea {

using namespace std;
using namespace tinyxml2;

class EAMLReader {
public:
	EAMLReader(const char* fileName);
	virtual ~EAMLReader();

	struct Variable {
		string key;
		string defaultValue;

		inline Variable(string key, string defaultValue) :
				key(key), defaultValue(defaultValue) {
		}
		bool operator==(const Variable& other) const {
			return this->key == other.key;
		}
	};

	using VariableMap = HashMap<string, string>;
	using VariableList = Ptr<unordered_set<Variable>>;

	VariableList GetVariableList(const XMLElement* elem = nullptr);

	StrategyPtr Build(const VariableMap& map);
	inline StrategyPtr Build() {
		VariableMap empty;
		return Build(empty);
	}

	const vector<ConstructiblePtr>& GetConstructedObjects() const;

private:
	XMLDocument mDoc;
	vector<ConstructiblePtr> mConstructed;

	string AttrValue(string str, const VariableMap& map);

	template<class T>
	inline T Convert(const char* str, const VariableMap& map) {
		return Serializer<T>::Parse(AttrValue(str, map));
	}

	ConstructiblePtr Construct(const XMLElement* elem, const VariableMap& map);

	template<class T>
	inline shared_ptr<T> Construct(const XMLElement* elem,
			const VariableMap& map) {
		shared_ptr<T> result = dynamic_pointer_cast<T>(Construct(elem, map));
		if (!result)
			throw EA_EXCEPTION(ConfigFileException, EAML_WRONG_TYPE,
					"This XML element cannot be casted into the right type.");
		return result;
	}
};

} /* namespace ea */

namespace std {

template <>
struct hash<ea::EAMLReader::Variable>
{
	std::size_t operator()(const ea::EAMLReader::Variable& k) const
	{
		return hash<string>()(k.key);
	}
};

}


