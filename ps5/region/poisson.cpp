#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include <iomanip>
#define EIGEN_STACK_ALLOCATION_LIMIT 0
#include <Eigen/Eigen>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/StdVector>
#include <Eigen/Eigenvalues>
#include <complex>

using Eigen::SparseMatrix;
using Eigen::Matrix;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Map;
using Eigen::ConjugateGradient;
using Eigen::aligned_allocator;
using Eigen::EigenSolver;

#include "TFile.h"
#include "TTree.h"

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorXd)


//Global vars, getting ghetto
const int N  = 35;
const int N2 = N-2;
const int NN = pow(N2,2);
const double a = 1/static_cast<double>(N2);


VectorXd ConjugateG(MatrixXd AA, VectorXd XX,VectorXd tr) {

  VectorXd xx(NN);

  int max_iter = 100;
  double tol = pow(10,-5);

  
  std::vector<VectorXd> r;
  std::vector<VectorXd> x;
  std::vector<VectorXd> p;

  for(int i = 0; i < max_iter; i++) {
    r.push_back(xx);
    x.push_back(xx);
    p.push_back(xx);
  }
  
  //r[0] = XX - AA*tr;
  r[0] = XX;
  auto z = 0;
  while( r[z].squaredNorm() > tol && z < max_iter) {
    
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
	    lap(i,j) = 0.0;
  	    //lap(i,j) = 1.0;

  	}
  	else if(j == i && (!(i % N2*l) || !(i % (N2*(l+1)-1)))) {
  	  //lap(i,j) =  1.0;
	  lap(i,j) =  0.0;
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
  
  //auto AA = lap - e*Matrix<double,NN,NN>::Identity(); // Laplace
  auto AA = lap; // Laplace
  VectorXd tr = VectorXd::Random(NN);
  VectorXd x = VectorXd::Random(NN);
  
  
  
#if 0
  // Reverse iteration for problem number 2
  std::cout << "Calculating reverse iteration" << std::endl;
  for(int i = 0; i < 1000; ++i) {
    std::cout << "step : " << i << std::endl;
    
    auto y = ConjugateG(AA,x,tr);
    y = y / y.norm();
    auto p = (y - x).norm() / y.size();
    std::cout <<  p << std::endl;
    if (p < pow(10,-3))
      break;
    x = y;
    
  }
#endif
  
  EigenSolver<MatrixXd> eigensolver(AA);
  if (eigensolver.info() != Eigen::Success) abort();
  std::cout << "The eigenvalues of A are:\n" << " "  << std::endl;
  std::cout << "Here's a matrix whose columns are eigenvectors of A \n"
	    << "corresponding to these eigenvalues:\n";


  
  auto u = eigensolver.eigenvalues()[1];
  auto i = u.real();

  std::vector<std::pair<int,double> > vals;

  for(int i = 0 ; i < NN; ++i)
    vals.push_back(std::make_pair(i,eigensolver.eigenvalues()[i].real()));
  
  std::sort(vals.begin(),vals.end(),[](std::pair<int,double> a,
				       std::pair<int,double> b )
	    {
	      return a.second < b.second;
	    }
	    );

  std::cout << vals[0].first << " " << vals[0].second << "\n";
  std::cout << vals[1].first << " " << vals[1].second << "\n";
  std::cout << vals[2].first << " " << vals[2].second << "\n";
  std::cout << vals[3].first << " " << vals[3].second << "\n";

  auto j = 0;
  for(const auto& o : vals) {
    j++;
    if(o.second > pow(10,-4))
       break;
  }
  std::cout << "\n";
  std::cout << vals[j].first << " " << vals[j].second << "\n";

  
  std::cout << eigensolver.eigenvectors().col(j)(1) << std::endl;
	
  //<< (eigensolver.eigenvectors()). << std::endl;

  //  t = matrixize(y, N+1);
  
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
  y->resize(NN);
  //y->resize(x[z].size());
  for(int o = 0 ; o < y->size(); ++o)
    y->at(o) = eigensolver.eigenvectors().col(j+1)(o).real();
      //y->at(o) = x[z](o);

  TFile *tf = new TFile("outfile.root","RECREATE");
  TTree *tt = new TTree("data","data");

  tt->Branch("y",&y); //address of pointer
  tt->Fill();
  tt->Write();
  tf->Close();
  

}
