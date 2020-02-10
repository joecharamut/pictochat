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
        src/state/StateManager.cpp src/state/StateManager.h
        src/state/State.cpp src/state/State.h
        src/state/TestState.cpp src/state/TestState.h
        src/Input.cpp src/Input.h
        src/types/Vector2.cpp src/types/Vector2.h
        src/types/Texture.cpp src/types/Texture.h
        src/types/Font.cpp src/types/Font.h
        src/types/Color.cpp src/types/Color.h
        src/Network.cpp src/Network.h)
add_executable(project ${SRCS})
add_dependencies(project copy-files)

add_custom_target(copy-files ALL
#        COMMAND cmake -E remove_directory ${CMAKE_BINARY_DIR}/res
#        COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
        COMMAND rm -rf ${CMAKE_BINARY_DIR}/res
        COMMAND cp -R ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
        )

if(CMAKE_CXX_COMPILER MATCHES "em\\+\\+")
    message(STATUS "Compiling for emscripten")
    set_target_properties(project PROPERTIES SUFFIX ".js")
    add_definitions(-DEMSCRIPTEN)
    set(EMSCRIPTEN_USE "-s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s USE_SDL_NET=2")
    set(EMSCRIPTEN_FLAGS "--no-heap-copy --preload-file res -s TOTAL_MEMORY=128MB -s EXIT_RUNTIME=1 -s WASM=1")

    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(EMSCRIPTEN_DEBUG "-s ASSERTIONS=2 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=1 --source-map-base http://localhost:8080/ --emrun")
    else()
        set(EMSCRIPTEN_DEBUG "")
    endif()

    set(EMSCRIPTEN_OPTS "${EMSCRIPTEN_USE} ${EMSCRIPTEN_FLAGS} ${EMSCRIPTEN_DEBUG}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EMSCRIPTEN_OPTS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EMSCRIPTEN_OPTS}")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${EMSCRIPTEN_OPTS}")
else()
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_net REQUIRED)

    target_link_libraries(project SDL2::Main SDL2::TTF SDL2::Image SDL2::Mixer SDL2::Net)
endif()

add_custom_target(project_wasm
        COMMAND bash ${CMAKE_SOURCE_DIR}/build.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})