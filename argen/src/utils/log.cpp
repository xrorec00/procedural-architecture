#include "log.h"
#include "globals.h"

#include <iostream>

namespace Log {

void debug(std::string msg) {
	if (Config::DEBUG) std::cout<<msg<<std::endl<<std::flush;
}

void error(std::string msg) {
	std::cerr<<msg<<std::endl<<std::flush;
	exit(1);
}

}