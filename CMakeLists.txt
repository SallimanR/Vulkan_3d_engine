cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(vulkan-tutorial VERSION 0.1.0)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

# set output dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# find source files:
file(GLOB_RECURSE project_src CONFIGURE DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${project_src})

# find include diretories and libraries
if(WIN32)
    find_package(GLFW3 3.3 REQUIRED)
else()
    find_package(glfw3 3.3 REQUIRED)
endif()

find_package(Vulkan REQUIRED)

include_directories("src/", ${Vulkan_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC
	${PROJECT_SOURCE_DIR}/src
	${Vulkan_INCLUDE_DIRS}
	# ${TINYOBJ_PATH}
	${GLFW_INCLUDE_DIRS}
	${GLM_PATH}
)

target_link_libraries(${PROJECT_NAME}
	glfw
	${Vulkan_LIBRARIES}
)

