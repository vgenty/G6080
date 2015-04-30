#include <iostream>
#include <cmath>
#include <memory>
#include <vector>

#define EIGEN_STACK_ALLOCATION_LIMIT 0
#include <Eigen/Eigen>
#include <Eigen/IterativeLinearSolvers>


using Eigen::SparseMatrix;
using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Map;
using Eigen::ConjugateGradient;


#include "TFile.h"
#include "TTree.h"

int main()
{

  const int N  = 50;
  const int N2 = N-2;
  const int NN = pow(N2,2);
  const double a = 1/static_cast<double>(pow(N,2));
  
  std::cout << "\n";
  std::cout << "\tProposed lattice spacing a:        " << 1/pow(static_cast<double>(N),2)      << "\n";
  std::cout << "\tProposed lattice lattice points N: " << NN << "\n";
  
  
  //auto lap = std::make_shared<MatrixXd>(MatrixXd::Zero(NN,NN));
  MatrixXd lap = MatrixXd::Zero(NN,NN); // Laplace
  //VectorXf phi = VectorXf::Zero(NN);    // Field
  MatrixXd rho = MatrixXd::Zero(N2,N2); // Rho later we will flatten
  
  
  //Fill the block diagonal
  for(int l = 0; l < N2; l++) 
    for(int i = N2*l; i < N2*(l+1); i++) 
      for(int j = i; j < N2*(l+1); j++) 
	if(j==i)         lap(i,j) =  4.0;
	else if(j==i+1)  lap(i,j) = -1.0;
	
  //Fill the band -I
  for(int i = 0; i < NN; i++) 
    for(int j = i; j < NN; j++) 
      if(j == i + N2) 
	lap(i,j) = -1.0;

  lap.triangularView<Eigen::Lower>() = lap.transpose();
  
  rho(floor(N2/2),floor(N2/2)) = 5*pow(N,2);
  Map<VectorXd> v(rho.data(),rho.size());

  //std::cout << lap << std::endl;
  

  VectorXd x(NN);
    
  ConjugateGradient<Matrix<double,NN,NN>,Eigen::Lower|Eigen::Upper> cg;
  cg.compute(lap);
  x = cg.solve(v);
  
  std::cout << "#iterations:     " << cg.iterations() << std::endl;
  std::cout << "estimated error: " << cg.error()      << std::endl;
  // // update b, and solve again

  std::vector<double> *y = new std::vector<double>();
  y->resize(x.size());
  for(int o = 0 ; o < y->size(); ++o)
    y->at(o) = x(o);

  //std::cout << 
  TFile *tf = new TFile("outfile.root","RECREATE");
  TTree *tt = new TTree("data","data");

  tt->Branch("y",&y); //address of pointer
  tt->Fill();
  tt->Write();
  tf->Close();
  
}
