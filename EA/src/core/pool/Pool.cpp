/*
 * Pool.cpp
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 */

#include "Pool.h"
#include "../../Common.h"
#include "GenomePool.h"
#include "OrganismPool.h"

namespace ea {

/**
 * @class Pool
 * The interface for list of objects, which can be stored in a Population.
 * This interface contains nothing. It only serves as the general interface for something which can be stored
 * in a Population. Please check GenomePool and OrganismPool for real implementations.
 *
 * @see GenomePool
 * @see OrganismPool
 */

Pool::~Pool() {
}

} /* namespace ea */
