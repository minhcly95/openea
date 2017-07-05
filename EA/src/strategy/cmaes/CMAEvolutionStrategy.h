/*
 * CMAES.h
 *
 *  Created on: Jun 6, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../core/interface/Strategy.h"
#include "CMAStatePool.h"
#include "../../evaluator/IndividualEvaluator.h"
#include <eigen3/Eigen/Dense>

namespace ea {

using namespace Eigen;

class CMAEvolutionStrategy : public Strategy {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	CMAEvolutionStrategy(uint pDimension, double pStepSize = 0.3);
	virtual ~CMAEvolutionStrategy();

	uint GetLambda() const;
	void SetLambda(uint pLambda);

	uint GetMu() const;
	void SetMu(uint pMu);

	Operator<IndividualEvaluator> evaluator;

	virtual bool IsReady() override;

protected:
	virtual void Setup() override;
	virtual void Begin() override;
	virtual void Loop() override;

	virtual vector<string> GetTimeRecordOrder() const override;

private:
	uint mLambda;
	uint mMu;
	CMAStatePool mState;

	uint& N;
	double& sigma;
	MatrixXd& B;
	VectorXd& D;
	VectorXd& mean, &ps, &pc;

	VectorXd weights;
	double mueff, cc, cs, c1, cmu, damps, chiN;

	VectorXd RandomVector();
	void Decompose(const MatrixXd& C);
};

} /* namespace ea */
