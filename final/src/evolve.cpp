#include "includes.h"

std::complex<double> ii(0,1);

double V(const double);
std::complex<double> wave_packet(const double&, const double,
				 const double,  const double);
double pi();
  
int main(int argc, char** argv) {


  //Fixed parameters
  auto a = double{0.004};
  auto L = 2.0;
  auto T = 10000;
  auto N = floor(L/a);

  double dx = a;
  double dt = 0.0001;

  std::vector<double> xx(N+1,0.0);

  //Possibly user defined parameters
  double E0    = 1000; //Incoming packet energy
  double sigma = 0.08;
  
  //Just let mass of particle be m=1 why not
  
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
  //SparseMatrix<std::complex<double> > Wb(N+1,N+1);
  //SparseMatrix<double> W(N+1,N+1);
  
  std::vector<VectorXcd> psi(T,VectorXcd::Zero(N+1));
  //VectorXcd psi = VectorXcd::Zero(N+1);  

  //Initialize \psi_0
  //Index to lattice space conversion
  for(unsigned int i = 0; i < xx.size(); ++i) {
    xx[i] = -1 + a*i;
    psi[0](i) = wave_packet(xx[i],E0,sigma,-0.2);
  }

  
  //Initialize W;
  for(int i = 1; i < N; ++i) {
    W.insert(i,i)   = (-2.0 + 4.0*ii*pow(dx,2)/dt - 2.0*pow(dx,2)*V(xx[i]));
    if(i>1) W.insert(i-1,i) = 1.0;
    if(i<N-1) W.insert(i+1,i) = 1.0;
    //Wb.insert(i,i)   = (-2.0 + 4.0*ii*pow(dx,2)/(-1.0*dt) - 2.0*pow(dx,2)*V(xx[i]));
    //if(i>1) Wb.insert(i-1,i) = 1.0;
    //if(i<N-1) Wb.insert(i+1,i) = 1.0;
    // W.insert(i,i)   = 2.0;
    // if(i>1)   W.insert(i,i-1) = 1.0;
    // if(i<N-1) W.insert(i,i+1) = 1.0;
  }
  
  W  *= dt/(pow(dx,2.0)*8.0*ii);
  //Wb *= (-1.0*dt)/(pow(dx,2.0)*8.0*ii);



    
  //ConjugateGradient<SparseMatrix<std::complex<double> > > cg;
  BiCGSTAB<SparseMatrix<std::complex<double>> > solver;
  VectorXcd chi(N+1);

  //solver.compute(W);
  for(int j = 0; j < T-1; ++j) {
    //if(j < floor((T-1)/2))
    chi = solver.compute(W).solve(psi[j]);
    //else
    //chi = solver.compute(Wb).solve(psi[j]);
    
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

////////////Methods////////////
double pi() { return 4.0*atan(1.0); }

std::complex<double> wave_packet(const double& x,
				 const double E,
				 const double sigma,
				 const double x0) {
  
  //Derive k from expectation value of E <\psi|p^2/2m|\psi>
  auto k = sqrt(2.0*E-1.0/(2.0*pow(sigma,2.0)));
  if(k != k) {
    std::cout << "Nan observed, increase the energy 2*E = "
	      << 2.0 * E << " other part is "
	      << 1.0/(2.0*pow(sigma,2.0)) << " \n";
    std::exit(1);
  }
  
  return 1/sqrt(sigma*sqrt(pi()))*exp(ii*k*x)*exp(-1.0*pow(x-x0,2.0)/(2*pow(sigma,2.0)));
  //Try eigenstate
  //return cos(pi()*x/2.0)*cos(pi()*x/3.0);
}

double V(const double x) {
  auto V0 = 1800.0;
  auto Vw = 0.6;
  auto Vc = 0.3;

  if(fabs(x - Vc) <= Vw/2.0)
    return V0;
  else
    return 0.0;
}
