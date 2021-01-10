//This file is generated, Please dont modify the content, Change config.yaml file and generate code

#include "data_dict.hpp"
#include "shm_block.hpp"
#include "rt_sub.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <utility>

#ifdef ENABLE_BENCHMARK_TEST
#include "benchmark.hpp"
Timer benchmark_us;
#endif

#ifdef ENABLE_EVENT_POLL
int event_poll_file_descriptor = epoll_create(1);
epoll_event event = { .events = EPOLLOUT };
int user_app_file_descriptor = -1;
#endif

#ifdef ENABLE_RINGBUF
ringbuf<int,RING_BUFFER_SIZE> user_notification_queue;
#endif

shm_block process_var_space("/dev/shm/var", 65, 1024);
shm_block service_var_space("/dev/shm/proc", 64, 128);

variable_t<float> var_index_0(0, SIGRTMIN + 1, 0, dtype_t::FLOAT32);
variable_t<int8_t> var_index_1(1, SIGRTMIN + 1, 4, dtype_t::SIGNED8);
variable_t<int8_t> var_index_2(2, SIGRTMIN + 1, 5, dtype_t::SIGNED8);

void rt_sub_handler(int signo, siginfo_t *info, void *extra) {
//	sigset_t newmask,oldmask;
//	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	switch (info->si_value.sival_int) {
	case 0:
		var_index_0.update();
#ifdef ENABLE_RINGBUF
		user_notification_queue.write(0);
#endif
#ifdef ENABLE_EVENT_POLL
		rt_sub::user_app_notify(0);
#endif
#ifdef ENABLE_DIRECT_USER_NOTIFY_CB_CALL
		rt_sub::user_app_notify(0);
#endif
#ifdef ENABLE_BENCHMARK_TEST
		benchmark_us.start();
#endif
		break;
	case 1:
		var_index_1.update();
#ifdef ENABLE_RINGBUF
		user_notification_queue.write(1);
#endif
#ifdef ENABLE_EVENT_POLL
		rt_sub::user_app_notify(1);
#endif
#ifdef ENABLE_DIRECT_USER_NOTIFY_CB_CALL
		rt_sub::user_app_notify(1);
#endif

		break;
	case 2:
#ifdef ENABLE_BENCHMARK_TEST
		benchmark_us.stop();
#endif
		var_index_2.update();
#ifdef ENABLE_RINGBUF
		user_notification_queue.write(2);
#endif
#ifdef ENABLE_EVENT_POLL
		rt_sub::user_app_notify(2);
#endif
#ifdef ENABLE_DIRECT_USER_NOTIFY_CB_CALL
		rt_sub::user_app_notify(2);
#endif



		break;

	}
//	sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
}

