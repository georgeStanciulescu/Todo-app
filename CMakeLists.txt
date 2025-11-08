cmake_minimum_required(VERSION 3.11)
project(untitled)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(ftxui QUIET)
if(NOT ftxui_FOUND)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/FTXUI")
        message(STATUS "Using local FTXUI directory")
        add_subdirectory(FTXUI EXCLUDE_FROM_ALL)
    else()
        include(FetchContent)
        
        set(FETCHCONTENT_FULLY_DISCONNECTED ON CACHE BOOL "Disable FetchContent updates")
        
        FetchContent_Declare(ftxui
          GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
          GIT_TAG v6.1.9
        )
        
        FetchContent_MakeAvailable(ftxui)
    endif()
endif()

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_executable(task ${SOURCES} ${HEADERS})

set(CMAKE_BUILD_TYPE Debug)

target_link_libraries(task 
  PRIVATE 
    ftxui::screen
    ftxui::dom 
    ftxui::component
)
target_include_directories(task PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_options(task PRIVATE
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -g>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -g>
)

install(TARGETS task DESTINATION bin)
