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
 * @brief
 * @author
 * @since Mon Jan 04 2021
 */
class rt_pub
{
	std::list<pid_t> subscriber_pid_list;
	int const sig_type;
	int const var_id;

public:
	/**
  * @brief
  * @param signal_type
  * @param index
  */
	rt_pub(int const signal_type, int const var_id) :
			sig_type(signal_type), var_id(var_id) {
	}

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
	~rt_pub(){};

	/**
  * @brief
  * @param pid
  * @return (void)
  */
	void add_subscriber(const pid_t pid) {
		this->subscriber_pid_list.push_back(pid);
	}

	/**
  * @brief
  * @param pid
  * @return (void)
  */
	void remove_subscriber(const pid_t pid) {
		for (auto iter = subscriber_pid_list.begin(); iter != subscriber_pid_list.end(); iter++) {
			if (*iter == pid) {
				subscriber_pid_list.erase(iter);
			}
		}
	}

	/**
  * @brief
  * @param index
  * @return (void)
  */
	void notify() {
		union sigval sig_val { };
		sig_val.sival_int = (int) (this->var_id);
		for (auto iter = subscriber_pid_list.begin(); iter != subscriber_pid_list.end(); iter++) {
			sigqueue(*iter, (int) this->sig_type, sig_val);
		}
	}
};





#endif /* RT_PUB_HPP_ */
