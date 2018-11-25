# c_struct_types Importer
#
#   This library will read a C++ header file and create python ctypes classes for all
#   the structs defined in the header.
#   Only supports types defined in the supported_types list below.
#
#   Include defined c struct types with:
#           from c_struct_types import *
#
#   Created by Charlie Tytler  2018
#

header_file = '../libraries/A0_Quadcopter_Libs/global_data.h'

supported_types = ['uint8_t', 'uint16_t', 'uint32_t', 'float', 'char']


import ctypes


__all__ = []  # To be added to as custom types are defined


def remove_multiple_spaces(string_before):
    string_after = string_before.replace('  ', ' ')
    while len(string_after) < len(string_before):
        string_before = string_after
        string_after = string_before.replace('  ', ' ')
    return string_after


# Read in contents of header file.
with open(header_file, 'r') as fid:
    h_contents = fid.read()
h_contents = h_contents.replace('\n', '')
h_contents = remove_multiple_spaces(h_contents)


def get_field_list(fields):
    field_list = []
    for field in fields:
        split_definition = field.strip().split(' ')
        field_type = split_definition[0].strip()
        field_name = split_definition[1].strip()
        if field_type in supported_types:
            if field_type == 'uint8_t':
                field_list.append((field_name, ctypes.c_int8))
            elif field_type == 'uint16_t':
                field_list.append((field_name, ctypes.c_int16))
            elif field_type == 'uint32_t':
                field_list.append((field_name, ctypes.c_int32))
            elif field_type == 'float':
                field_list.append((field_name, ctypes.c_float))
            elif field_type == 'char':
                field_list.append((field_name, ctypes.c_char))
            else:
                exec('field_list.append((field_name,' + field_type + '))')
        else:
            raise Exception("Unexpected type encountered in c parsing")
    return field_list



# Exctract all type definitions of form typedef ... ;
type_defs = []
typedef_idx = h_contents.find('typedef', 0)
while typedef_idx >= 0:
    # Get typename
    space_delimiter_idx = h_contents.find(' ', typedef_idx + 8)
    typedef_type = h_contents[typedef_idx + 8: space_delimiter_idx]
    if 'struct' in typedef_type:
        start_brace_idx = h_contents.find('{', typedef_idx)
        end_brace_idx = h_contents.find('}', typedef_idx)
        semicolon_idx = h_contents.find(';', end_brace_idx)
        struct_type_contents = h_contents[start_brace_idx + 1: end_brace_idx]
        struct_type_contents = struct_type_contents.split(';')

        fields = h_contents[start_brace_idx + 1 : end_brace_idx].split(';')
        field_list = get_field_list(fields[0:-1])

        struct_type_name = h_contents[end_brace_idx + 1: semicolon_idx].replace(' ', '')

        # Create ctype structure class with name struct_type_name
        new_type = type(struct_type_name, (ctypes.Structure,), {'_fields_':field_list})
        exec(struct_type_name + '= new_type')
        del new_type
        supported_types.append(struct_type_name)
        __all__.append(struct_type_name)

    else:
        semicolon_idx = h_contents.find(';', typedef_idx)
        type_name = h_contents[space_delimiter_idx: semicolon_idx]
        # Only struct definitions are supported at the moment.
        print("Definition skipped (not a struct): " + typedef_type.strip() + " " + type_name)

    typedef_idx = h_contents.find('typedef', semicolon_idx)
