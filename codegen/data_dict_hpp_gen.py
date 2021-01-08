import os
import yaml
import cogapp as cog


def generate_data_dict_cpp(yaml_file_name):
    if os.path.exists( yaml_file_name ):
        with open( yaml_file_name, 'r' ) as json_file:
            client_json = yaml.load( json_file, Loader=yaml.FullLoader )
            shm_mem = client_json["shared_memory"]
            dd = client_json["data_dictionary"]

            note_str =  ' //This file is generated, Please dont modify the content, Change config.yaml file and generate code\n'
            print(note_str)

            header_str = '#ifndef DATA_DICT_HPP_ \n#define DATA_DICT_HPP_ \n#include "shm_variables.hpp" \n '
            print(header_str)
            print("\n")

            handler_str = "extern void rt_sub_handler(int signo, siginfo_t *info, void *extra);"
            print(handler_str)
            print("\n")

            for idx,entry in enumerate(dd):
                var_name = entry["name"]
                if entry["data_type"] == "SIGNED8":
                    print( 'extern  variable_t<int8_t> var_%s;' % var_name )
                if entry["data_type"] == "SIGNED16":
                    print( 'extern  variable_t<int16_t> var_%s;' % var_name )
                if entry["data_type"] == "SIGNED32":
                    print( 'extern  variable_t<int32_t> var_%s;' % var_name )
                if entry["data_type"] == "SIGNED64":
                    print( 'extern  variable_t<int64_t> var_%s;' % var_name)
                if entry["data_type"] == "UNSIGNED8":
                    print( 'extern  variable_t<uint8_t> var_%s;' % var_name)
                if entry["data_type"] == "UNSIGNED16":
                    print( 'extern  variable_t<uint16_t> var_%s;' % var_name)
                if entry["data_type"] == "UNSIGNED32":
                    print( 'extern  variable_t<uint32_t> var_%s;' % var_name)
                if entry["data_type"] == "UNSIGNED64":
                    print( 'extern  variable_t<uint64_t> var_%s;' % var_name)
                if entry["data_type"] == "FLOAT32":
                    print( 'extern  variable_t<float> var_%s;' % var_name)
                if entry["data_type"] == "FLOAT64":
                    print( 'extern  variable_t<double> var_%s;' % var_name)

            print( "\n")
            print( "#endif" )


generate_data_dict_cpp("./config.yaml")