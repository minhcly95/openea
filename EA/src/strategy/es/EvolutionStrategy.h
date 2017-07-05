/*
 * MuPlusLambdaStrategy.h
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../rtoc/Constructible.h"
#include "../../EA/Core.h"

#include "../../core/interface/Initializer.h"
#include "../../core/interface/Selector.h"
#include "../../core/interface/Hook.h"
#include "../../evaluator/IndividualEvaluator.h"
#include "../../utility/MetaMutator.h"
#include "../../utility/MetaRecombinator.h"

namespace ea {

class EvolutionStrategy: public Strategy {
public:
	EA_TYPEINFO_CUSTOM_DECL

	EvolutionStrategy(uint popSize);
	virtual ~EvolutionStrategy();

	Operator<Initializer> initializer;
	OperatorGroup<MetaRecombinator> recombinators;
	SeriesOperatorGroup<MetaMutator> mutators;
	Operator<IndividualEvaluator> evaluator;
	Operator<ResizableSelector> survivalSelector;

	enum SelectionMode {
		PLUS,	///< Merge new Organism with the old pool (\f$(\mu+\lambda)\f$-ES). EAML: \tt{"+"} or \tt{"plus"}.
		COMMA	///< Discard the old pool before selection (\f$(\mu,\lambda)\f$-ES). EAML: \tt{","} or \tt{"comma"}.
	};
	void SetSelectionMode(SelectionMode pMode);
	SelectionMode GetSelectionMode() const;

	virtual bool IsReady() override;

protected:
	virtual void Setup() override;
	virtual void Begin() override;
	virtual void Loop() override;

	virtual vector<string> GetTimeRecordOrder() const override;

private:
	uint mPopSize;
	SelectionMode mSelectionMode;
};

} /* namespace ea */
