#ifndef BENCHMARK_HPP_
#define BENCHMARK_HPP_
#include <chrono>
#include <iostream>

class Timer{
public:
Timer():m_StartTimepoint{}{
}

~Timer(){
}

void start(){
	m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

void stop(){
	auto endTimepoint = std::chrono::high_resolution_clock::now();
	auto  start= std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
	auto duration = end - start;
//	double ms = duration * 0.001;
	std::clog<<duration<<"\n";
}

private:
std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
}benchmark_us;

extern Timer benchmark_us;

#endif /* BENCHMARK_HPP_ */
