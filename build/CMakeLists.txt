

cmake_minimum_required (VERSION 2.6)

set(GAME_NAME  Vidya)
project (Benis)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -pedantic -g -Wall -Wextra")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../out)
set(GAME_SRC_DIR ${PROJECT_BINARY_DIR}/../src/) 
set(TEST_SRC_DIR ${PROJECT_BINARY_DIR}/../tests/)
set(TEST_OUTPUT_DIR ${PROJECT_BINARY_DIR}/../testBin/)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2 )
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

#Prepare vidya binary
file(GLOB GAME_SRC_FILES
    ${GAME_SRC_DIR}*.h
    ${GAME_SRC_DIR}*.cpp
)

add_executable(${GAME_NAME} ${GAME_SRC_FILES})
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${GAME_NAME} dl pthread ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES})



#Prepare testing
#enable_testing()
#file(GLOB TEST_SRCS ${TEST_SRC_DIR}*.cpp)
#foreach(testSrc ${TEST_SRCS})
#        get_filename_component(testName ${testSrc} NAME_WE)
#
#        add_executable(${testName} ${testSrc})
#
#        target_link_libraries(${testName} LINK_PUBLIC Engine)
#        set_target_properties(${testName} PROPERTIES 
#            RUNTIME_OUTPUT_DIRECTORY  ${TEST_OUTPUT_DIR})
#
#        add_test(NAME ${testName} 
#                 WORKING_DIRECTORY ${TEST_OUTPUT_DIR} 
#                 COMMAND ${TEST_OUTPUT_DIR}${testName} )
#endforeach(testSrc)
    
