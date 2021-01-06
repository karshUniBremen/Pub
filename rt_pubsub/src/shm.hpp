#ifndef SHM_HPP_
#define SHM_HPP_

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


using Lock = std::shared_timed_mutex;
using WriteLock = std::unique_lock<Lock>;
using ReadLock = std::shared_lock<Lock>;

using dtype_t = enum {UNSIGNED8 = 0, UNSIGNED16, UNSIGNED32, UNSIGNED64, SIGNED8, SIGNED16, SIGNED32, SIGNED64, FLOAT32, FLOAT64};

template <typename T> struct shm_variable_t
{
	const int index;
	dtype_t type;
	std::atomic<T> value;
};

/**
 * @brief
 * @author
 * @since Mon Jan 04 2021
 */
class shm_block {
	const char *const name;
	unsigned int const block_id;
	unsigned int const size;
	uint8_t *addr;
	int mem_id;
	Lock mem_lock;

public:
	shm_block(const shm_block&) = delete;
	shm_block& operator=(const shm_block&) = delete;

	/**
	 * @brief
	 * @param name
	 * @param block_id
	 * @param block_size
	 */
	explicit shm_block(const char *const name, unsigned int const block_id, unsigned int const block_size) :
			name{name}, block_id{block_id}, size{block_size}, addr{nullptr}, mem_id { -1 } {

		try {
			key_t const key = ftok(this->name, this->block_id);

			this->mem_id = shmget(key, this->size, 0666 | IPC_CREAT);
			if (this->mem_id == -1) {
				throw std::runtime_error("throw shared memory get failed");
			}

			int *const ret = (int*) shmat(this->mem_id, (void*) 0, 0);
			if (*ret == -1) {
				throw std::runtime_error("shared memory attach failed");
			} else {
				this->addr = (uint8_t*) ret;
			}
		} catch (std::exception const &e) {
			std::clog << "Exception: " << e.what() << "\n";
		}
	}

	/**
	 * @brief
	 */
	~shm_block() noexcept
	{
		if (this->addr != nullptr) {
			if (shmdt(this->addr) == -1) {
				std::clog << "Shared memory cannot be detached\n";
			}
			shmctl(this->mem_id, IPC_RMID, nullptr);
		}
	}

	/**
	 * @brief
	 * @param var
	 * @param store_loc
	 * @param store_len
	 * @return
	 */
	template<typename T>
	void read(shm_variable_t<T>& var) const {
		T* ptr = (T*)(this->addr + var.index);
		var.value.store(*ptr,std::memory_order_relaxed);
	}

	/**
	 * @brief
	 * @param var
	 * @param store_loc
	 * @param store_len
	 * @return
	 */
	template<typename T>
	void write(shm_variable_t<T> &var) const {
		T* ptr = (T*)(this->addr + var.index);
		*ptr = (T) var.value.load(std::memory_order_relaxed);
	}
};



#endif /* SHM_HPP_ */
