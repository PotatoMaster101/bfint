# cmake -B bin
# cmake --build bin
cmake_minimum_required(VERSION 3.17)

project(bfint)

# use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# compiler warnings
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# specify source/include directories
set(SRC_DIR src)
set(INC_DIR include)
include_directories(${INC_DIR}/bfint)
add_executable(main ${SRC_DIR}/main/main.cpp ${SRC_DIR}/bfint/bfint.cpp)
