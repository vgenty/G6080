//Usual garbage
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <iomanip>

//Tell eigen to use the whole stack if necessary
#define EIGEN_STACK_ALLOCATION_LIMIT 0

//Eigen garbage
#include <Eigen/Eigen>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/StdVector>
#include <Eigen/Cholesky>

using Eigen::SparseMatrix;
using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Map;
using Eigen::ConjugateGradient;
using Eigen::aligned_allocator;

//Tell Eigen I want VectorXd in std::vector<T>
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorXd)

//ROOT garbage
#include "TFile.h"
#include "TTree.h"

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

VectorXd CG(const MatrixXd &A, const VectorXd &b, VectorXd x) {
  VectorXd r = b - A*x;
  VectorXd p = r;
  auto rso = r.dot(r);
  
  auto max = 1000;
 
  for(int i = 0 ;i <= pow(10,6); ++i) {
    auto Ap = A*p;
    auto alpha = rso/(p.dot(Ap));
        
    x = x + alpha*p;

    r = r - alpha*Ap;
    
    auto rsn = r.dot(r);
    
    if(sqrt(rsn) < pow(10,-3) || i == max)
      break;
    
    p = r + (rsn/rso)*p;
    rso = rsn;
    //std::cout << i << " " << rsn << "\n";
  }

  return x;
}


int main()
{
 
  std::cout << "\n";
  std::cout << "\tProposed lattice spacing a:        " << a  << "\n";
  std::cout << "\tProposed lattice lattice points N: " << NN << "\n";

  int l;
  MatrixXd lap = MatrixXd::Zero(NN,NN); // Laplace
  VectorXd rho = VectorXd::Zero(NN); // Rho later we will flatten
  VectorXd x   = VectorXd::Random(NN);

  auto d = 10;
  
  auto c = 50 ;
  
  auto loc_x_start = (0.5  + a/2);
  auto loc_y_end   = (0.25 + a/2);
  
  auto xstart = loc_x_start;
  auto ystart = loc_y_end;
  
  for(int i = 0; i < NN; ++i) {
    auto loc_x_end = floor(i/(N+1));
    auto loc_y_end = fmod(i,N+1);
    l=0;
    auto xend = loc_x_end*a;
    auto yend = loc_y_end*a;
    
    auto V = -c/(sqrt(pow((xend-xstart),2)+pow(yend-ystart,2)));
    std::cout << V << std::endl;
    
    if(!check_boundary(i) && !check_in_box(i)) {
      lap(i,i) = -4.0/pow(a,2) - V/5;
      
      for(int nei : {i+1,i-1,i+N+1,i-N-1}) {
	if( nei < NN && nei > 0){
	  if(!check_boundary(nei) && !check_in_box(nei)) {
	    lap(i,nei) = 1/pow(a,2);  
	  }
	}
      }    
    } else {
      x(i) = 0;
    }
  }
  
  lap = -1.0*lap*5.0; // (1/2m);
  
  auto e = -500;
  auto LAP = lap - e * MatrixXd::Identity(NN,NN); // Laplace
  
  
  std::cout << std::endl;
  std::cout << "get it done\n";
  VectorXd yy;
  
  VectorXd rrr = VectorXd::Zero(NN);
  
  std::vector<VectorXd> tr;
  std::vector<VectorXd> w;
  std::vector<double> al;
  std::vector<double> beta;
  
  for(int h = 0; h < 1000; ++h) {
    tr.push_back(rrr);
    w.push_back(rrr);
   
    beta.push_back(0.0);
    al.push_back(0.0);
  }
  std::cout << "a\n";
  
  tr[0] = VectorXd::Zero(NN);
  tr[1] = VectorXd::Random(NN);
  int j = 100;
  std::cout << "b\n";
  //Implement lanczos algorithm
  
  for(int pp = 1 ; pp < j-2; ++pp) {
    w[pp]  =lap*tr[pp];

      
    al[pp] = w[pp].dot(tr[pp]);
    w[pp] = w[pp] - al[pp]*tr[pp] - beta[pp]*tr[pp-1];
    
    beta[pp+1] = w[pp].norm();
    tr[pp+1] = w[pp]/beta[pp+1];
    
    std::cout << pp << "\n";
  }
  w[j-1]  = lap*tr[j-1];
  al[j-1] = w[j-1].dot(tr[j-1]);

  //Create tri-diagonal LANCZOS matrix
  std::cout << "tri-diagonal LANCZOS" << "\n";
  MatrixXd T(j,j);
  
  for(int ppp = 0; ppp < j; ++ppp){
      
    T(ppp,ppp)   = al[ppp];
    
    if(ppp+1 < j)
      T(ppp+1,ppp) = beta[ppp+1];
    if(ppp-1 > 0)
      T(ppp-1,ppp) = beta[ppp+1];

  }

  std::cout << T << std::endl;
  
  //#end
  //Cholsky decomposition to find eigenvector
  LLT<MatrixXd> lltOfA(T);  
  
  for(int w = 0; w < 1000; ++w) {
    yy = CG(LAP,x,x);
    yy = yy / yy.norm();
    auto p = (yy-x).norm()/yy.size();
    //d::cout << p << std::endl;
    if(p < pow(10,-3.5))
      break;
    
    x = yy;
    
  }
  

  //print out the eigenvalue
  std::cout << "Eigenvalue\n" << std::endl;
  std::cout << (lap*yy).norm()/yy.norm() << std::endl;
 
  //Write data out to ROOT file : - )
  std::vector<double> *y = new std::vector<double>();
  y->resize(yy.size());
  for(int o = 0 ; o < y->size(); ++o)
    y->at(o) = yy(o);
  
  TFile *tf = new TFile("outfile.root","RECREATE");
  TTree *tt = new TTree("data","data");
  
  tt->Branch("y",&y); //address of pointer
  tt->Fill();
  tt->Write();
  tf->Close();


 
 
}
