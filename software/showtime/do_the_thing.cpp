#include "showtime/do_the_thing.h"

#include <iostream>

namespace showtime {

void doTheThing(const std::string& who) {
  std::cout << "Doing the thing with " << who << "\n";
}

} // namespace
