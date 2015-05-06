#include "includes.h"

std::complex<double> ii(0,1);

double pi() { return 3.14159; }

std::complex<double> wave_packet(const double& x) {
  return exp(ii*2.0*pi()/0.01)*exp(-1.0*pow(x-0.0,2.0)/(2*pow(0.01,2.0)));
}

