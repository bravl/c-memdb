#**********************************************************************************************
# Project definition
#**********************************************************************************************
project('MACS-Core', 'c', 'cpp',
	version : '2.0.0',
	meson_version : '>= 0.44.0',
	default_options : ['c_std=gnu99', 'cpp_std=c++11'])

#**********************************************************************************************
# Project Global arguments
#**********************************************************************************************
add_project_arguments('-DALSA_INTERNAL_DEBUG', language : 'cpp')

c_memdb_src = [
	files(
		'avl.c',
		'bitree.c',
		'log.c',
		'main.c',
	)
]

executable('memdb', c_memdb_src)
