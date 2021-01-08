import os
import yaml
import cogapp as cog


def generate_data_dict_cpp(yaml_file_name):
    if os.path.exists( yaml_file_name ):
        with open( yaml_file_name, 'r' ) as json_file:
            client_json = yaml.load( json_file, Loader=yaml.FullLoader )
            shm_mem = client_json["shared_memory"]
            dd = client_json["data_dictionary"]

            note_str = ' //This file is generated, Please dont modify the content, Change config.yaml file and generate code\n'
            print( note_str )

            header_str = '#include "data_dict.hpp" \n#include "shm_block.hpp" \n#include <sys/types.h> \n#include <sys/wait.h> \n#include <utility> \n'
            print(header_str)
            # shared-memory section
            for entry in shm_mem:
                if entry == "variable_space_path":
                    var_space_path = shm_mem["variable_space_path"]
                if entry == "variable_size":
                    var_space_size = shm_mem["variable_size"]
                if entry == "proc_space_path":
                    proc_space_path = shm_mem["proc_space_path"]
                if entry == "proc_size":
                    proc_space_size = shm_mem["proc_size"]
            print('shm_block process_var_space("%s",65,%s);' %(var_space_path, var_space_size) )
            print( 'shm_block service_var_space("%s",64,%s);' % (proc_space_path, proc_space_size) )
            print( "\n\n" )

            # variables
            offset = 0
            for idx,entry in enumerate(dd):
                var_name = entry["name"]
                var_value = str(entry["value"])
                var_signal = entry["signal"]
                if entry["data_type"] == "SIGNED8":
                    print( 'volatile variable_t<int8_t> var_%s(%s,%s,%s,dtype_t::SIGNED8);' % (var_name, idx, var_signal, offset ) )
                    offset +=1
                if entry["data_type"] == "SIGNED16":
                    print( 'volatile variable_t<int16_t> var_%s(%s,%s,%s,dtype_t::SIGNED16);' % (var_name, idx, var_signal, offset ) )
                    offset += 2
                if entry["data_type"] == "SIGNED32":
                    print( 'volatile variable_t<int32_t> var_%s(%s,%s,%s,dtype_t::SIGNED32);' % (var_name, idx, var_signal, offset ) )
                    offset += 4
                if entry["data_type"] == "SIGNED64":
                    print( 'volatile variable_t<int64_t> var_%s(%s,%s,%s,dtype_t::SIGNED64);' % (var_name, idx, var_signal, offset ) )
                    offset += 8
                if entry["data_type"] == "UNSIGNED8":
                    print( 'volatile variable_t<uint8_t> var_%s(%s,%s,%s,dtype_t::UNSIGNED8);' % (var_name, idx, var_signal, offset ) )
                    offset += 1
                if entry["data_type"] == "UNSIGNED16":
                    print( 'volatile variable_t<uint16_t> var_%s(%s,%s,%s,dtype_t::UNSIGNED16);' % (var_name, idx, var_signal, offset ) )
                    offset += 2
                if entry["data_type"] == "UNSIGNED32":
                    print( 'volatile variable_t<uint32_t> var_%s(%s,%s,%s,dtype_t::UNSIGNED32);' % (var_name, idx, var_signal, offset ) )
                    offset += 4
                if entry["data_type"] == "UNSIGNED64":
                    print( 'volatile variable_t<uint64_t> var_%s(%s,%s,%s,dtype_t::UNSIGNED64);' % (var_name, idx, var_signal, offset ) )
                    offset += 8
                if entry["data_type"] == "FLOAT32":
                    print('volatile variable_t<float> var_%s(%s,%s,%s,dtype_t::FLOAT32);' % (var_name, idx, var_signal, offset ) )
                    offset += 4
                if entry["data_type"] == "FLOAT64":
                    print( 'volatile variable_t<double> var_%s(%s,%s,%s,dtype_t::FLOAT64);' % (var_name, idx, var_signal, offset ) )
                    offset += 8

            print("\n\n")

            # handler
            handler_str = "void rt_sub_handler(int signo, siginfo_t *info, void *extra){\n"
            handler_str += "switch(info->si_value.sival_int){\n"
            for idx, entry in enumerate(dd):
                var_name = entry["name"]
                handler_str +="\tcase "+str(idx)+":\n\t\t"
                handler_str += "var_"+var_name+".update();\n\t break;\n"

            handler_str +="\n\t}\n}"

            print(handler_str)
    else:
        print("File not found")


generate_data_dict_cpp( "./config.yaml" )
