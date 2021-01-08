#ifndef SHM_VARIABLES_HPP_
#define SHM_VARIABLES_HPP_

#include "shm_block.hpp"
#include "rt_pub.hpp"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <atomic>

/**
 * @brief Data dictionary variable type
 * @author Karthik
 * @since Fri Jan 08 2021
 */
template <class T>
class variable_t
{
	const int index;			 			/** data dictionary index of the variable */
	volatile std::atomic<T> value_store; 	/**value storage of the variable */
	rt_pub pub;					 			/**Publish object of the variable */

	/**
  * @brief Notify all subscriber attached to this variable
  * @return (void)
  */
	void notify()
	{
		pub.notify();
	}

public:
	/**
  * @brief constructor
  * @param index index value of the variable
  * @param signal_type Linux signal value
  */
	explicit variable_t(int const &index, int const signal_type) : index{index}, pub{signal_type, index}
	{
		var_space.write(index, value_store);
	}

	/**
  * @brief Writes data into shared memory and notifies all subscriber
  * @param value value to be written
  * @return (void)
  */
	void write(const T &value)
	{
		value_store.store(value);
		var_space.write(index, value_store);
		pub.notify();
	}

	/**
  * @brief reads data from shared memory
  * @param value value read from value store (not from shared memory)
  * @return (void)
  */
	void read(T &value)
	{
		// var_space.read(index,var);
		value = value_store.load(std::memory_order_relaxed);
	}

	/**
  * @brief updates value store of the variable from shared memory (called inside signal handler only)
  * @return (void)
  */
	void update()
	{
		var_space.read(index, value_store);
	}

	/**
  * @brief adds subscriber to the variable subscriber list
  * @param pid process ID of the subscriber
  * @return (void)
  */
	void add_subscriber(const pid_t pid)
	{
		pub.add_subscriber(pid);
	}

	/**
  * @brief removes subscriber from the subscriber list of the variable
  * @param pid process ID of the subscriber
  * @return (void)
  */
	void remove_subscriber(const pid_t pid)
	{
		pub.remove_subscriber(pid);
	}
};

#endif /* SHM_VARIABLES_HPP_ */
