cmake_minimum_required(VERSION 3.23.2)

set (CMAKE_CXX_STANDARD 17)

project(game)

file(GLOB srcFiles 
    src/*.cpp 
    # src/window/message_box.cpp
    src/window/*.cpp
    src/window/main_panel.cpp
    src/window/util.cpp
    src/window/window.cpp
    src/window/wrapper.cpp
    src/game/*.cpp
    src/game/map/*.cpp
    src/game/scripting/*.cpp 
)

add_executable(${PROJECT_NAME}
    ${srcFiles}
)

add_subdirectory(external/SDL)
add_subdirectory(external/SDL_ttf)

target_include_directories(${PROJECT_NAME} 
    PUBLIC external/SDL/include
    PUBLIC external/SDL_ttf
)
    
target_link_directories(${PROJECT_NAME} 
    PRIVATE external/SDL/src
    PRIVATE external/SDL_ttf
)

target_link_libraries(${PROJECT_NAME}
    SDL2
    SDL2_ttf
)