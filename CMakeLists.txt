cmake_minimum_required(VERSION 3.3)
project(HackGame)

set(CMAKE_CXX_STANDARD 20)


if (BUILD_TYPE MATCHES "Debug")
	message("Debug build")
	add_compile_options(-g)
else ()
	message("Release build")
	add_compile_options(-DNDEBUG)
endif ()

add_compile_options(
	-O3
	-Wall
	-Wextra
	-Wpedantic
	
    -Wcast-qual
    -Wold-style-cast
	
    -Wctor-dtor-privacy
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wduplicated-branches
    -Wduplicated-cond
    -Wshadow=compatible-local
    -Wzero-as-null-pointer-constant
    -Wextra-semi
    -Wsign-promo
    -Wlogical-op

    -Wno-float-equal
    -Wno-conversion
    -Wno-sign-conversion
	
	-Werror=return-type
)

add_executable(main
	src/start.cpp
	src/main.cpp
	src/init.cpp
	src/shaders.cpp
	src/util.cpp
	src/debug.cpp

	src/model/model.cpp
	src/model/models.cpp
	src/model/vao_model.cpp
	src/model/frame_model.cpp
	src/model/colored_model.cpp
	src/model/textured_model.cpp
	src/model/composite_model.cpp
	src/model/postprocessing_model.cpp

	src/context/shader.cpp
	src/context/tick_context.cpp

	src/entity/camera.cpp
	src/entity/simple_entity.cpp
	src/entity/player.cpp
	src/entity/enemy.cpp
	src/entity/block.cpp
	src/entity/bullet.cpp
	src/entity/minion.cpp
	src/entity/damageable.cpp

	src/entity/animation/animation.cpp
	src/entity/animation/billboard_animation.cpp
	src/entity/animation/enemy_damage.cpp
	src/entity/animation/enemy_destroy.cpp
	src/entity/animation/minion_destroy.cpp
	src/entity/animation/player_damage.cpp
)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(main glfw GLEW GL SOIL)
