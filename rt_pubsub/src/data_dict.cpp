////This file is generated, Please dont modify the content, Change config.yaml file and generate code
//#include "benchmark.hpp"
//#include "data_dict.hpp"
//#include "shm_block.hpp"
//#include <sys/types.h>
//#include <sys/wait.h>
//#include <utility>
//
//shm_block var_space("/dev/shm/var", 65, 1024);
//shm_block proc_space("/dev/shm/proc", 64, 128);
//
//variable_t<float> var_index_0(0, SIGRTMIN + 1,0, dtype_t::FLOAT32);
//variable_t<int8_t> var_index_1(1, SIGRTMIN + 1,4, dtype_t::SIGNED8);
//variable_t<int8_t> var_index_2(2, SIGRTMIN + 1, 5, dtype_t::SIGNED8);
//
//
////variable_info_t *indices[3] = {&info_index_0, &info_index_1, &info_index_2};
//void rt_sub_handler(int signo, siginfo_t *info, void *extra)
//{
//
//	switch (info->si_value.sival_int)
//	{
//	case 0:
//		var_index_0.update();
//		benchmark_us.start();
//		break;
//	case 1:
//		var_index_1.update();
//		break;
//	case 2:
//		benchmark_us.stop();
//		var_index_2.update();
//		break;
//	}
//}

 //This file is generated, Please dont modify the content, Change config.yaml file and generate code

#include "data_dict.hpp"
#include "shm_block.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <utility>

shm_block var_space("/dev/shm/var",65,1024);
shm_block proc_space("/dev/shm/proc",64,128);



variable_t<float> var_index_0(0,SIGRTMIN + 1,0,dtype_t::FLOAT32);
variable_t<int8_t> var_index_1(1,SIGRTMIN + 1,4,dtype_t::SIGNED8);
variable_t<int8_t> var_index_2(2,SIGRTMIN + 1,5,dtype_t::SIGNED8);



void rt_sub_handler(int signo, siginfo_t *info, void *extra){
switch(info->si_value.sival_int){
	case 0:
		var_index_0.update();
	 break;
	case 1:
		var_index_1.update();
	 break;
	case 2:
		var_index_2.update();
	 break;

	}
}


