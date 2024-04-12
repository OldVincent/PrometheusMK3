#==============================
# 编译要求核验
#==============================

cmake_minimum_required(VERSION 3.10)

#==============================
# 项目设定
#==============================

set(TARGET_NAME "GalaxyEngine")

#==============================
# 编译命令行设定
#==============================

set(CMAKE_CXX_STANDARD 17)

#==============================
# 源
#==============================

# 查找项目目录下所有源文件，记录入 TARGET_SOURCE 中
file(GLOB_RECURSE TARGET_SOURCE "*.cpp")
# 查找项目目录下所有头文件，记录入 TARGET_HEADER 中
file(GLOB_RECURSE TARGET_HEADER "*.hpp")
# 查找项目目录下所有CUDA源文件，记录入 TARGET_CUDA_SOURCE 中
file(GLOB_RECURSE TARGET_CUDA_SOURCE "*.cu")
# 查找项目目录下所有CUDA源文件，记录入 TARGET_CUDA_HEADER 中
file(GLOB_RECURSE TARGET_CUDA_HEADER "*.cuh")

#==============================
# 编译目标
#==============================

# 编译静态库
add_library(${TARGET_NAME} STATIC ${TARGET_SOURCE} ${TARGET_HEADER} ${TARGET_CUDA_SOURCE} ${TARGET_CUDA_HEADER})

#==============================
# 外部依赖
#==============================

# 外部模块目录
target_include_directories(${TARGET_NAME} PUBLIC "../ThirdParty/")
target_include_directories(${TARGET_NAME} PUBLIC "../")

# Boost
find_package(Boost 1.71 REQUIRED COMPONENTS system thread filesystem)
target_include_directories(${TARGET_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PUBLIC ${Boost_LIBRARIES})

# TBB
find_path(TBB_INCLUDE "tbb/tbb.h")
find_library(TBB_LIB "libtbb.so")
target_include_directories(${TARGET_NAME} PUBLIC ${TBB_INCLUDE})
target_link_libraries(${TARGET_NAME} PUBLIC ${TBB_LIB})

# 在Linux系统下，多线程模块并非自动链接的，需要额外链接。
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(Threads)
    target_link_libraries(${TARGET_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()