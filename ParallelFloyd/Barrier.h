#pragma once
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

class barrier {
	unsigned int const count;
	std::atomic<unsigned int> spaces;
	std::atomic<unsigned int> generation;
public:
	explicit barrier(unsigned nthreads) : count(nthreads), spaces(nthreads),
		generation(0) { }
	void wait() {
		unsigned const my_generation = generation;
		if (!--spaces) {
			spaces = count;
			++generation;
		}
		else {
			while (generation == my_generation)
				std::this_thread::yield();
		}
	}
};