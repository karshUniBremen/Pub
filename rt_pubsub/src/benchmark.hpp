#ifndef BENCHMARK_HPP_
#define BENCHMARK_HPP_
#include <chrono>
#include <iostream>

class Timer {
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
public:
	Timer() :
			m_StartTimepoint { } {
	}

	~Timer() {
	}

	void start() {
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	void stop() const {
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		auto const start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		auto const end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
		auto const duration = end - start;
//	double ms = duration * 0.001;
		std::clog << duration << "\n";
	}


};

Timer benchmark_us;

#endif /* BENCHMARK_HPP_ */
