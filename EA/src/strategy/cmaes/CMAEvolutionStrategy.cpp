/*
 * CMAES.cpp
 *
 *  Created on: Jun 6, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "CMAEvolutionStrategy.h"
#include "../../core/interface/Strategy.h"
#include "../../core/pool/GenomePool.h"
#include "../../core/pool/OrganismPool.h"
#include "../../EA/Type/Array.h"
#include "../../genome/ArrayGenome.h"
#include "../../selector/GreedySelection.h"

namespace ea {

/**
 * @class CMAEvolutionStrategy
 * An implementation of the <b>(μ/μw,λ)-CMA-ES</b> algorithm.
 * This class implements the \f$(\mu/\mu_w,\lambda)\f$ Covariance Matrix Adaptation Evolution %Strategy (\f$(\mu/\mu_w,\lambda)\f$-CMA-ES) algorithm.
 * The algorithm is used to optimized a vector of double (#DoubleArrayGenome) according to the input Evaluator.
 * The execution followed this order:
 * - [S] Spawning phase: \f$\lambda\f$ new samples are drawn from the mean and the covariance matrix.
 * - [E] Evaluation phase: Evaluator is applied to each sample.
 * - [U] Update phase: Update the state of the algorithm using only \f$\mu\f$ best Organism.
 * - [D] Decomposition phase: Decompose the matrix to get the inverse square root matrix.
 *
 * The numbering of Pool of this Strategy is:
 * - Main pool: OrganismPool #0 containing evaluated Organism with size of \f$\lambda\f$ (unfiltered, but sorted).
 * - State pool: CMAStatePool #2 containing the state of the algorithm (mean, covariance matrix, etc.).
 *
 * To add operators into the DefaultStrategy, use these class fields:
 * - #evaluator
 * - #hooks
 *
 * @name{CMAEvolutionStrategy}
 *
 * @eaml
 * @attr{dimension,
 * uint - Required - The number of dimensions}
 * @attr{step-size,
 * double - Required - The initial step size}
 * @attr{evaluator,
 * IndividualEvaluator - Required - The evaluation method}
 * @attr{hooks,
 * Hook - List\, Optional - List of global Hook}
 * @attr{lambda,
 * uint - Optional - The number of samples (offspring)}
 * @attr{mu,
 * uint - Optional - The number of Organism used to update the state (parent)}
 * @endeaml
 */

EA_TYPEINFO_CUSTOM_IMPL(CMAEvolutionStrategy) {
	return *ea::TypeInfo("CMAEvolutionStrategy")
		.Add("evaluator", &CMAEvolutionStrategy::evaluator)
		->Add("hooks", &CMAEvolutionStrategy::hooks)
		->Add("lambda", &CMAEvolutionStrategy::mLambda)
		->Add("mu", &CMAEvolutionStrategy::mMu)
		->SetConstructor<CMAEvolutionStrategy, uint, double>("dimension", "step-size");
}

/**
 * Create a CMA-ES algorithm with the given dimension and an optional initial step size.
 * @param pDimension The number of dimensions of the problem \f$N\f$.
 * @param pStepSize The initial step size \f$\sigma\f$.
 */
CMAEvolutionStrategy::CMAEvolutionStrategy(uint pDimension, double pStepSize) : evaluator(), mState(),
	N(mState.N), sigma(mState.sigma), B(mState.B), D(mState.D), mean(mState.mean), ps(mState.ps), pc(mState.pc),
	weights(), mueff(), cc(), cs(), c1(), cmu(), damps(), chiN() {

	mState.N = pDimension;
	mState.sigma = pStepSize;

	mLambda = 4 + floor(3 * log(N));
	mMu = mLambda / 2;
}

CMAEvolutionStrategy::~CMAEvolutionStrategy() {
}

/**
 * Get the number of samples (offspring).
 * @return The number of samples.
 */
uint CMAEvolutionStrategy::GetLambda() const {
	return mLambda;
}

/**
 * Set the number of samples (offspring).
 * @param pLambda The number of samples.
 */
void CMAEvolutionStrategy::SetLambda(uint pLambda) {
	mLambda = pLambda;
}

/**
 * Get the number of Organism used to update the state (parent).
 * @return The number of parents.
 */
uint CMAEvolutionStrategy::GetMu() const {
	return mMu;
}

/**
 * Set the number of Organism used to update the state (parent).
 * @param pMu The number of parents.
 */
void CMAEvolutionStrategy::SetMu(uint pMu) {
	mMu = pMu;
}

void CMAEvolutionStrategy::Setup() {
	mean = RandomVector();
	ps = VectorXd::Zero(N);
	pc = VectorXd::Zero(N);
	B = MatrixXd::Identity(N, N);
	D = VectorXd::Ones(N);

	SetPool(0, make_shared<OrganismPool>());
	SetPool(2, make_shared<CMAStatePool>(mState));
}

//void PrintSize(string id,  MatrixXd matrix) {
//	cout << id << ": " << matrix.rows() << "x" << matrix.cols() << endl;
//}

void CMAEvolutionStrategy::Begin() {
	weights = VectorXd::Constant(mMu, 1, log(mMu + 0.5)) - ArrayXd::LinSpaced(mMu, 1, mMu).log().matrix();
	weights /= weights.sum();
	mueff = 1 / weights.array().square().sum();

	cc = (4+mueff/N) / (N+4 + 2*mueff/N);
	cs = (mueff+2) / (N+mueff+5);
	c1 = 2 / (pow(N+1.3, 2) + mueff);
	cmu = min(1-c1, 2 * (mueff-2 + 1/mueff) / (pow(N+2, 2) + mueff));
	damps = 1 + 2*max(0., sqrt((mueff-1)/(N-1))-1) + cs;

	chiN = sqrt(N) * (1-1.0/(4*N)+1.0/(21*N*N));
}

void CMAEvolutionStrategy::Loop() {
	mState = *GetPool(2)->To<CMAStatePool>();

	// Spawning
	GenomePoolPtr spawnPool = make_shared<GenomePool>(mLambda);
	Execute("S", function<void(void)>([&] () {
		MultiThreading::For(0, mLambda, [&] (int i) {
			VectorXd x = mean + sigma * B * (D.array() * RandomVector().array()).matrix();
			DoubleArrayGenomePtr genome = make_shared<DoubleArrayGenome>();
			genome->GetGenes() = vector<double>(x.data(), x.data() + x.size());
			(*spawnPool)[i] = static_pointer_cast<Genome>(genome);
		});
	}));

	// Evaluation
	OrganismPoolPtr evaluatedPool = Execute("E", evaluator, spawnPool);
	evaluatedPool->Sort();

	// Extract
	OrganismPoolPtr filteredPool = GreedySelection().Select(evaluatedPool, mMu);
	MatrixXd X(N, mMu);
	MultiThreading::For(0, mMu, [&] (int i) {
		double* begin = &(*filteredPool->at(i)->GetGenome()->To<DoubleArrayGenome>()->GetGenes().begin());
		X.col(i) = Map<VectorXd>(begin, N);
	});

	// Update
	MatrixXd C;
	Execute("U", function<void(void)>([&] () {
		VectorXd meanOld = mean;
		mean = X * weights;

		ps = (1-cs)*ps + sqrt(cs*(2-cs)*mueff) * mState.GetInvSqrtC() * (mean - meanOld) / sigma;
		auto counteval = GetPopulation()->GetEvaluation();
		bool hsig = ps.norm() / sqrt(1.0 - pow(1-cs, 2.0*counteval/mLambda))/chiN < 1.4 + 2.0/(N+1);
		pc = (1-cc)*pc;
		if (hsig)
			pc += sqrt(cc*(2-cc)*mueff) * (mean - meanOld) / sigma;

		MatrixXd Xtmp = (1/sigma) * (X.colwise() - meanOld);
		C = mState.GetC();
		C = (1-c1-cmu) * C +
				c1 * (pc * pc.transpose() + (hsig ? 0 : 1) * cc * (2-cc) * C) +
				cmu * Xtmp * weights.asDiagonal() * Xtmp.transpose();

		sigma *= exp((cs/damps)*(ps.norm()/chiN - 1));
	}));

	Execute("D", function<void(void)>(bind(&CMAEvolutionStrategy::Decompose, this, ref(C))));

	// Finalize
	SetPool(0, evaluatedPool);
	SetPool(2, make_shared<CMAStatePool>(mState));
}

vector<string> CMAEvolutionStrategy::GetTimeRecordOrder() const {
	return { "S", "E", "U", "D" };
}

VectorXd CMAEvolutionStrategy::RandomVector() {
	normal_distribution<> dist;
	return VectorXd(N).unaryExpr([&dist] (double dummy) {
		return dist(Random::generator);
	});
}

bool CMAEvolutionStrategy::IsReady() {
	return bool(evaluator);
}

void CMAEvolutionStrategy::Decompose(const MatrixXd& C) {
	SelfAdjointEigenSolver<MatrixXd> solver(C);

	if (solver.info() != Success) {
		EA_LOG_ERROR << "CMA-ES: Eigen-decomposition is failed. Terminate..." << flush;
		GetSession()->Terminate();
		//throw EA_EXCEPTION(EAException, GENERAL, "CMA-ES: Eigen-decomposition is failed.");
	}

	B = solver.eigenvectors();
	D = solver.eigenvalues().unaryExpr([] (double in) {
		return sqrt(in);
	});
}

} /* namespace ea */
