#include "LArgon.h"

void LArgon::evolve(const bool r) {
  r ? _restart() : _from_file();

}

void LArgon::_restart() {

  
}

void LArgon::_from_file() {
  std::cout << "from_file\n";
  
}
