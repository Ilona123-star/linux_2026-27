cmake_minimum_required(VERSION 3.16)

project(HW2_Git LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Demo/IO)

add_executable(
	execution_demo
	Demo/Process/Execution.cpp
	Utils/Process/Execution.cpp
)

target_include_directories(
	execution_demo
	PRIVATE
	${PROJECT_SOURCE_DIR}
)
