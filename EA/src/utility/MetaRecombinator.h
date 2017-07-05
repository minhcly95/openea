/*
 * MetaRecombinator.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../EA/Type/Selector.h"
#include "../rtoc/Constructible.h"
#include "../selector/UniformSelection.h"

namespace ea {

class MetaRecombinator : public Constructible {
public:
	using InputType = OrganismPoolPtr;
	using OutputType = GenomePoolPtr;

	EA_TYPEINFO_CUSTOM_DECL;

	MetaRecombinator(const RecombinatorPtr& pVariator,const ResizableSelectorPtr& pSelector, double pRate);
	virtual ~MetaRecombinator();

	double GetRatio() const;
	const ResizableSelectorPtr& GetSelector() const;
	const RecombinatorPtr& GetRecombinator() const;

	void SetRatio(double ratio);
	void SetRecombinator(const RecombinatorPtr& recombinator);
	void SetSelector(const ResizableSelectorPtr& selector);

	template <class R, class... Args>
	MetaRecombinator(Args&&... args) : mRatio(1) {
		mRecombinator = make_shared<R>(forward<Args>(args)...);
		mSelector = make_shared<UniformSelection>();
	}
	MetaRecombinator(const RecombinatorPtr& pVariator);
	template <class S, class... Args>
	MetaRecombinator* Parent(Args&&... args) {
		mSelector = make_shared<S>(forward<Args>(args)...);
		return this;
	}
	void Ratio(double pRatio);

	GenomePoolPtr operator ()(const SessionPtr& pSession, OrganismPoolPtr pPool) const;

private:
	RecombinatorPtr mRecombinator;
	ResizableSelectorPtr mSelector;
	double mRatio;
};

} /* namespace ea */
