cmake_minimum_required(VERSION 3.11)
project(untitled)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ftxui QUIET)
if(NOT ftxui_FOUND)
    # Check if FTXUI directory exists locally
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/FTXUI")
        message(STATUS "Using local FTXUI directory")
        add_subdirectory(FTXUI EXCLUDE_FROM_ALL)
    else()
        # Fallback: try FetchContent (requires network)
        include(FetchContent)
        
        # Set offline mode if needed
        set(FETCHCONTENT_FULLY_DISCONNECTED ON CACHE BOOL "Disable FetchContent updates")
        
        FetchContent_Declare(ftxui
          GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
          GIT_TAG v6.1.9
        )
        
        FetchContent_MakeAvailable(ftxui)
    endif()
endif()

add_executable(task task.cpp
        TaskManager.h
        TaskManager.cpp
        Calculations.h
        ErrorHandling.h
        ErrorHandling.cpp)

# Link FTXUI libraries - use explicit target name
target_link_libraries(task 
  PRIVATE 
    ftxui::screen
    ftxui::dom 
    ftxui::component
)

# Set compiler flags
target_compile_options(task PRIVATE
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -g>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -g>
)

install(TARGETS task DESTINATION bin)
