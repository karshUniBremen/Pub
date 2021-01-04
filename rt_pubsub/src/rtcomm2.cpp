//============================================================================
// Name        : rtcomm.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//Reference : https://www.cs.auckland.ac.nz/references/unix/digital/APS33DTE/DOCU_006.HTM


#include "data_dict.h"
#include "rtpubsub.hpp"
#include <iostream>
#include <ostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int shm_test(){
	shm_block mem_block("/dev/null",65,1024);
	char wval = 10;
	char rval = 0;
	mem_block.write(shm_variables[1],(void*)&wval,sizeof(wval));
	mem_block.read(shm_variables[1],(void*)&rval,sizeof(rval));
	cout<<int(rval)<<endl;
	return 0;
}


pid_t pid;
shm_block mem_block("/dev/null",65,1024);

void rt_sub_handler(int signo, siginfo_t *info, void *extra){
	int int_val = info->si_value.sival_int;
	char rval = 0;
	mem_block.read(shm_variables[int_val],(void*)&rval,sizeof(rval));
	std::cout<<(int)rval<<std::endl;
}

int main(){
	char wval = 15;

	if((pid = fork())==0){
		rt_sub sub(SIGRTMIN+1,rt_sub_handler);
		sub.await();
	}else{
		rt_pub pub(SIGRTMIN+1,0);
		sleep(1);
		mem_block.write(shm_variables[0],(void*)&wval,sizeof(wval));
		pub.add_subscriber(pid);
		pub.notify(0);
	}
}

