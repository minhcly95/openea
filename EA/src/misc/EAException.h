/*
 * EAException.h
 *
 *  Created on: Mar 31, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../Common.h"
#include <exception>

namespace ea {

using namespace std;

/**
 * Create an exception with file name and line number included in the message.
 * Users should use this macro to produce exceptions rather than creating them directly
 * in order to produce more informative error messages.
 * @param EXCP The exception type (could be EAException, RTOCException or ConfigFileException).
 * @param CODE The enum value of the error code (must exist in the ErrorCode enum of corresponding exception).
 * @param MSG The message of the exception.
 * @param ... Other optional parameters for the constructor of exception (usually the inner exception).
 *
 * Example:
 *
 * @code
 * throw EA_EXCEPTION(RTOCException, CLASS_NAME_REGISTERED, "This class name is already registered.");
 * @endcode
 */
#define EA_EXCEPTION(EXCP, CODE, MSG, ...)\
	EXCP(EXCP::CODE, ea::EAException::Reduce(__FILE__) + ":"\
			+ std::to_string( __LINE__) + " : " #CODE " - " + string(MSG), ##__VA_ARGS__)

#define EA_EXCEPTION_WRAP(EXCP, CODE, MSG, CMD)\
	try {\
		CMD;\
	} catch (EAException& e) {\
		throw EA_EXCEPTION(EXCP, CODE, MSG, std::current_exception());\
	}

class EAException: public std::exception {
public:
	enum ErrorCode {
		OTHERS = 0x00,						///< General error.
		POPULATION_NOT_INITIALIZED = 0x01,	///< Population was not initialized yet and is not ready to be initialized.
		STRATEGY_NOT_READY,					///< Strategy is not ready to Strategy::Evolve().
		WRONG_CALL_ORDER,					///< Functions are called in wrong order.
		ID_DOES_NOT_EXIST,					///< The given ID does not exist.
		ARGUMENT_REQUIRED,					///< Function requires some arguments.
		SELECTOR_ERROR = 0x10,				///< Error when executing Selector::DoSelect().
		VARIATOR_ERROR,						///< Error when executing Variator::DoVariate().
		RECOMBINATOR_ERROR,					///< Error when executing Recombinator::DoCombine().
		MUTATOR_ERROR,						///< Error when executing Mutator::DoApply().
		INITIALIZER_ERROR,					///< Error when executing Initializer::DoInitialize().
		EVALUATOR_ERROR,					///< Error when executing Evaluator::DoEvaluate().
		HOOK_ERROR,							///< Error when executing custom code in Hook.
		PROXY_SELECTOR_FAILED = 0x20,		///< Implementation of Selection returns inconsistent pool size.
		PROXY_VARIATOR_FAILED,				///< Implementation of Variator returns inconsistent pool size.
		PROXY_RECOMBINATOR_FAILED,			///< Implementation of Recombinator returns inconsistent parent count.
		PROXY_INITIALIZER_FAILED,			///< Implementation of Initializer returns inconsistent pool size.
		PROXY_EVALUATOR_FAILED,				///< Implementation of Evaluator returns inconsistent pool size.
		PROXY_HOOK_FAILED,					///< Hook is not connected to the Population.
		PROXY_STRATEGY_FAILED,				///< Strategy is not connected to the Population.
		META_MUTATOR_INVALID_RATE = 0x30,	///< The mutation rate is out of range (from 0 to 1)
		META_RECOMBINATOR_INVALID_RATIO,	///< The offspring ratio is negative.
		EXISTED_FILE_IS_NOT_DIRECTORY = 0x40,	///< Provided path is not a directory.
		EXISTED_FILE_IS_NOT_REGULAR_FILE,	///< Provided path is not a regular file.
		FILE_DOES_NOT_EXIST,				///< Provided path doesn't exist.
		FILE_ALREADY_EXISTS,				///< Provided path already exists.
		CANNOT_CREATE_FILE,					///< Cannot create file for provided path.
		WRONG_FILE_FORMAT,					///< Format of the file content is wrong.
		FILE_FEATURE_NOT_SUPPORT,			///< Some features of file is not supported yet.
		CONSTRUCTIBLE_BAD_CAST = 0x50,		///< Cannot cast the Constructible object into the given type.
		TYPED_OPERATOR_BAD_CAST,			///< This Typed-operator doesn't support the type of input arguments.
		FITNESS_BAD_CAST,					///< The Fitness types are different (required to be the same when comparison)
		FITNESS_INCOMPATIBLE,				///< Fitness types are the same but in different context (e.g. maximizer vs minimizer)
		POOL_BAD_CAST,						///< Cannot cast the Pool into the given type.
		OPERATOR_BAD_CAST,					///< Cannot cast the given type to required operator.
		CLUSTER_CANNOT_DEPLOY = 0x60,		///< Cluster cannot deploy because there are not enough nodes in the cluster.
		CLUSTER_OPERATOR_NOT_LOADED,		///< Cluster slave node didn't load the operator (wrong cluster protocol).
		STRATEGY_PARALLEL_OP_FAILED = 0x70,	///< The number of input pools is not the same as the number of operators in the group.
		EVALUTOR_GROUP_EMPTY,				///< The evaluator group is empty while invoking.
		SESSION_DEPENDENT,					///< This operator depends on particular Session and must be invoked via Operator wrapper.
	};

	EAException(int pCode, string pMessage, exception_ptr pInner = nullptr);
	virtual ~EAException();

	int GetCode() const;
	const exception_ptr& GetInnerException() const;
	const string& GetMessage() const;

	virtual const char* what() const noexcept override;

	static string Reduce(string fileName);

private:
	int mCode;
	string mMessage;
	exception_ptr mInner;
};

class RTOCException: public EAException {
public:
	enum ErrorCode {
		CLASS_NAME_REGISTERED = 0x100,			///< This class name has been registered in the NameService.
		CLASS_NAME_NOT_EXIST,					///< This class name doesn't exist in the NameService.
		NAME_SERVICE_ADD_BAD_CAST,				///< This class name cannot be casted in the requested type.
		FIELD_NAME_REGISTERED = 0x110,			///< This field name has been registered in the TypeInfo.
		FIELD_MISSING,							///< Constructor defined in TypeInfo requires some more data.
		CONSTRUCTOR_MISSING,					///< This TypeInfo has not defined any constructor.
		FIELD_NOT_EXIST,						///< This field name doesn't exist in the TypeInfo.
		NOT_RUNTIME_CONSTRUCTED_OBJ = 0x120,	///< A RTOC feature has been used on a non-runtime-constructed object.
		DESERIALIZE_FAILED = 0x130,				///< No deserialization method provided.
		WRONG_OBJ_TYPE,							///< Object data cannot be casted into the given type.
		WRONG_DATA_CATEGORY,					///< This UnifiedData contains another category of data (string or object).
		BINARY_WRITE_BAD_CAST = 0x140,			///< Bad casting happened when calling BinarySerializer::Write().
		BINARY_READ_BAD_CAST,					///< Bad casting happened when calling BinarySerializer::Read().
		STORABLE_NO_TYPE_NAME					///< This Storable class doesn't define a type name.
	};

	RTOCException(int pCode, string pMessage, exception_ptr pInner = nullptr);
	virtual ~RTOCException();
};

class ConfigFileException: public EAException {
public:
	enum ErrorCode {
		LOADING_FILE_FAILED = 0x200, 	///< EAML file cannot be read (not exist or in wrong format).
		STRATEGY_NOT_READY,		 	///< The Population defined in EAML file is not completed.
		EAML_NO_NAMED_ATTR = 0x210,	 	///< EAML file is missing an required attribute.
		EAML_NO_NAMED_ELEM,			 	///< EAML file is missing an required element.
		EAML_NO_CHILD,				 	///< An EAML tag doesn't have a required child.
		EAML_WRONG_ROOT,			 	///< The EAML file doesn't begin with Population tag.
		EAML_WRONG_TYPE,			 	///< EAML file gives wrong class name for a required slot.
		EAML_ATTR_NOT_VALID = 0x220	 	///< The value of an attribute is not valid.
	};

	ConfigFileException(int pCode, string pMessage, exception_ptr pInner =
			nullptr);
	virtual ~ConfigFileException();
};

} /* namespace ea */


