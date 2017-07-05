/*
 * CMAInfoPool.cpp
 *
 *  Created on: Jun 6, 2017
 *      Author: Bui Quang Minh
 */

#include "CMAStatePool.h"
#include "../../rtoc/BinarySerializer.h"

namespace ea {

using namespace Eigen;

CMAStatePool::~CMAStatePool() {
}

#ifndef DOXYGEN_IGNORE
template<class Scalar, int Rows, int Cols>
class BinarySerializer<Matrix<Scalar, Rows, Cols>> {
public:
	using MatrixType = Matrix<Scalar, Rows, Cols>;

	inline static void Write(ostream& pStream, const MatrixType pData) {
		BinarySerializer<int>::Write(pStream, pData.rows());
		BinarySerializer<int>::Write(pStream, pData.cols());
		pStream.write((char*)pData.data(), pData.size() * sizeof(Scalar));
	}
	inline static void Read(istream& pStream, MatrixType& pData) {
		long rows = BinarySerializer<int>::Read(pStream);
		long cols = BinarySerializer<int>::Read(pStream);
		pData.resize(rows, cols);
		pStream.read((char*)pData.data(), pData.size() * sizeof(Scalar));
	}
	inline static const MatrixType Read(istream& pStream) {
		MatrixType data;
		Read(pStream, data);
		return data;
	}
};
#endif

void CMAStatePool::DoSerialize(ostream& pStream) const {
	Write(pStream, N);
	Write(pStream, sigma);
	Write(pStream, mean);
	Write(pStream, ps);
	Write(pStream, pc);
	Write(pStream, B);
	Write(pStream, D);
}

void CMAStatePool::DoDeserialize(istream& pStream) {
	N = Read<uint>(pStream);
	sigma = Read<double>(pStream);
	Read(pStream, mean);
	Read(pStream, ps);
	Read(pStream, pc);
	Read(pStream, B);
	Read(pStream, D);
}

/**
 * Get the covariance matrix \f$C\f$.
 * This covariance matrix is recovered from its component by using the formula \f$C = BD^2B^T\f$
 * with \f$D^2\f$ in the diagonal form.
 * @return The covariance matrix.
 */
MatrixXd CMAStatePool::GetC() const {
	return B * D.array().square().matrix().asDiagonal() * B.transpose();
}

/**
 * Get the inverse of square root of the covariance matrix \f$C^{-1/2}\f$.
 * This matrix is calculated by using the formula \f$C^{-1/2} = BD^{-1}B^T\f$
 * with \f$D^2{-1}\f$ in the diagonal form.
 * @return The inverse of square root of the covariance matrix.
 */
MatrixXd CMAStatePool::GetInvSqrtC() const {
	return B * D.array().inverse().matrix().asDiagonal() * B.transpose();
}

} /* namespace ea */

