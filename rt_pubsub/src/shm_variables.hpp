#ifndef SHM_VARIABLES_HPP_
#define SHM_VARIABLES_HPP_

#include "shm_block.hpp"
#include "rt_pub.hpp"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <atomic>

/**
 * @brief enumeration of data type
 */
using dtype_t = enum {UNSIGNED8 = 0, /** unsigned 8 bits */
	UNSIGNED16,/** unsigned 16 bits */
	UNSIGNED32,/** unsigned 32 bits */
	UNSIGNED64,/** unsigned 64 bits */
	SIGNED8,/** signed 8 bits */
	SIGNED16,/** signed 16 bits */
	SIGNED32,/** signed 32 bits */
	SIGNED64,/** signed 64 bits */
	FLOAT32,/** float 32 bits */
	FLOAT64 /** float 64 bits or double */};

/**
 * @brief Data dictionary variable type
 * @author Karthik
 * @since Fri Jan 08 2021
 */
template<class T>
class variable_t {
	const int index; /** data dictionary index of the variable */
	volatile std::atomic<T> value_store; /**value storage of the variable */
	rt_pub pub; /**Publish object of the variable */
	const uint32_t mem_offset; /** */
	const dtype_t type_info; /** */
	/**
	 * @brief Notify all subscriber attached to this variable
	 * @return (void)
	 */
	void notify() {
		pub.notify();
	}

public:
	/**
	 * @brief constructor
	 * @param index index value of the variable
	 * @param signal_type Linux signal value
	 */
	explicit variable_t(int const &index, int const signal_type, const uint32_t mem_offset, const dtype_t type_info) :
			index { index }, pub { signal_type, index }, mem_offset { mem_offset }, type_info { type_info } {
		var_space.write(this->mem_offset, value_store);
	}

	/**
	 * @brief Writes data into shared memory and notifies all subscriber
	 * @param value value to be written
	 * @return (void)
	 */
	void write(const T &value) {
		value_store.store(value);
		var_space.write(this->mem_offset, value_store);
		pub.notify();
	}

	/**
	 * @brief reads data from shared memory
	 * @param value value read from value store (not from shared memory)
	 * @return (void)
	 */
	void read(T &value) {
		// var_space.read(index,var);
		value = value_store.load(std::memory_order_relaxed);
	}

	/**
	 * @brief updates value store of the variable from shared memory (called inside signal handler only)
	 * @return (void)
	 */
	void update() {
		var_space.read(this->mem_offset, value_store);
	}

	/**
	 * @brief adds subscriber to the variable subscriber list
	 * @param pid process ID of the subscriber
	 * @return (void)
	 */
	void add_subscriber(const pid_t pid) {
		pub.add_subscriber(pid);
	}

	/**
	 * @brief removes subscriber from the subscriber list of the variable
	 * @param pid process ID of the subscriber
	 * @return (void)
	 */
	void remove_subscriber(const pid_t pid) {
		pub.remove_subscriber(pid);
	}

	/**
	 * @brief gets index value of the variable
	 * @return index value in int
	 */
	const int getindex() const {
		return index;
	}

	/**
	 * @brief gets offset value of memory location from shared memory's base address
	 * @return mem_offset value in uint32_t
	 */
	const uint32_t getoffset() const {
		return mem_offset;
	}

	/**
	 * @brief gets data type information of the variable
	 * @return uint8_t type value which enumerated by dtype_t
	 */
	const uint8_t getTypeInfo() {
		return (uint8_t) type_info;
	}
};

#endif /* SHM_VARIABLES_HPP_ */
