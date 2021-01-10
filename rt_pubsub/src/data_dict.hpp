////This file is generated, Please dont modify the content, Change config.yaml file and generate code
//
//#ifndef DATA_DICT_HPP_
//#define DATA_DICT_HPP_
//
//#include "shm_variables.hpp"
//
//extern variable_t<float> var_index_0;
//extern variable_t<int8_t> var_index_1;
//extern variable_t<int8_t> var_index_2;
//
//extern void rt_sub_handler(int signo, siginfo_t *info, void *extra);
//
//#endif

 //This file is generated, Please dont modify the content, Change config.yaml file and generate code

#ifndef DATA_DICT_HPP_
#define DATA_DICT_HPP_
#include "shm_variables.hpp"
#include <sys/epoll.h>


#define ENABLE_BENCHMARK_TEST
//#define ENABLE_RINGBUF_TEST

#define ENABLE_RINGBUF
//#define ENABLE_EVENT_POLL
//#define ENABLE_DIRECT_USER_NOTIFY_CB_CALL

#ifdef ENABLE_BENCHMARK_TEST
#define PUB_SUB_ITERATION_MAX_CNT 1000
#define READ_TEST_VAL 9
#define CSV
#endif

#ifdef ENABLE_RINGBUF_TEST
#define PUB_SUB_ITERATION_MAX_CNT 100
#endif

#ifdef ENABLE_EVENT_POLL
extern int event_poll_file_descriptor;
extern epoll_event event;
extern int user_app_file_descriptor;
#endif

#ifdef ENABLE_RINGBUF
#include "rt_ring_buffer.hpp"
#define RING_BUFFER_SIZE 100
extern ringbuf<int,RING_BUFFER_SIZE> user_notification_queue;
#endif

extern void rt_sub_handler(int signo, siginfo_t *info, void *extra);


extern variable_t<float> var_index_0;
extern variable_t<int8_t> var_index_1;
extern variable_t<int8_t> var_index_2;


#endif
