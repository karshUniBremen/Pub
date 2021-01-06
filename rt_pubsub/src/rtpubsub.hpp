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
#include <iostream>


/**
 * @brief 
 * @author 
 * @since Mon Jan 04 2021
 */
class rt_pub
{
	std::list<pid_t> subscriber_pid_list;
	int const sig_type;
	int const var_id;
	static rt_pub *instance;

	/**
  * @brief 
  * @param signal_type
  * @param index
  */
	rt_pub(int const signal_type, int const index);

public:
	/**
  * @brief 
  * @param 
  */
	rt_pub(const rt_pub &) = delete;

	/**
  * @brief 
  * @param 
  * @return 
  */
	rt_pub &operator=(const rt_pub &) = delete;

	/**
  * @brief 
  */
	~rt_pub();
	/**
  * @brief 
  * @param signal_type
  * @param index
  * @return 
  */
	static rt_pub *init(int const  signal_type, int const  index);

	/**
  * @brief 
  * @return 
  */
	static rt_pub *getInstance();

	/**
  * @brief 
  * @param pid
  * @return (void)
  */
	void add_subscriber(pid_t pid);

	/**
  * @brief 
  * @param pid
  * @return (void)
  */
	void remove_subscriber(pid_t pid);

	/**
  * @brief 
  * @param index
  * @return (void)
  */
	void notify();
};






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
	~rt_sub();

	/**
  * @brief 
  * @param signal_type
  * @param signal_handler
  * @return 
  */
	static rt_sub *init(int const signal_type, const sig_handler signal_handler = nullptr);

	/**
  * @brief 
  * @return 
  */
	static rt_sub *getInstance();
	/**
  * @brief 
  * @return (void)
  */
	void block();

	/**
  * @brief 
  * @return (void)
  */
	void unblock();

	/**
  * @brief 
  * @return (void)
  */
	void unblock_n_await() ;

	/**
  * @brief 
  * @return (void)
  */
	void await();

private:
	const int sig_type;
	const sig_handler handler;
	sigset_t newmask;
	sigset_t oldmask;
	struct sigaction action;
	static rt_sub *instance;
	/**
 * @brief
 * @param signal_type
 * @param signal_handler
 */
	explicit rt_sub(int const signal_type, sig_handler signal_handler);
};

#endif /* RTPUBSUB_HPP_ */
