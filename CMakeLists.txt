cmake_minimum_required(VERSION 3.27)
project(LibraryManagement)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(LibraryManagement
        main.cpp
        include/admin.h
        src/admin.cpp
        include/adminManager.h
        include/menu.h
        src/menu.cpp
        src/adminManager.cpp
        include/rbTree.h
        include/book.h
        src/book.cpp
        include/bookManager.h
        src/bookManager.cpp
)
