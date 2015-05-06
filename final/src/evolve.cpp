#include "methods.h"


int main(int argc, char** argv) {
  
  auto a = double{0.01};
  auto L = 2.0;
  auto T = 100;
  auto N = floor(L/a);


  double dx = a;
  double dt = 0.01;
  double V  = 0.0;
  

  std::vector<double> xx(N+1,0.0);

  //ROOT stuff
  TFile *tf = new TFile("outfile.root","RECREATE");
  TTree *tt = new TTree("data","data");
  
  std::vector<double> *y 
    = new std::vector<double>(N+1,0.0);

  tt->Branch("y",&y); //address of pointer
  tt->Branch("x",&xx); //address of pointer

  std::cout << "\n\t=== 1d Schrodinger on chain ===\n";
  std::cout << "\n";
  std::cout << "\tProposed chain space a:    " << a  << "\n";
  std::cout << "\tProposed lattice points N: " << N << "\n\n";
  
  
  SparseMatrix<std::complex<double> > W(N+1,N+1);
  //SparseMatrix<double> W(N+1,N+1);
  
  std::vector<VectorXcd> psi(T,VectorXcd::Zero(N+1));
  //VectorXcd psi = VectorXcd::Zero(N+1);  
  
  //Initialize W;
  for(int i = 1; i < N; ++i) {
    W.insert(i,i)   = (-2.0 + 4.0*ii*pow(dx,2)/dt - 2.0*pow(dx,2)*V);
    if(i>1) W.insert(i-1,i) = 1.0;
    if(i<N-1) W.insert(i+1,i) = 1.0;
    // W.insert(i,i)   = 2.0;
    // if(i>1)   W.insert(i,i-1) = 1.0;
    // if(i<N-1) W.insert(i,i+1) = 1.0;
  }
  
  W *= dt/(pow(dx,2.0)*8.0*ii);

  //Initialize \psi_0
  //Index to lattice space conversion
  for(int i = 0; i < xx.size(); ++i) {
    xx[i] = -1 + a*i;
    psi[0](i) = wave_packet(xx[i]);
  }
  
  //ConjugateGradient<SparseMatrix<std::complex<double> > > cg;
  BiCGSTAB<SparseMatrix<std::complex<double>> > solver;
  VectorXcd chi(N+1);

  //solver.compute(W);
  for(int j = 0; j < T; ++j) {
    chi = solver.compute(W).solve(psi[j]);
    psi[j+1] = chi - psi[j];
  }

  
  //Convert VectorXcd to std::vector<std::complex<double> >
  std::for_each(psi.begin(),psi.end(),[&](VectorXcd p){
      for(int o = 0 ; o < N+1; ++o)
	y->at(o) = std::norm(p(o));
      tt->Fill();
    });
  
  tt->Write();
  tf->Close();
  
}
