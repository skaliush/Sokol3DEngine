cmake_minimum_required(VERSION 3.28)
project(Sokol3DEngine C)

set(CMAKE_C_STANDARD 99)

include_directories(libs/SDL2/x86_64-w64-mingw32/include/SDL2)
link_directories(libs/SDL2/x86_64-w64-mingw32/lib)

add_executable(Sokol3DEngine src/main.c
        src/display.c
        src/display.h
        src/vector.c
        src/vector.h
        src/mesh.c
        src/mesh.h
        src/triangle.c
        src/triangle.h
        src/draw-numbers.c
        src/draw-numbers.h
        src/array.c
        src/array.h
        src/utils.c
        src/utils.h
        src/matrix.c
        src/matrix.h
        src/camera.c
        src/camera.h)

target_compile_definitions(Sokol3DEngine PRIVATE SDL_MAIN_HANDLED)

target_link_libraries(Sokol3DEngine SDL2 SDL2main)
