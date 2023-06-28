
cmake_minimum_required (VERSION 3.0)

project (MyOpenGL C CXX)
SET(CMAKE_CXX_STANDARD 17)

SET(OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH}/bin)


add_subdirectory (Dependencies/glew-2.2.0/build/cmake)
add_subdirectory (Dependencies/glfw-3.3.8)

# main target
aux_source_directory (src DIR_SRCS)
aux_source_directory (src/vendor/stb_image STB_IMAGE_SRCS)
aux_source_directory (src/vendor/imgui IMGUI_SRCS)

add_executable (myOpenGL ${DIR_SRCS} ${STB_IMAGE_SRCS} ${IMGUI_SRCS})

target_include_directories (myOpenGL PRIVATE src)
target_include_directories (myOpenGL PRIVATE src/vendor)
target_include_directories (myOpenGL PRIVATE Dependencies/glew-2.2.0/include)
target_include_directories (myOpenGL PRIVATE Dependencies/glfw-3.3.8/include)

# 链接静态库
target_link_libraries(myOpenGL glew_s)
target_link_libraries(myOpenGL glfw)