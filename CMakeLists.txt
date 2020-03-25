cmake_minimum_required(VERSION 3.14)
project(project)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_VERBOSE_MAKEFILE on)

set(SRCS
        src/Main.cpp src/Main.h
        src/Graphics.cpp src/Graphics.h
        src/types/Text.cpp src/types/Text.h
        src/ResourceManager.cpp src/ResourceManager.h
        src/util/Util.cpp src/util/Util.h
        src/state/StateManager.cpp src/state/StateManager.h
        src/state/State.cpp src/state/State.h
        src/state/TestState.cpp src/state/TestState.h
        src/Input.cpp src/Input.h
        src/types/Vector2.cpp src/types/Vector2.h
        src/types/Texture.cpp src/types/Texture.h
        src/types/Font.cpp src/types/Font.h
        src/types/Color.cpp src/types/Color.h
        src/net/Request.cpp src/net/Request.h
        src/state/BootState.cpp src/state/BootState.h
        src/dos/Shell.cpp src/dos/Shell.h
        src/dos/Console.cpp src/dos/Console.h
        src/dos/command/Command.cpp src/dos/command/Command.h
        src/dos/command/CommandDir.cpp src/dos/command/CommandDir.h
        src/dos/VirtualFS.cpp src/dos/VirtualFS.h
        src/dos/command/CommandCd.cpp src/dos/command/CommandCd.h
        src/dos/command/CommandCurl.cpp src/dos/command/CommandCurl.h
        src/dos/command/CommandComment.cpp src/dos/command/CommandComment.h
        src/types/Sound.cpp src/types/Sound.h
        src/fs/Filesystem.cpp src/fs/Filesystem.h)

include(${CMAKE_SOURCE_DIR}/cmake/cmake-modules/GetGitRevisionDescription.cmake)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

configure_file("${CMAKE_SOURCE_DIR}/src/util/Version.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/src/util/Version.cpp" @ONLY)
configure_file("${CMAKE_SOURCE_DIR}/src/util/Version.h" "${CMAKE_CURRENT_BINARY_DIR}/src/util/Version.h" COPYONLY)

include(${CMAKE_SOURCE_DIR}/cmake/FileInformation/Module/FileInformation.cmake)
set(SOURCE_SIZES "")
foreach(source ${SRCS})
    if (${source} MATCHES "cpp$")
        file(SIZE ${source} FILE_SIZE)
        list(APPEND SOURCE_SIZES ${source} ${FILE_SIZE})
    endif()
endforeach()

configure_file("${CMAKE_SOURCE_DIR}/src/util/Sources.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/src/util/Sources.cpp" @ONLY)
configure_file("${CMAKE_SOURCE_DIR}/src/util/Sources.h" "${CMAKE_CURRENT_BINARY_DIR}/src/util/Sources.h" COPYONLY)

list(APPEND SRCS
        "${CMAKE_CURRENT_BINARY_DIR}/src/util/Version.cpp" "src/util/Version.h"
        "${CMAKE_CURRENT_BINARY_DIR}/src/util/Sources.cpp" "src/util/Sources.h")

add_executable(project ${SRCS})
add_dependencies(project copy-files)

add_custom_target(copy-files ALL
        COMMAND rm -rf ${CMAKE_BINARY_DIR}/res
        COMMAND cp -R ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)

add_custom_target(copy-web-files ALL
        COMMAND rm -rf ${CMAKE_BINARY_DIR}/web
        COMMAND cp -R ${CMAKE_SOURCE_DIR}/web ${CMAKE_BINARY_DIR})

add_custom_target(project_wasm
        COMMAND bash ${CMAKE_SOURCE_DIR}/build.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_dependencies(project_wasm copy-web-files)

add_custom_target(copy-web-build-files ALL
        COMMAND cp ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.wasm ${CMAKE_BINARY_DIR}/web
        COMMAND cp ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.data ${CMAKE_BINARY_DIR}/web
        COMMAND cp ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.js ${CMAKE_BINARY_DIR}/web)

add_library(CxxUrl STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/CxxUrl/url.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/CxxUrl/string.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/CxxUrl/url.cpp)

set(COMMON_LIBRARIES CxxUrl)
include_directories(/home/joseph/emsdk/upstream/emscripten/system/include)
#include_directories(lib/cpp-httplib)
#include_directories(lib/CxxUrl)
include_directories(lib/json/single_include)
include_directories(lib)


if(CMAKE_CXX_COMPILER MATCHES "em\\+\\+")
    message(STATUS "Compiling for emscripten")
    set_target_properties(project PROPERTIES SUFFIX ".html")

    set(EMSCRIPTEN_USE "-s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s FETCH=1")
    set(EMSCRIPTEN_FLAGS "--no-heap-copy --preload-file res -s EXIT_RUNTIME=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 \
        -s ENVIRONMENT='web' -s TOTAL_MEMORY=128MB -l idbfs.js")

    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(EMSCRIPTEN_DEBUG "-s ASSERTIONS=2 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=1 \
            --source-map-base http://localhost:8080/ -s RUNTIME_LOGGING=1 -s DEMANGLE_SUPPORT=1 --emrun")
    else()
        set(EMSCRIPTEN_DEBUG "")
    endif()

    set(EMSCRIPTEN_OPTS "${EMSCRIPTEN_USE} ${EMSCRIPTEN_FLAGS} ${EMSCRIPTEN_DEBUG}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EMSCRIPTEN_OPTS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EMSCRIPTEN_OPTS}")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${EMSCRIPTEN_OPTS}")

    target_link_libraries(project ${COMMON_LIBRARIES})
else()
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/sdl2-cmake-modules)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    target_link_libraries(project ${COMMON_LIBRARIES} SDL2::Main SDL2::TTF SDL2::Image SDL2::Mixer)
endif()
