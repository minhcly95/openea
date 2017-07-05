/*
 * Storable.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "Storable.h"
#include "NameService.h"

namespace ea {

/**
 * @class Storable
 * Base class for classes which can be stored in binary form.
 * All classes which can be serialized to and deserialized from a binary data stream should be a child
 * of this class. Child classes should override DoSerialize() and DoDeserialize() to implement their own
 * serialization format. This enables the ability to store data to the file system or send it between processes.
 * Genome, Fitness, Organism, Pool, and Population are children of this class.
 *
 * Child classes which will be stored should override GetTypeInfo() function (inherit from Constructible)
 * to provide the type information such as class name. Moreover, only for child classes of Storable, they must also define
 * a default constructor (constructor with no parameters), so the module can have an entry point for the deserialization process.
 *
 * @see GetTypeInfo()
 * @see DoSerialize()
 * @see DoDeserialize()
 */

Storable::~Storable() {
}

/**
 * @fn void Storable::DoSerialize(ostream& pStream)
 * Implementation of serialization method.
 * Child classes should override this function to implement their own serialization method.
 * If this is not overriden, then default behaviour is doing nothing (that means this function
 * doesn't need to be overriden for classes which don't require saving any data except for class identity).
 * @param pStream The output stream to write to.
 */

/**
 * Public method of DoSerialize().
 * This function will call DoSerialize() internally.
 * @param pStream The output stream to write to.
 */
void Storable::Serialize(ostream& pStream) const {
	this->DoSerialize(pStream);
}

/**
 * @fn void Storable::DoDeserialize(istream& pStream)
 * Implementation of deserialization method.
 * Child classes should override this function to implement their own deserialization method.
 * The deserialization method should match with the serialization one.
 * If this is not overriden, then default behaviour is doing nothing (that means this function
 * doesn't need to be overriden for classes which don't require saving any data except for class identity).
 * @param pStream The input stream to read from.
 */

/**
 * Public method of DoDeserialize().
 * This function will call DoDeserialize() internally.
 * @param pStream The input stream to read from.
 */
void Storable::Deserialize(istream& pStream) {
	this->DoDeserialize(pStream);
}

/**
 * @fn void Storable::Write(ostream& pStream, const T& pData)
 * Helper to call BinarySerializer::Write().
 * This function provides a shorthand method to call BinarySerializer::Write() when implementing
 * customized version of DoSerialize() function.
 * @tparam T The type of the data (auto-deduced).
 * @param pStream The output stream to write to.
 * @param pData The data to be serialized.
 */

/**
 * @fn const T Storable::Read(istream& pStream)
 * Helper to call BinarySerializer::Read().
 * This function provides a shorthand method to call BinarySerializer::Read() when implementing
 * customized version of DoDeserialize() function.
 * @tparam T The type of the data.
 * @param pStream The input stream to read from.
 * @return The data has been deserialized.
 */

} /* namespace ea */
