#include "helpers.h"

#include <cstdlib>

namespace Helper {
	bool GetBool (const std::string & str) {
		if (str == "true") {
			return true;
		} else {
			return false;
		}
	}

	int GetInt (const std::string & str) {
		return atoi(str.c_str());
	}
}
