#ifndef RT_PUB_HPP_
#define RT_PUB_HPP_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <list>
#include <stdexcept>
#include <iostream>

/**
 * @brief real-time publisher
 * @author karthik
 * @since Mon Jan 04 2021
 */
class rt_pub
{
	std::list<pid_t> subscriber_pid_list; /** */
	int const sig_type;					  /** */
	int const var_id;					  /** */

public:
	/**
	 * @brief copy constructor deleted
	 */
	rt_pub(const rt_pub &) = delete;

	/**
	 * @brief assignment operator deleted
	 */
	rt_pub &operator=(const rt_pub &) = delete;

	/**
  * @brief constructor of real-time publisher
  * @param signal_type Linux signal number
  * @param index index variable to which publisher belongs
  */
	rt_pub(int const signal_type, int const var_id) : sig_type(signal_type), var_id(var_id)
	{
	}

	/**
  * @brief destructor
  */
	~rt_pub(){};

	/**
  * @brief add subscriber to the subscriber list of the publisher
  * @param pid process id of the subscriber
  * @return (void)
  */
	void add_subscriber(const pid_t pid)
	{
		this->subscriber_pid_list.push_back(pid);
	}

	/**
  * @brief remove subscriber from subscriber list of the publisher
  * @param pid process id
  * @return (void)
  */
	void remove_subscriber(const pid_t pid)
	{
		for (auto iter = subscriber_pid_list.begin(); iter != subscriber_pid_list.end(); iter++)
		{
			if (*iter == pid)
			{
				subscriber_pid_list.erase(iter);
			}
		}
	}

	/**
  * @brief notify all subscriber
  * @return (void)
  */
	void notify()
	{
		union sigval sig_val
		{
		};
		sig_val.sival_int = (int)(this->var_id);
		for (auto iter = subscriber_pid_list.begin(); iter != subscriber_pid_list.end(); iter++)
		{
			sigqueue(*iter, (int)this->sig_type, sig_val);
		}
	}

//	/**
//	 * @brief gets pid and status of the subscriber
//	 * @param subscriber_id subscriber id
//	 * @param pid process id of subscriber read from memory
//	 * @param status
//	 */
//	static void get_listener_info(const uint32_t subscriber_id, pid_t& pid, uint8_t& status){
//		uint32_t offset = subscriber_id*(sizeof(pid_t)+sizeof(uint8_t));
//		service_var_space.read(offset, pid);
//		service_var_space.read(offset+sizeof(pid_t), status);
//	}

	/**
	 * @brief gets pid and status of the subscriber
	 * @param subscriber_id subscriber id
	 * @param pid process id of subscriber read from memory
	 */
	static void get_listener_info(const uint32_t subscriber_id, pid_t& pid){
		uint32_t offset = subscriber_id*(sizeof(pid_t)+sizeof(uint8_t));
		service_var_space.read(offset, pid);
	}
};

#endif /* RT_PUB_HPP_ */
