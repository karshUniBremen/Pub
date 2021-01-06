#include "data_dict.hpp"

shm_block var_space("/dev/var",65,1024);
shm_block proc_space("/dev/proc",66,128);

shm_variable_t<int> index_1 = { .index=1, .type=UNSIGNED8, .value=12.6 };
shm_variable_t<int> index_2 = { .index=2, .type=UNSIGNED8, .value=1 };
shm_variable_t<int> index_3 = { .index=3, .type=UNSIGNED8, .value=1 };
