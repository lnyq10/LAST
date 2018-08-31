set(ext_include ${PROJECT_SOURCE_DIR}/abc/include)
set(ext_include ${PROJECT_SOURCE_DIR}/abc/include PARENT_SCOPE)

set(ext_lib ${PROJECT_SOURCE_DIR}/abc/lib)
set(ext_lib ${PROJECT_SOURCE_DIR}/abc/lib PARENT_SCOPE)

set(abc_libraries ${ext_lib}/libabc.a dl pthread readline PARENT_SCOPE)
