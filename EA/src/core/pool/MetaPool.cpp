/*
 * MetaPool.cpp
 *
 *  Created on: Jun 20, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "MetaPool.h"

namespace ea {

/**
 * @class MetaPool
 * Data structure which aggregates other Pool into a map.
 * This class inherits directly from map<uint, PoolPtr> to provide a way to store multiple Pool
 * under a common Pool. Using this Pool, users can build a hierarchy of Pool inside the Population
 * (MetaPool can also contain other MetaPool).
 */

MetaPool::MetaPool() : map<uint, PoolPtr>() {
}

MetaPool::~MetaPool() {
}

void MetaPool::DoSerialize(ostream& pStream) const {
	Write(pStream, map<uint, PoolPtr>(*this));
}

void MetaPool::DoDeserialize(istream& pStream) {
	Read(pStream, static_cast<map<uint, PoolPtr>&>(*this));
}

} /* namespace ea */
