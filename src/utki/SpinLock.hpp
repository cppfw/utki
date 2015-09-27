#pragma once

#include <atomic>

namespace utki{

/**
 * @brief Spin lock.
 */
class SpinLock{
    std::atomic_flag flag = ATOMIC_FLAG_INIT ;
public:
	/**
	 * @brief Acquire spin lock.
     */
    void lock(){
        while(this->flag.test_and_set(std::memory_order_acquire)){}
    }
	
	/**
	 * @brief Release spin lock.
     */
    void unlock(){
        this->flag.clear(std::memory_order_release);
    }
};

}
