#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <iomanip>
#define EIGEN_STACK_ALLOCATION_LIMIT 0
#include <Eigen/Eigen>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/StdVector>

using Eigen::SparseMatrix;
using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Map;
using Eigen::ConjugateGradient;
using Eigen::aligned_allocator;

#include "TFile.h"
#include "TTree.h"

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorXd)


VectorXd ConjugateG(MatrixXd& AA, VectorXd& tr) {

  int k = 0;
  int max_iter = 100;
  double tol = pow(10,-5);

  
  std::vector<VectorXd> r;
  std::vector<VectorXd> x;
  std::vector<VectorXd> p;

  for(int i = 0; i < 500; i++) {
    r.push_back(xx);
    x.push_back(xx);
    p.push_back(xx);
  }

  r[0] = tr;
  auto z = 0;
  std::cout <<  r[0].squaredNorm() << "\n";

  while( r[z].squaredNorm() > tol && z < max_iter) {
    
    //std::cout << "z " << z << " " << r[z].squaredNorm() << "\n";
    
    z++;
    
    if( z == 1 ) 
      p[1] = r[0];
    else 
      p[z] = r[z-1] + r[z-1].dot(r[z-1])/(r[z-2].dot(r[z-2]))*p[z-1];
    
    auto s = AA*p[z];
    auto a = r[z-1].dot(r[z-1])/(p[z].dot(s));

    x[z] = x[z-1] + a*p[z];
    r[z] = r[z-1] - a*s;

  }

  return x[z];
  
}


int main()
{

  const int N  = 150;
  const int N2 = N-2;
  const int NN = pow(N2,2);
  const double a = 1/static_cast<double>(N2);
  
  std::cout << "\n";
  std::cout << "\tProposed lattice spacing a:        " << a  << "\n";
  std::cout << "\tProposed lattice lattice points N: " << NN << "\n";
  
  
  MatrixXd lap = MatrixXd::Zero(NN,NN); // Laplace
  MatrixXd rho = MatrixXd::Zero(N2,N2); // Rho later we will flatten
  
  
  //Fill the block diagonal
  for(int l = 0; l < N2; l++)  {
    for(int i = N2*l; i < N2*(l+1); i++)  {
      for(int j = i; j < N2*(l+1); j++)  {
  	if(l==0 || l==N2-1) {
  	  if(j==i)
  	    lap(i,j) = 1.0;
  	}
  	else if(j == i && (!(i % N2*l) || !(i % (N2*(l+1)-1)))) {
  	  lap(i,j) =  1.0;
  	}
	
  	else if(j == i) {
  	  lap(i,j) = 4.0;

  	  lap(i,j+1)=-1.0;
  	  lap(i,j-1)=-1.0;
  	  lap(i,j-N2) = -1.0;
  	  lap(i,j+N2) = -1.0;
	  
  	}
      }
    }
  }

	
  //Make symmetric // Not necessary I fixed it lol
  //lap.triangularView<Eigen::Lower>() = lap.transpose();


  // Uncomment for problem 1
  // Place charges
  // rho(floor(0.25*N2)-1,floor(0.50*N2))   = -20*a/4.0;
  // rho(floor(0.25*N2)+1,floor(0.50*N2))   = -20*a/4.0;
  // rho(floor(0.25*N2)  ,floor(0.50*N2)+1) = -20*a/4.0;
  // rho(floor(0.25*N2)  ,floor(0.50*N2)-1) = -20*a/4.0;

  //Get the indicies for the box?
  int x1 = floor((0.625 - 0.125)*N2);
  int y1 = floor((0.750 - 0.125)*N2);
  int x2 = floor((0.625 + 0.125)*N2);
  int y2 = floor((0.750 + 0.125)*N2);

  //Convert to column vector
  
  std::cout << " x1: " << x1 <<
    " y1: " << y1 <<
    " x2: " << x2 <<
    " y2: " << y2 << "\n";
    
  //Flatten it

  //std::cout << lap << std::endl;

  
  //Begin conjugate gradient
  VectorXd xx(NN);
    
  
  
  //Go to \rho and do subtraction
  Map<VectorXd> v(rho.data(),rho.size());
  
  //Constant potential condition for initial \phi
  for(int i = 0; i < (x2-x1); ++i) {
    for(int j = 0; j < (y2-y1); j++) {
      //Uncomment region 1
      //x[0](x1*N2+y1+i+j*N2) = 0.08;
      
      for (int l = 0; l < NN; l++) {
	lap(x1*N2+y1+i+j*N2,l) = 0.0;
      }
    }
  }


  // Uncomment for problem 1
  
  // for(int i = 0; i < (y2-y1); ++i) {
  //   v((x1-1)*N2 + y1 + i) = 0.08;
  //   v((x2)*N2   + y1 + i)   = 0.08;
  // }

  // for(int i = 0; i < (y2-y1); ++i) {
  //   v((x1+i)*N2 + y1-1) = 0.08;
  //   v((x1+i)*N2   + y2)   = 0.08;
  // }



  //Number 2
  auto e = 5.2741;
  
  auto AA = lap - e*Matrix<double,NN,NN>::Identity(); // Laplace
  VectorXd tr = VectorXd::Random(NN);
  
  VectorXd x(NN);
  
  for(int i = 0; i < 1000; ++i) {

    auto y = CG(AA,tr);
    y = y / y.norm();
    p = (y-x).norm() / y.size();
    if (p < pow(10,-3.5))
      break;
    x = y;
    
  }

    t = matrixize(y, N+1);
    surf(t);

    eigenvalue = norm(A*y)/norm(y)

    
  }

  // Uncomment for problem 1
  // std::cout << x[z]((x1-2)*N2 + y1 - 1) << std::endl;
  // std::cout << x[z]((x1-2)*N2 + y1 - 2) << std::endl;

  
  // ConjugateGradient<Matrix<double,NN,NN>,Eigen::Lower|Eigen::Upper> cg;
  // cg.compute(lap);
  // x = cg.solve(v);
  
  // std::cout << "#iterations:     " << cg.iterations() << std::endl;
  // std::cout << "estimated error: " << cg.error()      << std::endl;

  //Write out to ROOT file


  std::vector<double> *y = new std::vector<double>();
  y->resize(x[z].size());
  for(int o = 0 ; o < y->size(); ++o)
    y->at(o) = x[z](o);

  TFile *tf = new TFile("outfile.root","RECREATE");
  TTree *tt = new TTree("data","data");

  tt->Branch("y",&y); //address of pointer
  tt->Fill();
  tt->Write();
  tf->Close();
  

}
