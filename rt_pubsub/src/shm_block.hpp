#ifndef SHM_BLOCK_HPP_
#define SHM_BLOCK_HPP_

#include <iostream>
#include <atomic>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdint>
#include <cstring>
#include <exception>
#include <shared_mutex>
#include <stdexcept>

//#include "shm_variable_info_t.hpp"

//extern variable_info_t *indices[];

/**
 * @brief shared memory block class
 * @author karthik
 * @since Mon Jan 04 2021
 */
class shm_block
{
	const char *const name;		 /** */
	unsigned int const block_id; /** */
	unsigned int const size;	 /** */
	uint8_t *addr;				 /** */
	int mem_id;					 /** */

public:
	/**
	 * @brief copy constructor is deleted
	 */
	shm_block(const shm_block &) = delete;

	/**
	 * @brief assignment operator is deleted
	 */
	shm_block &operator=(const shm_block &) = delete;

	/**
	 * @brief constructor
	 * @param name user defined name to the shared memory
	 * @param block_id user defined ID to the shared memory
	 * @param block_size size of the shared memory
	 */
	explicit shm_block(const char *const name, unsigned int const block_id, unsigned int const block_size) : name{name}, block_id{block_id}, size{block_size}, addr{nullptr}, mem_id{-1}
	{

		try
		{
			key_t const key = ftok(this->name, this->block_id);

			this->mem_id = shmget(key, this->size, 0666 | IPC_CREAT);
			if (this->mem_id == -1)
			{
				throw std::runtime_error("throw shared memory get failed");
			}

			int* ret = (int*) shmat(this->mem_id, (void*) 0, 0);
			if (ret == (int*)-1)
			{
				throw std::runtime_error("shared memory attach failed");
			}
			else
			{
				this->addr = (uint8_t *)ret;
			}
		}
		catch (std::exception const &e)
		{
			std::clog << "Exception: " << e.what() << "\n";
		}
	}

	/**
  * @brief destructor
  */
	~shm_block() noexcept
	{
		if (this->addr != nullptr)
		{
			if (shmdt(this->addr) == -1)
			{
				std::clog << "Shared memory cannot be detached\n";
			}
			shmctl(this->mem_id, IPC_RMID, nullptr);
		}
	}

	/**
	 * @brief reads value from shared memory location of specified index variable
	 * @param index index  of the variable
	 * @param val variable in which read value is stored
	 * @return void
	 */
	template <typename T>
	void read(const uint32_t &offset, T &val) const
	{
		volatile std::atomic<T> atm_var(0);
		T *ptr = (T *)(this->addr + offset);
		atm_var.store(*ptr, std::memory_order_seq_cst);
		val = atm_var.load(std::memory_order_seq_cst);
	}

	/**
	 * @brief reads value from shared memory location of specified index variable
	 * @param index index  of the variable
	 * @param val atomic variable in which read value is stored
	 * @return
	 */
	template <typename T>
	void read(const uint32_t &offset, volatile std::atomic<T> &val) const
	{
		T *ptr = (T *)(this->addr + offset);
		val.store(*ptr, std::memory_order_seq_cst);
	}

	/**
	 * @brief write data to shared memory of variable with specified index
	 * @param index index of the variable
	 * @param value value to be written
	 * @return void
	 */
	template <typename T>
	void write(const uint32_t &offset, const T &value) const
	{
		volatile std::atomic<T> atm_var(value);
		T *ptr = (T *)(this->addr + offset);
		*ptr = atm_var.load(std::memory_order_seq_cst);
	}

	/**
	 * @brief write data to shared memory of variable with specified index
	 * @param index index index of the variable
	 * @param value atomic variable containing value to be written
	 * @return
	 */
	template <typename T>
	void write_(const uint32_t &offset, std::atomic<T> &value) const
	{
		T *ptr = (T *)(this->addr + offset);
		*ptr = value.load(std::memory_order_seq_cst);
	}
};

extern shm_block process_var_space;	 /** variable space in shared memory */
extern shm_block service_var_space; /** process space in shared memory*/

#endif /* SHM_BLOCK_HPP_ */
