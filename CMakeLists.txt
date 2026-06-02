cmake_minimum_required(VERSION 3.15)
project(IVIMessageBus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find all source files automatically
file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(include)

add_executable(ivi_bus ${SOURCES})

# Link threading library (required for std::thread)
find_package(Threads REQUIRED)
target_link_libraries(ivi_bus Threads::Threads)