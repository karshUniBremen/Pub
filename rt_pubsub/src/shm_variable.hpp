#ifndef SHM_VARIABLE_HPP_
#define SHM_VARIABLE_HPP_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <array>
#include <atomic>

using dtype_t = enum {UNSIGNED8 = 0, UNSIGNED16, UNSIGNED32, UNSIGNED64, SIGNED8, SIGNED16, SIGNED32, SIGNED64, FLOAT32, FLOAT64};

template<class T> using variable_t = std::atomic<T>;

class variable_info
{
	const int index;
	const uint32_t mem_offset;
	const dtype_t type_info;
public:
	explicit variable_info(const int index,const uint32_t mem_offset, const dtype_t type_info)
	:index{index},mem_offset{mem_offset}, type_info{type_info}{

	}

	variable_info(const variable_info&) = delete;
	variable_info& operator=(const variable_info&) = delete;

	const int getindex() const {
		return index;
	}

	const uint32_t getoffset() const{
		return mem_offset;
	}

};



#endif /* SHM_VARIABLE_HPP_ */
