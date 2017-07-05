/*
 * BinarySerializer.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"
#include "BinarySerializer.h"
#include "../core/Organism.h"

namespace ea {

using namespace std;

/**
 * @class BinarySerializer
 * Static class provide serialize and de-serialize method in binary form.
 * This class provides a binary (de)serialization method so object can be stored in file system
 * or transfered to another process. If users plan to serialize a custom class, there are two ways to achieve this:
 *
 * - Define a custom specialization of BinarySerializer. This is recommended if the type is trivial like enum or simple struct.
 * - For complex classes, inherit Storable and override Storable::DoSerialize() and Storable::DoDeserialize() function,
 * as well as the Constructible::GetTypeInfo() (by using type info macros).
 * By this way, class information for polymorphism could be stored by using the registered class name in TypeInfo.
 * Note that in order to deserialize the object, the TypeInfo must be added to the NameService by calling NameService::Add<T>().
 *
 * Supported types include primitive types, string, vector of supported types, and children of Storable.
 * @tparam The type to be (de)serialized.
 */

/**
 * @fn void BinarySerializer::Write(ostream& pStream, const T pData)
 * Implementation of serialization method.
 * If this is not specialized, then default behaviour is using the default binary form of the class stored in the main memory
 * (suitable for primitive type and simple struct).
 * @param pStream The output stream to write to.
 * @param pData The data to be serialized.
 */

/**
 * @fn const T BinarySerializer::Read(istream& pStream)
 * Implementation of deserialization method.
 * If this is not specialized, then default behaviour is using the default binary form of the class stored in the main memory
 * (suitable for primitive type and simple struct).
 * @param pStream The input stream to read from.
 * @return The data has been deserialized.
 */

} // namespace ea
