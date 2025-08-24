cmake_minimum_required(VERSION 3.3)
project(HackGame)

set(CMAKE_CXX_STANDARD 20)

# add_definitions("-O3")
add_definitions("-g")
add_compile_options(
	-O3
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
	-Wsign-conversion
)

add_executable(main
	src/main.cpp

	src/model/model.cpp
	src/model/models.cpp
	src/model/simple_model.cpp
	src/model/composite_model.cpp

	src/context/tick_context.cpp

	src/entity/camera.cpp
	src/entity/simple_entity.cpp
	src/entity/animation_entity.cpp
	src/entity/block.cpp
	src/entity/player.cpp
	src/entity/bullet.cpp
	src/entity/enemy.cpp

	src/shaders.cpp
)

target_link_libraries(main glfw GLEW GL)
