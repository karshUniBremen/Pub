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


/**
 * @brief 
 * @author 
 * @since Mon Jan 04 2021
 */
class rt_sub
{
public:
	/**
  * @brief 
  * @param signo
  * @param info
  * @param extra
  * @return 
  */
	using  sig_handler = void (*)(int signo, siginfo_t *info, void *extra);


	/**
  * @brief 
  * @param 
  */
	rt_sub(const rt_sub &) = delete;
	/**
  * @brief 
  * @param 
  * @return 
  */
	rt_sub &operator=(const rt_sub &) = delete;

	/**
  * @brief 
  */
	~rt_sub() {
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);
		signal(this->sig_type, SIG_DFL);
		sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
	}

	/**
  * @brief 
  * @param signal_type
  * @param signal_handler
  * @return 
  */
	static rt_sub* init(int const signal_type, const sig_handler signal_handler) {
		if (rt_sub::instance == nullptr) {
			rt_sub::instance = new rt_sub{signal_type, signal_handler};
		}
		return rt_sub::instance;
	}

	/**
  * @brief 
  * @return 
  */
	static rt_sub* getInstance() {
		return rt_sub::instance;
	}
	/**
  * @brief 
  * @return (void)
  */
	void block() {
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	}

	/**
  * @brief 
  * @return (void)
  */
	void unblock() {
		sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
	}

	/**
  * @brief 
  * @return (void)
  */
	void unblock_n_await() {
		sigprocmask(SIG_UNBLOCK, &newmask, &oldmask);
		sigsuspend (&oldmask);
	}

	/**
  * @brief 
  * @return (void)
  */
	void await() {
		sigsuspend (&oldmask);
	}

private:
	const int sig_type;
	const sig_handler handler;
	sigset_t newmask;
	sigset_t oldmask;
	struct sigaction action;
	inline static rt_sub *instance = nullptr;
	/**
 * @brief
 * @param signal_type
 * @param signal_handler
 */
	explicit rt_sub(int const signal_type, const sig_handler signal_handler)
	: sig_type{signal_type}, handler{signal_handler},newmask {},oldmask {},action {}
	{
		try
		{
			if (this->handler == nullptr)
			{
				throw std::runtime_error("handler can't be null");
			}
			sigemptyset(&newmask);
			sigaddset(&newmask, sig_type);
			sigprocmask(SIG_BLOCK, &newmask, &oldmask);
			action.sa_flags = SA_SIGINFO;
			action.sa_sigaction = this->handler;
			if (sigaction(sig_type, &action, nullptr) == -1)
			{
				throw std::runtime_error("Error during signal setup");
			}
		}
		catch (std::exception const &e)
		{
			std::clog << "Exception: " << e.what() << "\n";
		}
	}
};

#endif /* RT_SUB_HPP_ */
