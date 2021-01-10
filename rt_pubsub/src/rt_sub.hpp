/*
 * rtpubsub.hpp
 *
 *  Created on: Jan 4, 2021
 *      Author: karsh
 */

#ifndef RT_SUB_HPP_
#define RT_SUB_HPP_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <list>
#include <stdexcept>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <atomic>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <fcntl.h>

extern int event_poll_file_descriptor;
extern epoll_event events;

/**
 * @brief real-time subscriber
 * @author 
 * @since Mon Jan 04 2021
 */
class rt_sub {
public:

	using user_app_notification_cb_t = void(*)(int index);

	/**
	 * @brief signal handler function type
	 * @param signo signal number
	 * @param info signal information
	 * @param extra extra
	 * @return void
	 */
	using sig_handler = void (*)(int signo, siginfo_t *info, void *extra);

	/**
	 * @brief copy constructor deleted
	 */
	rt_sub(const rt_sub&) = delete;

	/**
	 * @brief assignment operator deleted
	 */
	rt_sub& operator=(const rt_sub&) = delete;

	/**
	 * @brief destrucor
	 */
	~rt_sub() {
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);
		signal(this->sig_type, SIG_DFL);
		sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
	}

	/**
	 * @brief initialization
	 * @param signal_type signal value to be registered
	 * @param signal_handler signal handler to be attached
	 * @return
	 */
	static rt_sub* init(const uint32_t subscriber_id, int const signal_type, const sig_handler signal_handler) {
		if (rt_sub::instance == nullptr) {
			rt_sub::instance = new rt_sub { subscriber_id, signal_type, signal_handler };
		}
		return rt_sub::instance;
	}

	/**
	 * @brief gets instance of the subscriber
	 * @return pointers to the subscriber instance
	 */
	static rt_sub* getInstance() {
		return rt_sub::instance;
	}
	/**
	 * @brief blocks the signals
	 * @return (void)
	 */
	void block() {
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	}

	/**
	 * @brief unblocks the signal
	 * @return (void)
	 */
	void unblock() {
		sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
	}

//	/**
//	 * @brief unblocks and awaits for the signal
//	 * @return (void)
//	 */
//	void unblock_n_await() {
//		sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
//		sigsuspend(&oldmask);
//	}
//
//	/**
//	 * @brief awaits for the signal
//	 * @return (void)
//	 */
//	void await() {
//		sigsuspend(&oldmask);
//	}

	static void user_app_notify(int index) {
#ifdef ENABLE_USER_APP_EVENT
		if(user_app_file_descriptor!=-1){
			write(user_app_file_descriptor,&index,sizeof(index));
		}
#endif
#ifdef ENABLE_DIRECT_USER_NOTIFY_CB_CALL
		rt_sub* const instance = rt_sub::getInstance();
		if(instance != nullptr && instance->user_app_notification_cb != nullptr){
			instance->user_app_notification_cb(index);
		}
#endif
	}

	static void registerUserAppNotificationCB(user_app_notification_cb_t cb) {
		rt_sub* const instance = rt_sub::getInstance();
		if (instance != nullptr && cb != nullptr) {
			instance->setUserAppNotificationCb(cb);
		}
	}

	static void deregisterUserAppNotificationCB() {
		rt_sub* const instance = rt_sub::getInstance();
		if (instance != nullptr) {
			instance->setUserAppNotificationCb(nullptr);
		}
	}

//	/**
//	 * @brief stop listening, by writing in service var space memory corresponding to subscriber id
//	 * 		  this let all publisher know and they wont send messages
//	 */
//	void stop_listening(){
//		pid.store(0,std::memory_order_relaxed);
//		listen_status.store(0,std::memory_order_relaxed);
//		service_var_space.write(mem_offset,pid);
//		service_var_space.write(mem_offset+sizeof(pid),listen_status);
//	}

private:
	const int sig_type; /** signal number */
	const sig_handler handler; /** signal handler*/
	sigset_t newmask; /** new mask for signal*/
	sigset_t oldmask; /** old mask for signal*/
	struct sigaction action; /** signal action */
	const uint32_t subscriber_id;/** subscriber id */
	uint32_t mem_offset;/** subscriber id store memory offset in service var space*/
	volatile std::atomic<pid_t> pid;/** process id of subscriber */
	volatile std::atomic<uint8_t> listen_status;/** listening status of subscriber */
	inline static rt_sub *instance = nullptr; /** subscriber instance*/
	user_app_notification_cb_t user_app_notification_cb;
	/**
	 * @brief constructor
	 * @param signal_type signal number of the subscriber signal
	 * @param signal_handler signal handler
	 */
	explicit rt_sub(const uint32_t subscriber_id, int const signal_type, const sig_handler signal_handler) :
			sig_type { signal_type }, handler { signal_handler }, newmask { }, oldmask { }, action { }, subscriber_id(subscriber_id), mem_offset(0), user_app_notification_cb(
					nullptr) {
		try {
			if (this->handler == nullptr) {
				throw std::runtime_error("handler can't be null");
			}
#ifdef ENABLE_USER_APP_EVENT
			if (event_poll_file_descriptor == -1) {
				throw std::runtime_error("Failed to create epoll file descriptor\n");
			}
#endif
			mem_offset = this->subscriber_id * (sizeof(pid) + sizeof(listen_status));
			pid.store(getpid(), std::memory_order_relaxed);
			listen_status.store(1, std::memory_order_relaxed);
			service_var_space.write(mem_offset, pid);
			service_var_space.write(mem_offset + sizeof(pid), listen_status);

#ifdef ENABLE_USER_APP_EVENT
			std::string str =  std::string("sub_")+ std::to_string(subscriber_id);
			user_app_file_descriptor =  open(str.c_str(), O_RDWR); //memfd_create(str.c_str(),1);//
			if(user_app_file_descriptor == -1){
				throw std::runtime_error("Failed to create file descriptor \n");
			}

			event.data.fd = user_app_file_descriptor;
			if (epoll_ctl(event_poll_file_descriptor, EPOLL_CTL_ADD, user_app_file_descriptor, &event)==-1) {
				close(event_poll_file_descriptor);
				throw std::runtime_error("Failed to add file descriptor to epoll\n");
			}
#endif
			sigemptyset(&newmask);
			sigaddset(&newmask, sig_type);
			sigprocmask(SIG_BLOCK, &newmask, &oldmask);
			action.sa_flags = SA_SIGINFO;
			action.sa_sigaction = this->handler;
			if (sigaction(sig_type, &action, nullptr) == -1) {
				throw std::runtime_error("Error during signal setup");
			}
		} catch (std::exception const &e) {
			std::clog << "Exception: " << e.what() << "\n";
		}
	}

	void setUserAppNotificationCb(user_app_notification_cb_t userAppNotificationCb) {
		user_app_notification_cb = userAppNotificationCb;
	}

};

#endif /* RT_SUB_HPP_ */
