//vgenty@nevis.columbia.edu

#include "Analyzer.h"

int main(int argc, char *argv[]) {
  
  auto a = std::make_unique<Analyzer>("raw");
  a->a();
  a->b();
  return 0;
}
