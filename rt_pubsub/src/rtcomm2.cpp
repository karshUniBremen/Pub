//============================================================================
// Name        : rtcomm.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//Reference : https://www.cs.auckland.ac.nz/references/unix/digital/APS33DTE/DOCU_006.HTM

#include "data_dict.hpp"
#include "rtpubsub.hpp"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


using namespace std;

int shm_test(){
	index_1.value = 12.80;
	var_space.write(index_1);
	var_space.read(index_1);
	clog<<float(index_1.value)<<endl;
	return 0;
}


bool end = false;

void rt_sub_handler(int signo, siginfo_t *info, void *extra){
	var_space.read(index_1);
	std::clog<<int(index_1.value)<<std::endl;
}

void pub_sub_test(){
	pid_t pid {0};
	if((pid = fork())==0){
		rt_sub *sub = rt_sub::init(SIGRTMIN+1,rt_sub_handler);
		sub->await();
	}else{
		sleep(0.5);
		rt_pub* pub = rt_pub::init(SIGRTMIN+1,1);
		index_1.value = 17;
		var_space.write(index_1);
		pub->add_subscriber(pid);
		pub->notify();
	}
}


int main(){
//	shm_test();
	pub_sub_test();
}

