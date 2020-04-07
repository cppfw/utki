#include "time.hpp"

#include <chrono>

using namespace utki;

uint32_t utki::get_ticks_ms(){
	return uint32_t(
			std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::high_resolution_clock::now().time_since_epoch()
				).count()
		);
}
