//============================================================================
// Name        : rtcomm.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//Reference : https://www.cs.auckland.ac.nz/references/unix/digital/APS33DTE/DOCU_006.HTM
#include "rt_pub.hpp"
#include "rt_sub.hpp"
#include "shm_block.hpp"
#include <iostream>
#include <ostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <utility>

using namespace std;

extern void rt_sub_handler(int signo, siginfo_t *info, void *extra);

//int shm_test(){
//	index_1.setvalue(12.80);
//	var_space.write(index_1);
//	var_space.read(index_1);
//	clog<<float(index_1.getvalue())<<endl;
//	return 0;
//}

//int main(int argc, char **argv) {
//
//	if (argc < 2) {
//		std::cerr << "Need one input atleast 1.PUB  2.SUB" << std::endl;
//		return 1;
//	}
//
//	if (std::string(argv[1]) == "PUB") {
//		sleep(1);
//		rt_pub pub_0(SIGRTMIN + 1, 0);
//		rt_pub pub_1(SIGRTMIN + 1, 1);
//		rt_pub pub_2(SIGRTMIN + 1, 2);
//		pid_t pid =  getpid()-1;
//		pub_0.add_subscriber(pid);
//		pub_1.add_subscriber(pid);
//		pub_2.add_subscriber(pid);
//
//		var_space.write<float>((const float&) 13, 0);
//		pub_0.notify();
//
//		for (char i = 0; i < 10; i++) {
//			var_space.write<char>((const char&) i, 1);
//			pub_1.notify();
//			sleep(1);
//
//		}
//		//		benchmark_us.stop();
//		var_space.write<char>((const char&) 19, 2);
//		pub_2.notify();
//		sleep(1);
//	} else if (std::string(argv[1]) == "SUB") {
//
//		rt_sub *sub = rt_sub::init(SIGRTMIN + 1, rt_sub_handler);
//		auto i = 0;
//		while(1){
//			sub->unblock_n_await();
//		}
//	}
//}
//

int main(int argc, char **argv) {
	pid_t pid { };

	if ((pid = fork()) == 0) {
		rt_sub *sub = rt_sub::init(SIGRTMIN + 1, rt_sub_handler);
		//
		while(1){
			sub->await();
		}

	}else{
	sleep(1);
	rt_pub pub_0(SIGRTMIN + 1, 0);
	rt_pub pub_1(SIGRTMIN + 1, 1);
	rt_pub pub_2(SIGRTMIN + 1, 2);

	pub_0.add_subscriber(pid);
	pub_1.add_subscriber(pid);
	pub_2.add_subscriber(pid);

	var_space.write<float>((const float&) 13, 0);
	pub_0.notify();

	for (auto i = 0; i < 1000000; i++) {
		var_space.write<char>((const char&) i, 1);
		pub_1.notify();

	}
	//		benchmark_us.stop();
	var_space.write<char>((const char&) 19, 2);
	pub_2.notify();
	sleep(1);
	}
}

