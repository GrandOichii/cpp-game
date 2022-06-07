cmake_minimum_required(VERSION 3.23.2)

set (CMAKE_CXX_STANDARD 17)

project(game)

file(GLOB srcFiles 
    src/*.cpp 
    src/window/*.cpp
    src/game/*.cpp
    src/game/map/*.cpp
    src/game/scripting/*.cpp 
)

add_executable(${PROJECT_NAME}
    ${srcFiles}
)

add_subdirectory(external/SDL)

target_include_directories(${PROJECT_NAME} 
    PUBLIC external/SDL/include
)
    
target_link_directories(${PROJECT_NAME} 
    PRIVATE external/SDL/src
)

target_link_libraries(${PROJECT_NAME}
    SDL2
)