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
#ifndef CSV
		std::clog<<"Total Iteration of Pub-sub: "<< PUB_SUB_ITERATION_MAX_CNT<<std::endl;
		std::clog << "benchmark result: "<<duration << "us \n";
		std::clog << "Approx. time for one Pub-Sub: "<< (float)duration/PUB_SUB_ITERATION_MAX_CNT <<"us \n";
#else
		std::clog<<"TI,"<< PUB_SUB_ITERATION_MAX_CNT<<",BR,"<<duration << ",PS,"<< (float)duration/PUB_SUB_ITERATION_MAX_CNT <<"\n";
#endif
	}


};

#endif /* BENCHMARK_HPP_ */
