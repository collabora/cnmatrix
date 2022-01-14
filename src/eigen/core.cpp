//#define EIGEN_RUNTIME_NO_MALLOC
#include "internal.h"
const char* cnMatrixBackend() {
	return "Eigen";
}

void cnSqRootSymmetric(const CnMat *srcarr, CnMat *dstarr) {
    auto src = CONVERT_TO_EIGEN(srcarr);
    auto dst = CONVERT_TO_EIGEN(dstarr);

    EIGEN_RUNTIME_SET_IS_MALLOC_ALLOWED(false);
    dst.noalias() = Eigen::LLT<MatrixType>(src).matrixL().toDenseMatrix();
}

void cnMulTransposed(const CnMat *src, CnMat *dst, int order, const CnMat *delta, double scale) {
	auto srcEigen = CONVERT_TO_EIGEN(src);
	auto dstEigen = CONVERT_TO_EIGEN(dst);

	if (delta) {
		auto deltaEigen = CONVERT_TO_EIGEN(delta);
		if (order == 0)
			dstEigen.noalias() = scale * (srcEigen - deltaEigen) * (srcEigen - deltaEigen).transpose();
		else
			dstEigen.noalias() = scale * (srcEigen - deltaEigen).transpose() * (src - delta);
	} else {
		if (order == 0)
			dstEigen.noalias() = scale * srcEigen * srcEigen.transpose();
		else
			dstEigen.noalias() = scale * srcEigen.transpose() * srcEigen;
	}
}

void cnTranspose(const CnMat *M, CnMat *dst) {
	auto src = CONVERT_TO_EIGEN(M);
	auto dstEigen = CONVERT_TO_EIGEN(dst);
	if (CN_FLT_PTR(M) == CN_FLT_PTR(dst))
		dstEigen = src.transpose().eval();
	else
		dstEigen.noalias() = src.transpose();
}

void print_mat(const CnMat *M);

double cnDet(const CnMat *M) {
	EIGEN_RUNTIME_SET_IS_MALLOC_ALLOWED(false);
	auto MEigen = CONVERT_TO_EIGEN(M);
	return MEigen.determinant();
}

