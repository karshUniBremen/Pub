/*
 * rtpubsub.hpp
 *
 *  Created on: Jan 4, 2021
 *      Author: karsh
 */

#ifndef RTPUBSUB_HPP_
#define RTPUBSUB_HPP_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <list>
#include <stdexcept>

class rt_pub {
	std::list<pid_t> subscriber_pid_list;
	int sig_type;
	int index;
public:
	rt_pub(const rt_pub&) = delete;
	rt_pub& operator =(const rt_pub&) = delete;

	rt_pub(int signal_type, uint32_t index) :
			sig_type(signal_type), index(index) {
	}
	~rt_pub() {
	}
	void add_subscriber(pid_t pid) {
		this->subscriber_pid_list.push_back(pid);
	}
	void remove_subscriber(pid_t pid) {
		for (auto iter = subscriber_pid_list.begin();
				iter != subscriber_pid_list.end(); iter++) {
			if (*iter == pid) {
				subscriber_pid_list.erase(iter);
			}
		}
	}

	void notify(uint32_t index) {
		union sigval sig_val;
		sig_val.sival_int = (int) index;
		for (auto iter = subscriber_pid_list.begin();
				iter != subscriber_pid_list.end(); iter++) {
			sigqueue(*iter, this->sig_type, sig_val);
		}
	}
};



class rt_sub {
	typedef void (*sig_handler)(int signo, siginfo_t *info, void *extra);
private:
	sigset_t newmask;
	sigset_t oldmask;
	struct sigaction action;
	int sig_type;
	sig_handler handler;

public:
	rt_sub(const rt_sub&) = delete;
	rt_sub& operator =(const rt_sub&) = delete;
	rt_sub(int const signal_type, sig_handler signal_handler = NULL) :
			sig_type(signal_type), handler(signal_handler) {
		try {
			if (this->handler == NULL) {
				throw std::runtime_error("handler can't be null");
			}
			sigemptyset(&newmask);
			sigaddset(&newmask, sig_type);
			sigprocmask(SIG_BLOCK, &newmask, &oldmask);
			action.sa_flags = SA_SIGINFO;
			action.sa_sigaction = this->handler;
			if (sigaction(sig_type, &action, NULL) == -1) {
				throw std::runtime_error("Error during signal setup");
			}

		} catch (std::exception const &e) {
			std::cout << "Exception: " << e.what() << "\n";
		}
	}

	~rt_sub() {
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);
		signal (this->sig_type, SIG_DFL);
		sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
	}

	void block(){
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	}

	void unblock(){
		sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
	}

	void unblock_n_await() {
		sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
		sigsuspend(&oldmask);
	}

	void await(){
		sigsuspend(&oldmask);
	}
};

#endif /* RTPUBSUB_HPP_ */
