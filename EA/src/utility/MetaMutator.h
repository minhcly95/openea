/*
 * MetaMutator.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../rtoc/Constructible.h"
#include "../core/interface/Variator.h"

namespace ea {

class MetaMutator : public Variator {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	MetaMutator(const MutatorPtr& pMutator, double pRate = 1);
	virtual ~MetaMutator();

	double GetRate() const;
	const MutatorPtr& GetMutator() const;

	void SetRate(double pRate);
	void SetMutator(const MutatorPtr& pMutator);

	template <class M, class... Args>
	MetaMutator(Args&&... args) : mRate(1) {
		mMutator = make_shared<M>(forward<Args>(args)...);
	}
	void Rate(double pRate);

protected:
	virtual GenomePoolPtr DoVariate(const GenomePoolPtr& pPool) override;

private:
	MutatorPtr mMutator;
	double mRate;
};

} /* namespace ea */
