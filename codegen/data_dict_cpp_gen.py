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

            header_str = '#include "data_dict.hpp" \n \
                          #include "shm_block.hpp" \n \
                          #include <sys/types.h> \n \
                          #include <sys/wait.h> \n \
                          #include <utility> \n'
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
            print('shm_block var_space("%s",65,%s);' %(var_space_path, var_space_size) )
            print( 'shm_block proc_space("%s",64,%s);' % (proc_space_path, proc_space_size) )

            variable_count = 0
            for entry in enumerate( dd ):
                variable_count +=1

            indices_str = "std::array<variable_info*," + str( variable_count ) + "> indices { "

            # variables
            offset = 0
            for idx,entry in enumerate(dd):
                if entry["data_type"] == "SIGNED8":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'volatile variable_t<int8_t> var_%s(%s);' % (var_name, var_value) )
                    print('variable_info info_%s(%s,%s,dtype_t::SIGNED8);' %(var_name, idx,offset))
                    indices_str +="&info_"+var_name+ ("," if idx != (variable_count-1)else "")
                    offset +=1
                if entry["data_type"] == "SIGNED16":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'volatile variable_t<int16_t> var_%s(%s);' % (var_name, var_value) )
                    print( 'variable_info info_%s(%s,%s,dtype_t::SIGNED16);' % (var_name, idx, offset) )
                    indices_str += "&info_" + var_name + ("," if idx != (variable_count-1)else "")
                    offset += 2
                if entry["data_type"] == "SIGNED32":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'volatile variable_t<int32_t> var_%s(%s);' % (var_name, var_value) )
                    print( 'variable_info info_%s(%s,%s,dtype_t::SIGNED32);' % (var_name, idx, offset) )
                    indices_str += "&info_" + var_name + ("," if idx != (variable_count-1)else "")
                    offset += 4
                if entry["data_type"] == "SIGNED64":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'volatile variable_t<int64_t> var_%s(%s);' % (var_name, var_value) )
                    print( 'variable_info info_%s(%s,%s,dtype_t::SIGNED64);' % (var_name, idx, offset) )
                    indices_str += "&info_" + var_name + ("," if idx != (variable_count-1)else "")
                    offset += 8
                if entry["data_type"] == "UNSIGNED8":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'volatile variable_t<uint8_t> var_%s(%s);' % (var_name, var_value) )
                    print( 'variable_info info_%s(%s,%s,dtype_t::UNSIGNED8);' % (var_name, idx, offset) )
                    indices_str += "&info_" + var_name + ("," if idx != (variable_count-1)else "")
                    offset += 1
                if entry["data_type"] == "UNSIGNED16":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'volatile variable_t<uint16_t> var_%s(%s);' % (var_name, var_value) )
                    print( 'variable_info info_%s(%s,%s,dtype_t::UNSIGNED16);' % (var_name, idx, offset) )
                    indices_str += "&info_" + var_name + ("," if idx != (variable_count-1)else "")
                    offset += 2
                if entry["data_type"] == "UNSIGNED32":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'volatile variable_t<uint32_t> var_%s(%s);' % (var_name, var_value) )
                    print( 'variable_info info_%s(%s,%s,dtype_t::UNSIGNED32);' % (var_name, idx, offset) )
                    indices_str += "&info_" + var_name + ("," if idx != (variable_count-1)else "")
                    offset += 4
                if entry["data_type"] == "UNSIGNED64":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'volatile variable_t<uint64_t> var_%s(%s);' % (var_name, var_value) )
                    print( 'variable_info info_%s(%s,%s,dtype_t::UNSIGNED64);' % (var_name, idx, offset) )
                    indices_str += "&info_" + var_name + ("," if idx != (variable_count-1)else "")
                    offset += 8
                if entry["data_type"] == "FLOAT32":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'volatile variable_t<float> var_%s(%s);' % (var_name, var_value) )
                    print( 'variable_info info_%s(%s,%s,dtype_t::FLOAT32);' % (var_name, idx, offset) )
                    indices_str += "&info_" + var_name + ("," if idx != (variable_count-1)else "")
                    offset += 4
                if entry["data_type"] == "FLOAT64":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'volatile variable_t<double> var_%s(%s);' % (var_name, var_value) )
                    print( 'variable_info info_%s(%s,%s,dtype_t::FLOAT64);' % (var_name, idx, offset) )
                    indices_str += "&info_" + var_name + ("," if idx != (variable_count-1)else "")
                    offset += 8

            indices_str += "};"
            print(indices_str)

            # handler
            handler_str = "void rt_sub_handler(int signo, siginfo_t *info, void *extra){\n"
            handler_str += "switch(info->si_value.sival_int){\n"
            for idx, entry in enumerate(dd):
                var_name = entry["name"]
                handler_str +="\tcase "+str(idx)+":\n\t\t"
                if entry["data_type"] == "SIGNED8":
                    handler_str += "var_space.read<int8_t>("+str(idx)+",var_"+var_name+");\n\t break;\n"
                if entry["data_type"] == "SIGNED16":
                    handler_str += "var_space.read<int16_t>("+str(idx)+",var_"+var_name+");\n\t break;\n"
                if entry["data_type"] == "SIGNED32":
                    handler_str += "var_space.read<int32_t>("+str(idx)+",var_"+var_name+");\n\t break;\n"
                if entry["data_type"] == "SIGNED64":
                    handler_str += "var_space.read<int64_t>("+str(idx)+",var_"+var_name+");\n\t break;\n"
                if entry["data_type"] == "UNSIGNED8":
                    handler_str += "var_space.read<uint8_t>("+str(idx)+",var_"+var_name+");\n\t break;\n"
                if entry["data_type"] == "UNSIGNED16":
                    handler_str += "var_space.read<uint16_t>("+str(idx)+",var_"+var_name+");\n\t break;\n"
                if entry["data_type"] == "UNSIGNED32":
                    handler_str += "var_space.read<uint32_t>("+str(idx)+",var_"+var_name+");\n\t break;\n"
                if entry["data_type"] == "UNSIGNED64":
                    handler_str += "var_space.read<uint64_t>("+str(idx)+",var_"+var_name+");\n\t break;\n"
                if entry["data_type"] == "FLOAT32":
                    handler_str += "var_space.read<float>("+str(idx)+",var_"+var_name+");\n\t break;\n"
                if entry["data_type"] == "FLOAT64":
                    handler_str += "var_space.read<double>("+str(idx)+",var_"+var_name+");\n\t break;\n"

            handler_str +="\n\t}\n}"

            print(handler_str)
    else:
        print("File not found")


generate_data_dict_cpp( "./config.yaml" )
