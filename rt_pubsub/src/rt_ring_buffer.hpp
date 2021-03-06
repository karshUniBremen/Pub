#ifndef RT_RING_BUFFER_HPP_
#define RT_RING_BUFFER_HPP_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <array>
#include <atomic>

template<class T, size_t RING_SIZE>
class ringbuf{
	std::atomic<size_t> curSize;
	std::array<std::atomic<T>,RING_SIZE> ring;
	std::atomic<size_t> read_index;
	std::atomic<size_t> write_index;

public:
	ringbuf(const ringbuf&) = delete;
	ringbuf& operator=(const ringbuf&) = delete;

	ringbuf():curSize{0},read_index{0},write_index{0}
	{

	}

	void write(const T& data) noexcept{
		volatile size_t index_val =  write_index.load(std::memory_order_acquire);
		if(index_val == RING_SIZE){
			ring[0].store(data, std::memory_order_release);
			write_index.store(1,std::memory_order_release);
			curSize.fetch_add(1, std::memory_order_acq_rel);
		}else{
			ring[index_val].store(data, std::memory_order_release);
			write_index.fetch_add(1, std::memory_order_acq_rel);
			curSize.fetch_add(1, std::memory_order_acq_rel);
		}
	}

	bool read(T& data) noexcept{
		if(curSize == 0){
			return false;
		}

		volatile size_t index_val =  read_index.load(std::memory_order_acquire);
		if(index_val == RING_SIZE){
			data = ring[0].load(std::memory_order_acquire);
			read_index.store(1,std::memory_order_release);
			curSize.fetch_sub(1, std::memory_order_acq_rel);
		}else{
			data = ring[index_val].load(std::memory_order_acquire);
			read_index.fetch_add(1,std::memory_order_acq_rel);
			curSize.fetch_sub(1, std::memory_order_acq_rel);
		}
		return true;
	}

	bool peek(T& data) noexcept{
		if(curSize == 0){
			return false;
		}

		volatile size_t index_val =  read_index.load(std::memory_order_acquire);
		if(index_val == RING_SIZE){
			data = ring[0].load(std::memory_order_acquire);
		}else{
			data = ring[index_val].load(std::memory_order_acquire);
		}
		return true;
	}
};



#endif /* RT_RING_BUFFER_HPP_ */
