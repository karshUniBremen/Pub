/*
 * data_dict.h
 *
 *  Created on: Jan 3, 2021
 *      Author: karsh
 */

#ifndef DATA_DICT_H_
#define DATA_DICT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdint>
#include <cstring>
#include <shared_mutex>
#include <thread>

typedef std::shared_timed_mutex Lock;
typedef std::unique_lock< Lock >  WriteLock;
typedef std::shared_lock< Lock >  ReadLock;

typedef enum {
	UNSIGNED8 = 0,
	UNSIGNED16,
	UNSIGNED32,
	UNSIGNED64,
	SIGNED8,
	SIGNED16,
	SIGNED32,
	SIGNED64,
	FLOAT32,
	FLOAT64,
	STRING
} dtype_t;


typedef struct {
	int index;
	dtype_t type;
	unsigned int len;
} shm_variable_t;


class shm_block {
private:
	const char *name;
	unsigned int block_id;
	unsigned int size;
	void *addr;
	int mem_id;
	Lock mem_lock;
public:
	shm_block(const shm_block&) = delete;
	shm_block& operator =(const shm_block&) = delete;

	shm_block(const char *name, unsigned int block_id, unsigned int block_size = 1024) :
			name(name), block_id((int) block_id), size(block_size), addr(
					NULL),mem_id(-1) {

		try {
			key_t key = ftok(this->name, this->block_id);

			this->mem_id = shmget(key, this->size, 0666 | IPC_CREAT);
			if (this->mem_id == -1) {
				throw std::runtime_error("throw shared memory get failed");
			}

			int *ret = (int*) shmat(this->mem_id, (void*) 0, 0);
			if (*ret == -1) {
				throw std::runtime_error("shared memory attach failed");
			} else {
				this->addr = (void*)ret;
			}
		} catch (std::exception const &e) {
			std::cout << "Exception: " << e.what() << "\n";
		}
	}

	~shm_block() noexcept {
		if (this->addr != NULL) {
			if (shmdt(this->addr) == -1) {
				//"shared memory detatch failed"
			}
			shmctl(this->mem_id, IPC_RMID, NULL);
		}
	}

	bool read(shm_variable_t var, void *store_loc, unsigned int store_len) {

		ReadLock r_lock(this->mem_lock);

		//check if store length is sufficient to copy
		if(store_len < var.len){
			return false;
		}

		//check if read boundaries
		if ((var.index + var.len) > this->size) {
			return false;
		}

		if (std::memcpy(store_loc, this->addr + var.index, var.len) == NULL) {
			return false;
		}
		return true;
	}

	bool write(shm_variable_t var, void *store_loc, unsigned int store_len) {

		WriteLock w_lock(this->mem_lock);

		//check if store length is sufficient to copy
		if(store_len < var.len){
			return false;
		}

		//check if read boundaries
		if ((var.index + var.len) > this->size) {
			return false;
		}

		if (std::memcpy(this->addr + var.index, store_loc, var.len) == NULL) {
			return false;
		}
		return true;
	}
};



const shm_variable_t shm_variables[3]={{1,UNSIGNED8,1},{2,UNSIGNED8,1},{3,UNSIGNED8,1} };

#endif /* DATA_DICT_H_ */
