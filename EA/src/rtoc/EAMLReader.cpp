/*
 * EAMLReader.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "EAMLReader.h"

#include "../EA.h"

/**
 * @class std::hash<ea::EAMLReader::Variable>
 * Specialization of std::hash to provide hashing method for ea::EAMLReader::Variable.
 * The hashing method will be used in the ea::EAMLReader::VariableList which takes the hash value
 * to check the existence of a key in the list.
 */

namespace ea {

/**
 * @class EAMLReader
 * Reader for the EAML file format.
 * This class provides a set of functions to load an EAML configuration file and reconstruct a Population
 * from that file. To use the reader, simply create the reader by calling the constructor EAMLReader()
 * with the file name and then call Build() to reconstruct the Population.
 *
 * @see EAMLReader()
 * @see Build()
 */

/**
 * @class EAMLReader::Variable
 * Data structure to store the variable name and its default value.
 * This structure is used in the GetVariableList() function.
 * @see GetVariableList()
 */

/**
 * @var EAMLReader::Variable::key
 * The name of the variable.
 * This is the key used to reference when building the EAMLReader::VariableMap used for Build() function.
 */

/**
 * @var EAMLReader::Variable::defaultValue
 * The default value of the variable.
 * This is the default value for the variable if the variable is not mentioned
 * in the EAMLReader::VariableMap used for Build() function.
 */

using namespace std;
using namespace tinyxml2;

/**
 * Create a reader for the given file name.
 * This constructor also loads the file for the given path. If the file doesn't exist or has any problem
 * when reading, an exception will be thrown.
 * @param fileName The path to EAML file (absolute or relative).
 */
EAMLReader::EAMLReader(const char* fileName) :
		mDoc(), mConstructed() {
	XMLError e = mDoc.LoadFile(fileName);
	if (e != XML_SUCCESS)
		throw EA_EXCEPTION(ConfigFileException, LOADING_FILE_FAILED,
				"Error when loading file, code = " + to_string(e));
}

EAMLReader::~EAMLReader() {
	mDoc.Clear();
}

/**
 * @fn PopulationPtr EAMLReader::Build()
 * Reconstruct a Population from the EAML file using default value for variables.
 * This function will call Build(const VariableMap& map) internally with an empty \ref VariableMap.
 * @return The reconstructed Population.
 */

/**
 * Reconstruct a Population from the EAML file with the given \ref VariableMap.
 * This function uses the NameService and TypeInfo routines to reconstruct whole a Population
 * only from configuration file. If the file doesn't follow the format or misses some operators,
 * exception will be thrown.
 *
 * The \ref VariableMap will contain a map of Variable keys and new values. These values
 * will be used instead of the default values defined in the EAML file.
 *
 * @param map The \ref VariableMap containing pairs of key and value.
 * @return The reconstructed Population.
 */
StrategyPtr EAMLReader::Build(const VariableMap& map) {
	StrategyPtr strategy = Construct<Strategy>(mDoc.FirstChildElement(), map);

	if (!strategy->IsReady())
		throw EA_EXCEPTION(ConfigFileException, STRATEGY_NOT_READY,
				"EAMLReader::Build(): Strategy is not ready! Operators are missing.");

	return static_pointer_cast<Strategy>(strategy);
}

ConstructiblePtr EAMLReader::Construct(const XMLElement* elem,
		const VariableMap& map) {
	const char* className = elem->Name();

	TypeInfo::UnifiedMap dataMap;
	for (const XMLAttribute* attr = elem->FirstAttribute(); attr;
			attr = attr->Next()) {
		dataMap.emplace(piecewise_construct, forward_as_tuple(attr->Name()),
				forward_as_tuple(AttrValue(attr->Value(), map)));
	}

	for (const XMLElement* child = elem->FirstChildElement(); child; child = child->NextSiblingElement()) {
		vector<ConstructiblePtr> objList;

		for (const XMLElement* obj = child->FirstChildElement(); obj; obj =	obj->NextSiblingElement()) {
			objList.push_back(Construct(obj, map));
		}

		if (objList.size() == 1)
			dataMap.emplace(piecewise_construct, forward_as_tuple(child->Name()),
					forward_as_tuple(objList[0]));
		else
			dataMap.emplace(piecewise_construct, forward_as_tuple(child->Name()),
					forward_as_tuple(objList));
	}

	ConstructiblePtr obj = NameService::Get(className).Construct(dataMap);
	mConstructed.push_back(obj);
	return obj;
}

string EAMLReader::AttrValue(string str, const VariableMap& map) {
	vector<string> tokens;
	boost::split(tokens, str, boost::is_any_of("|"));

	string input = tokens[0];
	if (tokens.size() > 2)
		throw EA_EXCEPTION(ConfigFileException, EAML_ATTR_NOT_VALID,
				"EAMLReader::AttrValue: Too many vertical bars \"|\" in attribute value.");

	if (tokens.size() == 2) {
		auto itr = map.find(tokens[1]);
		if (itr != map.end())
			input = itr->second;
	}

	return input;
}

/**
 * Query all the Variable existed in the EAML file.
 * This is a recurrent function. Normal uses should call it without any parameter.
 * @param elem Normal uses should leave this blank. This is the parameter used for recurrent invocation.
 * @return The \ref VariableList containing a list of available Variable.
 */
EAMLReader::VariableList EAMLReader::GetVariableList(const XMLElement* elem) {
	if (elem == nullptr)
		elem = mDoc.RootElement();

	VariableList list = make_shared<unordered_set<Variable>>();

	for (const XMLAttribute* attr = elem->FirstAttribute(); attr;
			attr = attr->Next()) {
		vector<string> tokens;
		string attrValue = attr->Value();
		boost::split(tokens, attrValue, boost::is_any_of("|"));

		if (tokens.size() > 2)
			throw EA_EXCEPTION(ConfigFileException, EAML_ATTR_NOT_VALID,
					"EAMLReader::GetKeyList: Too many vertical bars \"|\" in attribute value.");

		if (tokens.size() == 2)
			list->insert(Variable(tokens[1], tokens[0]));
	}

	for (const XMLElement* child = elem->FirstChildElement(); child; child =
			child->NextSiblingElement()) {
		VariableList childList = GetVariableList(child);
		list->insert(childList->begin(), childList->end());
	}

	return list;
}

const vector<ConstructiblePtr>& EAMLReader::GetConstructedObjects() const {
	return mConstructed;
}

} /* namespace ea */
