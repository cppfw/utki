vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO cppfw/utki
    REF "${VERSION}"
    SHA512 b315be256b0fbd7a9be2e8db894381d1270ecc8701a53ca04d5741d1a4d5e01e3a7f30f441601abc023d58e2fd0b1dfb9f88b865267188de85f88c01dc91fa6f
    HEAD_REF latest
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}/vcpkg"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "utki")

# Delete the include directory from the debug installation to prevent overlap.
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

# Install the LICENSE file to the package's share directory and rename it to copyright.
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

# Copy the usage instruction file to the package's share directory.
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
