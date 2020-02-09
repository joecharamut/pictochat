cmake_minimum_required(VERSION 3.14)
project(project)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_VERBOSE_MAKEFILE on)

set(SRCS
        src/Main.cpp src/Main.h
        src/Graphics.cpp src/Graphics.h
        src/types/Text.cpp src/types/Text.h
        src/ResourceManager.cpp src/ResourceManager.h
        src/Util.cpp src/Util.h
        src/state/StateManager.cpp src/state/StateManager.h src/state/State.cpp src/state/State.h src/state/TestState.cpp src/state/TestState.h src/Input.cpp src/Input.h src/types/Vector2.cpp src/types/Vector2.h src/types/Texture.cpp src/types/Texture.h src/types/Font.cpp src/types/Font.h)
add_executable(project ${SRCS})
add_dependencies(project copy-files)

add_custom_target(copy-files ALL
        COMMAND cmake -E remove_directory ${CMAKE_BINARY_DIR}/res
        COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)

if(CMAKE_CXX_COMPILER MATCHES "em\\+\\+")
    message(STATUS "Compiling for emscripten")
    set_target_properties(project PROPERTIES SUFFIX ".html")
    add_definitions(-DEMSCRIPTEN)
    set(EMSCRIPTEN_FEATURES "-s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 --no-heap-copy --preload-file res")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EMSCRIPTEN_FEATURES}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EMSCRIPTEN_FEATURES}")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${EMSCRIPTEN_FEATURES}")
else()
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)

    target_link_libraries(project SDL2::Main SDL2::TTF SDL2::Image)
endif()

add_custom_target(project_wasm
        COMMAND bash ${CMAKE_SOURCE_DIR}/build.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})