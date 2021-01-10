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

#ifdef ENABLE_RINGBUF_TEST
#include "benchmark.hpp"
#include "rt_ring_buffer.hpp"
Timer benchmark_us;
#endif


#ifdef ENABLE_EVENT_POLL
#include <sys/epoll.h>
#endif

using namespace std;


void test_fd()
{
#ifdef ENABLE_EVENT_POLL
	int read_buffer=1;
	int event_cnt = epoll_wait(event_poll_file_descriptor, &event, 1, 1000);
	if(event_cnt > 0){
		if(read(user_app_file_descriptor, &read_buffer, sizeof(int))>0)
			std::clog << read_buffer << std::endl;
	}
#endif
}

void test_direct_usr_notify_cb(int index)
{
#ifdef ENABLE_DIRECT_USER_NOTIFY_CB_CALL
	//	cout<<"Index: "<< index <<endl;
#endif
}

void test_usr_notify_queue(int index)
{
#ifdef ENABLE_RINGBUF
	int index_val = 0;
	if(user_notification_queue.read(index_val)==true){
//		cout<<"Index: "<< index_val <<endl;
	}
#endif
}

void benchmark_test(){
#ifdef ENABLE_BENCHMARK_TEST
	pid_t pid{};// stores child's process ID
	int8_t newValue = 0;

	if ((pid = fork()) == 0)
	{
		//This is a child process

		//Create and initialize the subscriber
		rt_sub* const sub = rt_sub::init(1, SIGRTMIN + 1, rt_sub_handler);
#ifdef ENABLE_DIRECT_USER_NOTIFY_CB_CALL
#ifdef ENABLE_RINGBUF
		sub->registerUserAppNotificationCB(test_usr_notify_queue);
#else
		sub->registerUserAppNotificationCB(test_direct_usr_notify_cb);
#endif
#endif

		sub->unblock();
		while (1)
		{
			// wait for signal to receive. This stop process from terminating
			test_fd();
			usleep(1);
		}
	}
	else
	{
		sleep(1); // wait for subscriber to join

		// Add subscriber to index 0,1,2 variables
		pid_t sub_1_pid {};
		rt_pub::get_listener_info(1, sub_1_pid);

		if(sub_1_pid != 0){
			var_index_0.add_subscriber(sub_1_pid);
			var_index_1.add_subscriber(sub_1_pid);
			var_index_2.add_subscriber(sub_1_pid);
		}

		//Write data to shared memory and notify all subscriber of index 0
		var_index_0.write(12.3);

		for (auto i = 0; i < PUB_SUB_ITERATION_MAX_CNT; i++)
		{
			//Write data to shared memory and notify all subscriber of index 1
			var_index_1.write(i);
		}

		//Write data to shared memory and notify all subscriber of index 2
		var_index_2.write(READ_TEST_VAL);
		// Read latest
		var_index_2.read(newValue);
		//		clog<< "Read value expected: "<< READ_TEST_VAL<<" Test reading: "<<static_cast<int>(newValue) << endl;

		//Wait for 2 seconds for child process to print benchmark results, then kill child process
		sleep(1);
		kill(pid,SIGTERM);
	}
#endif
}


void test_rt_ring_buffer(){
#ifdef ENABLE_RINGBUF_TEST
	ringbuf<int,PUB_SUB_ITERATION_MAX_CNT> rngbuf;
	int rval = 0;
	benchmark_us.start();
	for(auto i=0;i<PUB_SUB_ITERATION_MAX_CNT;i++){
		rngbuf.write(i);
	}

	for(auto i=0;i<PUB_SUB_ITERATION_MAX_CNT;i++){

		rngbuf.read(rval);
//		std::clog<<rval<<"\n";
	}
	benchmark_us.stop();
#endif
}
/**
 * @brief Example application
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{
#ifdef ENABLE_BENCHMARK_TEST
	benchmark_test();
#endif
#ifdef ENABLE_RINGBUF_TEST
	test_rt_ring_buffer();
#endif
}
