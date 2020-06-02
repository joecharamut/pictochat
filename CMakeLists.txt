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
        src/Input.cpp src/Input.h
        src/types/Vector2.cpp src/types/Vector2.h
        src/types/Texture.cpp src/types/Texture.h
        src/types/Font.cpp src/types/Font.h
        src/types/Color.cpp src/types/Color.h
        src/net/Request.cpp src/net/Request.h
        src/types/Sound.cpp src/types/Sound.h
        src/gui/GuiObject.cpp src/gui/GuiObject.h
        src/gui/GuiButton.cpp src/gui/GuiButton.h
        src/gui/GuiImage.cpp src/gui/GuiImage.h
        src/chat/MainMenuGui.cpp src/chat/MainMenuGui.h
        src/gui/GuiText.cpp src/gui/GuiText.h
        src/types/Timer.cpp src/types/Timer.h
        src/chat/ChatState.cpp src/chat/ChatState.h
        src/chat/TopScreenGui.cpp src/chat/TopScreenGui.h
        src/chat/GuiMacros.h
        src/net/Socket.cpp src/net/Socket.h
        src/chat/Message.cpp src/chat/Message.h
        src/chat/KeyboardGui.cpp src/chat/KeyboardGui.h
        src/util/Base64.cpp src/util/Base64.h src/chat/ImageUtil.cpp src/chat/ImageUtil.h src/MobileTextInput.cpp src/MobileTextInput.h)

add_executable(project ${SRCS})
add_dependencies(project copy-files)

add_custom_target(copy-files ALL
        COMMAND rm -rf ${CMAKE_BINARY_DIR}/res
        COMMAND cp -R ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)

add_custom_target(copy-web-files ALL
        COMMAND rm -rf ${CMAKE_BINARY_DIR}/web
        COMMAND cp -R ${CMAKE_SOURCE_DIR}/web ${CMAKE_BINARY_DIR})

add_custom_target(project_wasm
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/build.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_dependencies(project_wasm copy-web-files)

add_custom_target(project_wasm_release
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/build-release.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_dependencies(project_wasm_release copy-web-files)

add_custom_target(copy-web-build-files ALL
        COMMAND cp ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.wasm ${CMAKE_BINARY_DIR}/web | true
        COMMAND cp ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.data ${CMAKE_BINARY_DIR}/web | true
        COMMAND cp ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.js ${CMAKE_BINARY_DIR}/web | true)

add_library(CxxUrl STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/CxxUrl/url.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/CxxUrl/string.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/CxxUrl/url.cpp)

include(ExternalProject)
ExternalProject_Add(sdl2-gfx
        URL http://www.ferzkopp.net/Software/SDL2_gfx/SDL2_gfx-1.0.4.zip
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/sdl2_gfx
        BUILD_IN_SOURCE 1
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external/sdl2_gfx
        PATCH_COMMAND patch -p 0 --forward --binary < ${CMAKE_SOURCE_DIR}/sdl_gfx.patch || true
        CONFIGURE_COMMAND bash -c "source ~/emsdk/emsdk_env.sh && chmod +x configure && emconfigure ./configure --enable-mmx=no"
        BUILD_COMMAND bash -c "source ~/emsdk/emsdk_env.sh && emmake make"
        INSTALL_COMMAND "")
add_library(SDL2_gfx UNKNOWN IMPORTED)
add_dependencies(SDL2_gfx sdl2-gfx)
set_property(TARGET SDL2_gfx PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/sdl2_gfx/.libs/libSDL2_gfx.a)
include_directories(${CMAKE_SOURCE_DIR}/external/sdl2_gfx)

set(COMMON_LIBRARIES CxxUrl SDL2_gfx)
include_directories(~/emsdk/upstream/emscripten/system/include)
#include_directories(lib/cpp-httplib)
#include_directories(lib/CxxUrl)
include_directories(lib/json/single_include)
include_directories(lib)

if(EMSCRIPTEN)
    message(STATUS "Compiling for emscripten")
    set_target_properties(project PROPERTIES SUFFIX ".html")

    set(EMSCRIPTEN_USE "-s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[\"png\"]' \
        -s USE_SDL_MIXER=2 -s FETCH=1")
    set(EMSCRIPTEN_FLAGS "--no-heap-copy --preload-file res -s EXIT_RUNTIME=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 \
        -s ENVIRONMENT='web' -s TOTAL_MEMORY=128MB -l websocket.js")

    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(EMSCRIPTEN_DEBUG "-s ASSERTIONS=2 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=1 \
            -s DISABLE_EXCEPTION_CATCHING=0 -s RUNTIME_LOGGING=1 -s DEMANGLE_SUPPORT=1 --emrun -O0")
    else()
        set(EMSCRIPTEN_DEBUG "-O2")
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
