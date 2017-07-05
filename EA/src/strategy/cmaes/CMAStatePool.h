/*
 * CMAInfoPool.h
 *
 *  Created on: Jun 6, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../core/pool/Pool.h"
#include "../../Common.h"
#include "../../rtoc/Constructible.h"
#include <eigen3/Eigen/Dense>

namespace ea {

using namespace Eigen;

/**
 * Extension of Pool to store the state of the CMAEvolutionStrategy algorithm.
 * Because a Population can only store a Pool, states of CMAEvolutionStrategy must be kept
 * in a Pool in order to be saved and restored. The states of a CMA-ES include:
 * - #N: The number of dimensions \f$N\f$
 * - #mean: The current mean \f$m\f$
 * - #ps: The isotropic evolution path \f$p_\sigma\f$
 * - #pc: The anisotropic evolution path \f$p_c\f$
 * - #sigma: The step-size \f$\sigma\f$
 * - #B: The eigenvectors of covariance matrix \f$C\f$
 * - #D: The singular values of covariance matrix \f$C\f$
 *
 * The covariance matrix \f$C\f$ can be restored from #B and #D by using the function GetC().
 * The square root of inverse covariance matrix \f$C^{-1/2}\f$ could also be calculated by using GetInvSqrtC().
 */
class CMAStatePool : public Pool {
public:
	EA_TYPEINFO_DEFAULT(CMAStatePool);

	virtual ~CMAStatePool();

	uint N;			///< The number of dimensions.
	VectorXd mean;	///< The current mean, a vector with length #N.
	VectorXd ps;	///< The isotropic evolution path, a vector with length #N.
	VectorXd pc;	///< The anisotropic evolution path, a vector with length #N.
	double sigma;	///< The step-size
	MatrixXd B;		///< The eigenvectors of covariance matrix, orthogonal matrix with size #N x #N
	VectorXd D;		///< The singular values of covariance matrix, positive vector with length #N

	MatrixXd GetC() const;
	MatrixXd GetInvSqrtC() const;

protected:
	virtual void DoSerialize(ostream& pStream) const override;
	virtual void DoDeserialize(istream& pStream) override;
};

} /* namespace ea */
