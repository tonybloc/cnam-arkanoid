cmake_minimum_required(VERSION 3.7)

project(Arkanoid LANGUAGES C)

include_directories(${PROJECT_NAME}/headers)
include_directories(${PROJECT_NAME}/sources)
include_directories(${PROJECT_NAME}/public)

IF (MSVC)
set (CMAKE_PREFIX_PATH "c:/sdl2")
ENDIF()

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME}
    "./arkanoid.c";
    "./prototypes.h"
    "./headers/ball.h"
    "./headers/ship.h"
    "./headers/include.h"
    "./headers/gui_brick.h"
    "./headers/round.h"
    "./sources/ball.c"
    "./sources/ship.c"
    "./sources/gui_brick.c"
    "./sources/round.c"
    )

target_sources(${PROJECT_NAME} PUBLIC arkanoid.c)
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/sprites.bmp
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/Arkanoid_sprites.bmp
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/Arkanoid_ascii.bmp
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_01.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_02.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_03.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_04.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_05.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_06.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_07.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_08.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_09.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_10.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_11.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_12.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_13.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_14.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_15.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_16.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_17.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_18.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_19.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_20.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_21.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_22.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_23.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_24.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_25.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_26.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_27.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_28.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_29.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_30.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_31.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_32.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_33.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_34.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_35.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/public/level_36.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/public)

IF (MSVC)
file(COPY ${SDL2_LIBDIR}/SDL2.dll
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/)
file(COPY ${SDL2_LIBDIR}/SDL2.dll
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
ENDIF()
