cmake_minimum_required(VERSION 3.3)
project(HackGame)

set(CMAKE_CXX_STANDARD 20)

# add_definitions("-O3")
add_definitions("-g")
add_compile_options(
	-Wall
	-Wextra
	-Wpedantic
	
	-Wcast-align
	-Wcast-qual
	-Wenum-compare
	
	-Wctor-dtor-privacy
	-Wnon-virtual-dtor
	-Woverloaded-virtual
	-Wredundant-decls
	-Wsign-promo
	
	-Wduplicated-branches
	-Wduplicated-cond
	-Wshadow=compatible-local
	-Wzero-as-null-pointer-constant
#	-Wconversion
#	-Wsign-conversion
)

add_executable(main
	src/main.cpp
	src/model.cpp
	src/models.cpp
	src/tick_context.cpp
	src/block.cpp
	src/camera.cpp
	src/player.cpp
	src/bullet.cpp
	src/shaders.cpp
)

target_link_libraries(main glfw GLEW GL)
