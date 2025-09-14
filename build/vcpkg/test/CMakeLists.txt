cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(test)

# !!! find_package must go after project() declaration !!!
# Otherwise VCPKG does not set the CMAKE_PREFIX_PATH to find packages.
find_package(myci CONFIG REQUIRED)
find_package(utki CONFIG REQUIRED)

add_executable(test main.cpp)

target_link_libraries(
    test
    PRIVATE
        utki::utki
)
