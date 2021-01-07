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

            header_str = '#ifndef DATA_DICT_HPP_ \n#define DATA_DICT_HPP_ \n#include "shm_variable.hpp" \n '
            print(header_str)

            variable_count = 0
            for entry in enumerate( dd ):
                variable_count += 1

            indices_str = "extern std::array<variable_info*," + str( variable_count ) + "> indices;"
            print(indices_str)

            for idx,entry in enumerate(dd):
                if entry["data_type"] == "SIGNED8":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'extern volatile variable_t<int8_t> var_%s;' % var_name )
                if entry["data_type"] == "SIGNED16":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'extern volatile variable_t<int16_t> var_%s;' % var_name )
                if entry["data_type"] == "SIGNED32":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'extern volatile variable_t<int32_t> var_%s;' % var_name )
                if entry["data_type"] == "SIGNED64":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'extern volatile variable_t<int64_t> var_%s;' % var_name)
                if entry["data_type"] == "UNSIGNED8":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'extern volatile variable_t<uint8_t> var_%s;' % var_name)
                if entry["data_type"] == "UNSIGNED16":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'extern volatile variable_t<uint16_t> var_%s;' % var_name)
                if entry["data_type"] == "UNSIGNED32":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'extern volatile variable_t<uint32_t> var_%s;' % var_name)
                if entry["data_type"] == "UNSIGNED64":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'extern volatile variable_t<uint64_t> var_%s;' % var_name)
                if entry["data_type"] == "FLOAT32":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'extern volatile variable_t<float> var_%s;' % var_name)
                if entry["data_type"] == "FLOAT64":
                    var_name = entry["name"]
                    var_value = entry["value"]
                    print( 'extern volatile variable_t<double> var_%s;' % var_name)

            print( "#endif" )
generate_data_dict_cpp("./config.yaml")