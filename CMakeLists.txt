#==============================
# 编译要求核验
#==============================

cmake_minimum_required(VERSION 3.10)

#==============================
# 项目设定
#==============================

project("Prometheus Mk3" LANGUAGES CXX CUDA)

#==============================
# 内部编译单元
#==============================

add_subdirectory("GalaxyEngine")
add_subdirectory("PrometheusSystem")
add_subdirectory("CalibrationTool")

#==============================
# 外部编译单元
#==============================

add_subdirectory("ThirdParty/CameraDriver")
add_subdirectory("ThirdParty/SerialPortDriver")