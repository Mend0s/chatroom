cmake_minimum_required(VERSION 3.10)
project(ServerApp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件的目录
include_directories(
    ${PROJECT_SOURCE_DIR}/controller
    ${PROJECT_SOURCE_DIR}/model
    ${PROJECT_SOURCE_DIR}/resources
    ${PROJECT_SOURCE_DIR}/tools
)

# 查找并配置 JsonCpp 库
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

# 如果找到了 JsonCpp 库，添加它的头文件目录
if(JSONCPP_FOUND)
  include_directories(${JSONCPP_INCLUDE_DIRS})
endif()

# 添加所有源文件
file(GLOB_RECURSE SOURCES
     "${PROJECT_SOURCE_DIR}/controller/*.cpp"
     "${PROJECT_SOURCE_DIR}/model/*.cpp"
     "${PROJECT_SOURCE_DIR}/resources/*.cpp"
     "${PROJECT_SOURCE_DIR}/tools/*.cpp"
)

# 创建可执行文件
add_executable(ServerApp ${SOURCES} main.cpp)

# 链接 JsonCpp 库
if(JSONCPP_FOUND)
  target_link_libraries(ServerApp ${JSONCPP_LIBRARIES} pthread)
  # 如果 JsonCpp 需要特定的编译定义或编译选项，也可以在这里添加
  add_definitions(${JSONCPP_DEFINITIONS})
endif()