#include <iostream>
#include <cmath>

#include <Eigen/Dense>

using Eigen::MatrixXf;
using Eigen::VectorXf;

int main()
{

  const int N  = 5;
  const int N2 = N-2;
  const int NN = pow(N-2,2);
    
  std::cout << "\n";
  std::cout << "\tProposed lattice spacing a:        " << 1/pow(static_cast<double>(N),2)      << "\n";
  std::cout << "\tProposed lattice lattice points N: " << pow(static_cast<double>(N),4) << "\n";
  
  
  MatrixXf lap(NN,NN); // Laplace
  VectorXf phi(NN); // Field
  VectorXf rho(NN); // Rho 


  //Fill the block diagonal
  for(int l = 0; l < N2; l++) 
    for(int i = N2*l; i < N2*(l+1); i++) 
      for(int j = i; j < N2*(l+1); j++) 
	if(j==i)         lap(i,j) =  2.0;
	else if(j==i+1)  lap(i,j) = -1.0;
	
    
  

  
  //Fill the band -I
  for(int i = 0; i < NN; i++)
    for(int j = i; j < NN; j++)
      if(j == i + N2) lap(i,j) = -1.0; 
  
  

  auto lap2 = lap + lap.transpose();
  std::cout << lap2 << std::endl;
  
}
