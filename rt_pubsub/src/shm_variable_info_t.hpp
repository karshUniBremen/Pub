#ifndef SHM_VARIABLE_INFO_T_HPP_
#define SHM_VARIABLE_INFO_T_HPP_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <array>
#include <atomic>

/**
 * @brief enumeration of data type
 */
using dtype_t = enum { UNSIGNED8 = 0, /** unsigned 8 bits */
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
 * @brief variable information type
 * @author karthik
 * @since Fri Jan 08 2021
 */
class variable_info_t
{
	const int index;		   /** */
	const uint32_t mem_offset; /** */
	const dtype_t type_info;   /** */

public:
	/**
	 * @brief copy constructor is deleted
	 */
	variable_info_t(const variable_info_t &) = delete;

	/**
		 * @brief asignment operator is deleted
		 */
	variable_info_t &operator=(const variable_info_t &) = delete;

	/**
  * @brief constructor
  * @param index index value of the variable
  * @param mem_offset memory offset from base address of the shared memory
  * @param type_info data type information (enumerated to dtype_t)
  * @see dtype_t
  */
	explicit variable_info_t(const int index, const uint32_t mem_offset, const dtype_t type_info)
		: index{index}, mem_offset{mem_offset}, type_info{type_info}
	{
	}

	/**
  * @brief gets index value of the variable
  * @return index value in int
  */
	const int getindex() const
	{
		return index;
	}

	/**
  * @brief gets offset value of memory location from shared memory's base address
  * @return mem_offset value in uint32_t
  */
	const uint32_t getoffset() const
	{
		return mem_offset;
	}

	/**
  * @brief gets data type information of the variable
  * @return uint8_t type value which enumerated by dtype_t
  */
	const uint8_t getTypeInfo()
	{
		return (uint8_t)type_info;
	}
};

#endif /* SHM_VARIABLE_INFO_T_HPP_ */
