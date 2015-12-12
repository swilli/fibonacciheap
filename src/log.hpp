#ifndef LOG_HPP
#define LOG_HPP

#include <limits.h>

namespace math {
	unsigned int log(unsigned int val) {
		if (val == 0) {
			return UINT_MAX;
		}
		if (val == 1) {
			return 0;
		}
		unsigned int ret = 0;
		while (val > 1) {
			val >>= 1;
			ret++;
		}
		return ret;
	}
}
#endif
