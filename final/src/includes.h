#include <iostream>
#include <cmath>
#include <memory>  //STL
#include <vector>  //STL
#include <iomanip> // For std::set_precision
#include <complex>

#define EIGEN_STACK_ALLOCATION_LIMIT 0
#include <Eigen/Eigen>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/StdVector>
#include <Eigen/SparseLU>

using Eigen::SparseMatrix;
using Eigen::Matrix;
using Eigen::MatrixXcd;
using Eigen::VectorXcd;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Map;
using Eigen::ConjugateGradient;
using Eigen::BiCGSTAB;
using Eigen::SparseLU;

#include "TFile.h"
#include "TTree.h"

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorXd)
