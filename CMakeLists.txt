cmake_minimum_required(VERSION 3.16)

project(dsu_strike)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(dsu_strike
    main.cpp
    game.cpp
    Vec2.cpp
    entity.cpp
)

target_include_directories(dsu_strike PRIVATE .)

target_link_libraries(dsu_strike PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

# Copy config.txt to build folder after every build
add_custom_command(TARGET dsu_strike POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/config.txt
        $<TARGET_FILE_DIR:dsu_strike>/config.txt
)