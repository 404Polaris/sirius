set(MySql_FOUND OFF)


set(INCLUDE_SEARCH_DIRS
        /usr/include
        /usr/include/mysql
        /usr/local/include
        /usr/local/include/mysql
        /usr/local/mysql/include)


set(LIBRARY_SEARCH_DIRS
        /usr/lib
        /usr/lib/mysql
        /usr/local/lib
        /usr/local/lib/mysql
        /usr/local/mysql/lib)


find_path(MySqlClient_INCLUDE_DIR NAMES mysql.h PATHS ${INCLUDE_SEARCH_DIRS})
find_path(MySqlClient_LIBRARY_DIR NAMES libmysqlclient.so PATHS ${LIBRARY_SEARCH_DIRS})

if (MySqlClient_INCLUDE_DIR)
    if (MySqlClient_LIBRARY_DIR)
        set(MySqlClient_FOUND ON)
        include_directories(${MySqlClient_INCLUDE_DIR})
        link_directories(${MySqlClient_LIBRARY_DIR})
    endif ()
endif ()








