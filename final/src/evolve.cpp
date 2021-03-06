#include "includes.h"

//Prototypes and type definitions:

typedef SparseMatrix<std::complex<double> > SMCD;

std::complex<double> ii(0,1);

double V(const double,const double);

std::complex<double> wave_packet(const double&, const double,
				 const double,  const double,
				 const double);

double wave_pocket(const double, const double);

double V_pocket(const double, const double,
		const double, const double,
		const double);
double pi();

std::tuple<SMCD,
	   SMCD > get_LU(SMCD&);

VectorXcd solve_LU(SMCD&,
		   SMCD&,
		   VectorXcd&);


//Begin main program
  
int main(int argc, char** argv) {


  //Fixed parameters
  //auto a = double{0.004};
  auto a = double{0.0005};
  auto L = 2.0;
  auto T = 1000;
  auto N = floor(L/a);

  double dx = a;
  double dt = 0.0001;

  std::vector<double> xx(N+1,0.0);

  //Possibly user defined parameters
  double V0    = 1000;
  double E0    = V0*atof(argv[1]); //Incoming packet energy
  //  double sigma = 0.05;
  double sigma = atof(argv[3]);
  
  auto k = sqrt(2.0*E0-1.0/(2.0*pow(sigma,2.0)));
  if(k != k) {
    std::cout << "Nan observed, increase the energy 2*E = "
	      << 2.0 * E0 << " other part is "
	      << 1.0/(2.0*pow(sigma,2.0)) << " \n";
    std::exit(1);
  }
  
  //Just let mass of particle be m=1 why not
  
  //Decaying state
  auto h = 3000.0;//potential height
  auto w = 0.1;  //inner width
  auto b = 0.5;   //bottom height
  auto s = 0.01;  //side width
  //E0 = pow(pi(),2.0)/(2*pow(w,2.0));
  
  
  //ROOT stuff
  TFile *tf = new TFile(argv[2],"RECREATE");
  TTree *tt = new TTree("data","data");
  
  std::vector<double> *y 
    = new std::vector<double>(N+1,0.0);
  double psi2;
  double ref = 0.0;
  double psi2inside;
  double psi2res;
  
  tt->Branch("y",&y); //address of pointer
  tt->Branch("x",&xx); 
  tt->Branch("psi2",&psi2,"psi2/D");
  tt->Branch("ref" ,&ref ,"ref/D");
  tt->Branch("psi2inside" ,&psi2inside ,"psi2inside/D");
  tt->Branch("psi2res" ,&psi2res ,"psi2res/D");
  
  std::cout << "\n\t=== 1d Schrodinger on chain ===\n";
  std::cout << "\n";
  std::cout << "\tProposed chain space a:    " << a  << "\n";
  std::cout << "\tProposed lattice points N: " << N  << "\n";
  std::cout << "\tIncident energy:           " << E0 << "\n";
  std::cout << "\tStep potential:            " << V0 << "\n";
  std::cout << "\tVelocity:                  " << k  << "\n";
  
  
  SMCD W(N+1,N+1);
  //SMCD Wb(N+1,N+1);
  //SparseMatrix<double> W(N+1,N+1);
  
  std::vector<VectorXcd> psi(T,VectorXcd::Zero(N+1));
  //VectorXcd psi = VectorXcd::Zero(N+1);  

  //Initialize \psi_0
  //Index to lattice space conversion
  for(unsigned int i = 0; i < xx.size(); ++i) {
    xx[i] = -1 + a*i;
    psi[0](i) = wave_packet(xx[i],E0,sigma,-0.5,k);
    //psi[0](i) = wave_pocket(xx[i],w);
  }

  
  //Initialize W;
  for(int i = 1; i < N; ++i) {
    //W.insert(i,i)   = (-2.0 + 4.0*ii*pow(dx,2)/dt - 2.0*pow(dx,2)*V(xx[i],V0));
    W.insert(i,i)   = (-2.0 + 4.0*ii*pow(dx,2)/dt - 2.0*pow(dx,2)*V_pocket(s,b,w,h,xx[i]));
    
    if(i>1) W.insert(i-1,i) = 1.0;
    if(i<N-1) W.insert(i+1,i) = 1.0;
    
    // Wb.insert(i,i)   = (-2.0 + 4.0*ii*pow(dx,2)/(-1.0*dt) - 2.0*pow(dx,2)*V(xx[i],V0));
    // if(i>1) Wb.insert(i-1,i) = 1.0;
    // if(i<N-1) Wb.insert(i+1,i) = 1.0;

  }
  
  W  *= dt/(pow(dx,2.0)*8.0*ii);
  //Wb *= (-1.0*dt)/(pow(dx,2.0)*8.0*ii);

  //Setup Factorization of W
  SMCD LL;
  SMCD UU;
  std::tie(LL,UU) = get_LU(W);
  
  //Check solution for psi[t=0]
  //VectorXcd xxx = solve_LU(LL,UU,psi[0]);
  
  //BiCGSTAB<SMCD> solver;
  //W.makeCompressed();
 
  VectorXcd chi(N+1);

  //solver.compute(W);
  for(int j = 0; j < T-1; ++j) {
    //chi = solver.compute(W).solve(psi[j]);
    chi = solve_LU(LL,UU,psi[j]);
    psi[j+1] = chi - psi[j];
  }

  //Convert VectorXcd to std::vector<std::complex<double> >
  //Lambda is the best way to go here
  
  double rref;
  std::for_each(psi.begin(),psi.end(),[&](VectorXcd p){
      rref = 0.0;
      psi2inside = 0.0;
      psi2res = 0.0;

      for(int o = 0 ; o < N+1; ++o) {
	y->at(o) = std::norm(p(o));
	if(xx[o] < 0.0)
	  rref += std::norm(p(o))*dx;
	if(fabs(xx[o]) < w/2.0)
	  psi2inside += std::norm(p(o))*dx;
	if(xx[o] > (w/2.0 + s))
	  psi2res += std::norm(p(o))*dx;
      }
      
      psi2 = (p.dot(p)).real()*dx;
      ref  = rref;
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
				 const double x0,
				 const double k) {
  
  //Derive k from expectation value of E <\psi|p^2/2m|\psi>
  
  return 1/sqrt(sigma*sqrt(pi()))*exp(ii*k*x)*exp(-1.0*pow(x-x0,2.0)/(2*pow(sigma,2.0)));
  //Try eigenstate
  //return cos(pi()*x/2.0)*cos(pi()*x/3.0);
}

double V(const double x,const double V0) {
  auto Vw = 2.0;
  auto Vc = 1.0;
  
  if(fabs(x - Vc) <= Vw/2.0)
    return V0;
  else
    return 0.0;
}
double wave_pocket(const double x, const double w) {

  if(fabs(x) < w/2.0)
    return sqrt(2/w)*sin(2*pi()*x/w);
  else
    return 0.0;
}

double V_pocket(const double s, const double b,
		const double w, const double h,
		const double x) {
  
  if(fabs(x) < w/2.0)
    return b;
  else if((x <= -1.0*w/2.0 && x >=(-1.0*w/2.0) - s) || 
	  (x >= w/2.0 && x <= w/2.0 + s))
    return h;
  else
    return 0.0;

}

std::tuple<SMCD,SMCD> get_LU(SMCD& A) {
  
  SMCD L(A.rows(),A.rows());
  SMCD U(A.rows(),A.rows());
  
  
  auto Np1 = A.rows();
  
  for(int i = 0; i < Np1; ++i) {
    L.insert(i,i) = 1.0;
    if(i < Np1-1)
      U.insert(i,i+1) = A.coeff(i,i+1);
    if(i == 0) 
      U.insert(i,i) = A.coeff(i,i);
    else {
      if(U.coeff(i-1,i-1) != 0.0)
	L.insert(i,i-1) = A.coeff(i-1,i)/U.coeff(i-1,i-1);
      
      U.insert(i,i)   = A.coeff(i,i) - L.coeff(i,i-1)*A.coeff(i-1,i);
    }	
    
  }
  
  return make_pair(L,U);
}

VectorXcd solve_LU(SMCD& L, SMCD& U, VectorXcd& b) {

  //Vic's implementation of LU, seems speedy enough

  auto Np1 = L.rows();
  VectorXcd y(Np1);
  VectorXcd x(Np1);

  //Forward substitution
  y(0) = b(0);
  for(int i=1; i < Np1; ++i) 
    y(i) = b(i) - L.coeff(i,i-1)*y(i-1);
  
   //Backward substition
  for(int i=0; i < Np1; ++i) 
    if(U.coeff(i,i) != 0.0)
      x(i) = y(i)/U.coeff(i,i);
    else
      x(i) = 0.0;

  for(int i=Np1-1; i >= 0; i--) 
    if(U.coeff(i,i) != 0.0)
      x(i) = (y(i) - U.coeff(i,i+1)*x(i+1))/U.coeff(i,i);
    else
      x(i) = 0.0;
  
  return x;

}

