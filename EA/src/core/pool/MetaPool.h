/*
 * MetaPool.h
 *
 *  Created on: Jun 20, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "Pool.h"
#include "../../rtoc/Constructible.h"

namespace ea {

using namespace std;

class MetaPool : public Pool, public map<uint, PoolPtr> {
public:
	EA_TYPEINFO_DEFAULT(MetaPool)

	MetaPool();
	virtual ~MetaPool();

protected:
	virtual void DoSerialize(ostream& pStream) const override;
	virtual void DoDeserialize(istream& pStream) override;
};

} /* namespace ea */
