module_type = 'lib-static'
module_name = 'minicmn_static'

include_dir_list = ['../minicmn']
src_search_dir_list = ['src']

build_list = [
    'data_model.c',
    'os_path.c',
    'sds.c',
]

build_list_windows = [
    'data_model_winapi.c',
    'os_path_winapi.c',
    'shlib_winapi.c',
]

build_list_posix = [
    'data_model_posix.c',
    'os_path_posix.c',
    'shlib_posix.c',
]
