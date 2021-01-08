//============================================================================
// Name        : rtcomm.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "data_dict.hpp"
#include "rt_sub.hpp"
#include <iostream>
#include <ostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <utility>
#include <atomic>

using namespace std;

/**
 * @brief Example application
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char **argv)
{
	pid_t pid{};// stores child's process ID
	int8_t newValue = 0;

	if ((pid = fork()) == 0)
	{
		//This is a child process

		//Create and initialize the subscriber
		rt_sub *sub = rt_sub::init(1, SIGRTMIN + 1, rt_sub_handler);
		while (1)
		{
			// wait for signal to receive. This stop process from terminating
			sub->await();
		}
	}
	else
	{
		sleep(1); // wait for subscriber to join

		// Add subscriber to index 0,1,2 variables
		pid_t sub_1_pid {};
		rt_pub::get_listener_info(1, sub_1_pid);

		if(sub_1_pid == 0){
			var_index_0.add_subscriber(sub_1_pid);
			var_index_1.add_subscriber(sub_1_pid);
			var_index_2.add_subscriber(sub_1_pid);
		}

		//Write data to shared memory and notify all subscriber of index 0
		var_index_0.write(12.3);

		for (auto i = 0; i < 100000; i++)
		{
			//Write data to shared memory and notify all subscriber of index 1
			var_index_1.write(i);
		}

		//Write data to shared memory and notify all subscriber of index 2
		var_index_2.write(9);

		// Read latest
		var_index_2.read(newValue);
		clog<< static_cast<int>(newValue) << endl;
	}
}
