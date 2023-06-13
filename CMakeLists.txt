
cmake_minimum_required (VERSION 3.0)

project (MyOpenGL C CXX)
SET(OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH}/bin)

aux_source_directory (src DIR_SRCS)

include_directories (Dependencies/glew-2.2.0/include)
include_directories (Dependencies/glfw-3.3.8/include)

add_subdirectory (Dependencies/glew-2.2.0/build/cmake)
add_subdirectory (Dependencies/glfw-3.3.8)

# 生成目标(可执行文件)
add_executable (myOpenGL ${DIR_SRCS})

# 链接静态库
target_link_libraries(myOpenGL glew_s)
target_link_libraries(myOpenGL glfw)