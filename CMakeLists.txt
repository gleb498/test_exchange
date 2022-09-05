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

add_compile_options(-Wall -Werror -Wfatal-errors -Wno-deprecated)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

find_package(Threads REQUIRED)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(BOOST_COMPONENTS)
list(APPEND BOOST_COMPONENTS system program_options)
hunter_add_package(Boost COMPONENTS ${BOOST_COMPONENTS})
find_package(Boost COMPONENTS ${BOOST_COMPONENTS})

if(Boost_FOUND)
  message(STATUS "Boost was found")
endif()

set(COMMON_MODULES Boost::boost)
set(PROJECT_MODULES network core)

include_directories("3rd/spdlog/include")

include(project)

# modules
# add_subdirectory(3rd)
add_subdirectory(app)
add_subdirectory(libs)
add_subdirectory(tests)