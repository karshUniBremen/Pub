 //This file is generated, Please dont modify the content, Change config.yaml file and generate code

#include "data_dict.hpp" 
 #include "shm_block.hpp" 
 #include <sys/types.h> 
 #include <sys/wait.h> 
 #include <utility> 

shm_block var_space("/dev/shm/var",65,1024);
shm_block proc_space("/dev/shm/proc",64,128);
volatile variable_t<float> var_index_0(0.0);
variable_info info_index_0(0,0,dtype_t::FLOAT32);
volatile variable_t<int8_t> var_index_1(1);
variable_info info_index_1(1,4,dtype_t::SIGNED8);
volatile variable_t<int8_t> var_index_2(2);
variable_info info_index_2(2,5,dtype_t::SIGNED8);
std::array<variable_info*,3> indices { &info_index_0,&info_index_1,&info_index_2};
void rt_sub_handler(int signo, siginfo_t *info, void *extra){
switch(info->si_value.sival_int){
	case 0:
		var_space.read<float>(0,var_index_0);
	 break;
	case 1:
		var_space.read<int8_t>(1,var_index_1);
	 break;
	case 2:
		var_space.read<int8_t>(2,var_index_2);
	 break;

	}
}
