AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod_see_login.cpp")
AC_ADD_SCRIPT_LOADER("SeeLogin" "${CMAKE_CURRENT_LIST_DIR}/src/loader_see_login.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/mod_see_login.conf.dist")