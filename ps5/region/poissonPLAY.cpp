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
const int N  = 100;
const int NN = pow(N+1,2);
const double a = 1/static_cast<double>(N);

bool check_boundary(int i) {
  if(floor(i/(N+1)) != 0  &&
     floor(i/(N+1)) != N &&
     fmod(i,N+1)    != 0 && 
     fmod(i,N+1)    != 1)
    return false;
  else
    return true;
}

bool check_in_box(int i) {
  auto p = floor(i/(N+1));
  auto q = fmod(i,N+1)-1;
  if (q < 0)
    q = N+1;
  
  auto xx = p*a;
  auto yy = q*a;
  if(xx <= 0.75 && xx >= 0.5 &&
     yy <= 0.875 && yy >= 0.625)
    return true;
  else
    return false;
}

VectorXd CG(MatrixXd A, VectorXd b, VectorXd x) {
  VectorXd r = b - A*x;
  VectorXd p = r;
  auto rso = r.dot(r);
  
  for(int i = 0 ;i <= pow(10,6); ++i) {
    auto Ap = A*p;
    auto alpha = rso/(p.dot(Ap));
        
    x = x + alpha*p;

    r = r - alpha*Ap;
    auto rsn = r.dot(r);
    
    if(sqrt(rsn) < pow(10,-4))
      break;
    //else if(sqrt(rs) > pow(10,30))
    //  x = 99
    
    p = r + (rsn/rso)*p;
    rso = rsn;
    std::cout << i << " " << rsn << "\n";
  }

  return x;
}

int main()
{
 
  std::cout << "\n";
  std::cout << "\tProposed lattice spacing a:        " << a  << "\n";
  std::cout << "\tProposed lattice lattice points N: " << NN << "\n";


  auto p = floor((0.5 + a/2)/a);
  auto q = floor((0.25 + a/2)/a);
  
  MatrixXd lap = MatrixXd::Zero(NN,NN); // Laplace
  VectorXd rho = VectorXd::Zero(NN); // Rho later we will flatten
  VectorXd x = VectorXd::Zero(NN); // Rho later we will flatten
  auto d = 10;
  
  auto c = -2*pow(10,5);

  rho(p*(N+1)+q) = -c/4.0 ;
  rho(p*(N+1)+q+1) = -c/4.0 ;
  rho(p*(N+1)+q+N+1) = -c/4.0 ;
  rho(p*(N+1)+q+1+N+1) = -c/4.0 ;
  
  for(int i = 0; i < NN; ++i) {
    if(!check_boundary(i) && !check_in_box(i)) {
      lap(i,i) = -4.0/pow(a,2);
       
      for(int nei : {i+1,i-1,i+N+1,i-N-1}) {
	if( nei < NN && nei > 0){
	  if(!check_boundary(nei)) {
	    if(check_in_box(nei)) {
	      rho(i) = rho(i) - d/pow(a,2);
	    } else {
	      lap(i,nei) = 1/pow(a,2);
	    }
	  }
	}
      }
    } else {
      if(check_boundary(i)) {
	x(i) = 0;
      }
      else if(check_in_box(i)) {
	x(i) = d;
      }
    }
   
  }
  
  
  auto ex = CG(lap,rho,x);
  
  
  //Write out to ROOT file
  std::vector<double> *y = new std::vector<double>();
  y->resize(ex.size());
  for(int o = 0 ; o < y->size(); ++o)
    y->at(o) = ex(o);
  
  TFile *tf = new TFile("outfile.root","RECREATE");
  TTree *tt = new TTree("data","data");
  
  tt->Branch("y",&y); //address of pointer
  tt->Fill();
  tt->Write();
  tf->Close();


 
 
}
