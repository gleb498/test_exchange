cmake_minimum_required(VERSION 3.0.2)

include(cmake/HunterGate.cmake)
cmake_host_system_information(RESULT HUNTER_JOBS_NUMBER QUERY NUMBER_OF_LOGICAL_CORES)
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.24.2.tar.gz"
        SHA1 "17fd3b9f0b8c271eac9b71e30a6aa1b9fecab299"
        LOCAL
)

project(text_exchange)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_compile_options(-Wall -Werror -Wfatal-errors -Wno-deprecated -fpic)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs")

find_package(Threads REQUIRED)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(BOOST_COMPONENTS)
list(APPEND BOOST_COMPONENTS system program_options thread context)
hunter_add_package(Boost COMPONENTS ${BOOST_COMPONENTS})
hunter_add_package(nlohmann_json)

find_package(Boost REQUIRED CONFIG COMPONENTS ${BOOST_COMPONENTS})
find_package(nlohmann_json CONFIG REQUIRED)

if(Boost_FOUND)
  message(STATUS "Boost was found")
endif()

set(COMMON_MODULES Boost::boost nlohmann_json::nlohmann_json)
set(PROJECT_MODULES core network config)

include_directories("libs")
include_directories("include")
include_directories("app")
include_directories("3rd/spdlog/include")
include_directories("3rd/cli/include")

include(project)

# modules
add_subdirectory(app)
add_subdirectory(libs)
add_subdirectory(tests)