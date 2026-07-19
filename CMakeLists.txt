cmake_minimum_required(VERSION 4.3)
project(pinconf VERSION 1.0 DESCRIPTION "The configuration format library for pinpal" LANGUAGES C)
include(GNUInstallDirs)

# ---------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------
find_package(pinlog REQUIRED)
find_package(pinmem REQUIRED)


set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


set(CMAKE_BUILD_RPATH 
    "${CMAKE_CURRENT_BINARY_DIR}:$<TARGET_FILE_DIR:pinlog::pinlog>"
    "${CMAKE_CURRENT_BINARY_DIR}:$<TARGET_FILE_DIR:pinmem::pinmem>"
)

# Install-time: look in the standard library install directory
set(CMAKE_INSTALL_RPATH 
    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
)



# ---------------------------------------------------------------------
# Libraries
# ---------------------------------------------------------------------
add_library(pinconf SHARED src/pinconf.c)
add_library(pinconf_static STATIC src/pinconf.c)


set_target_properties(pinconf PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER include/pinconf/pinconf.h)

set_target_properties(pinconf_static PROPERTIES
    OUTPUT_NAME pinconf
    CLEAN_DIRECT_OUTPUT 1)


target_link_libraries(pinconf PRIVATE pinlog pinmem)
target_link_libraries(pinconf_static PRIVATE pinlog pinmem)

target_include_directories(pinconf PRIVATE include/)
target_include_directories(pinconf_static PRIVATE include/)

# ---------------------------------------------------------------------
# Examples (build only, not installed)
# ---------------------------------------------------------------------
add_executable(example example.c)
target_link_libraries(example pinconf pinmem pinlog)
target_include_directories(example PRIVATE include)

# ---------------------------------------------------------------------
# pkg-config files (optional, for legacy use)
# ---------------------------------------------------------------------
configure_file(pinconf.pc.in pinconf.pc @ONLY)

# ---------------------------------------------------------------------
# Installation
# ---------------------------------------------------------------------
install(TARGETS pinconf pinconf_static
    EXPORT pinconfTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}   # for static libraries
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pinconf/
)

install(FILES ${CMAKE_BINARY_DIR}/pinconf.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# ---------------------------------------------------------------------
# CMake package configuration (for find_package)
# ---------------------------------------------------------------------
include(CMakePackageConfigHelpers)

# Generate version file
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/pinconfConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Create a config file template
set(CONFIG_TEMPLATE ${CMAKE_BINARY_DIR}/pinconfConfig.cmake.in)
file(WRITE ${CONFIG_TEMPLATE}
"@PACKAGE_INIT@

# Find our dependency
find_package(pinlog REQUIRED)

# Import targets
include(\${CMAKE_CURRENT_LIST_DIR}/pinconfTargets.cmake)

# Provide convenience aliases (optional)
if(NOT TARGET pinconf::pinconf)
    add_library(pinconf::pinconf ALIAS pinconf)
endif()
")

# Configure the file (this substitutes @PACKAGE_INIT@ and paths)
configure_package_config_file(
    ${CONFIG_TEMPLATE}
    ${CMAKE_BINARY_DIR}/pinconfConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pinconf
    PATH_VARS CMAKE_INSTALL_PREFIX
)

# Install config and version files
install(FILES
    ${CMAKE_BINARY_DIR}/pinconfConfig.cmake
    ${CMAKE_BINARY_DIR}/pinconfConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pinconf
)

# Export the targets (so they become imported targets for consumers)
install(EXPORT pinconfTargets
    FILE pinconfTargets.cmake
    NAMESPACE pinconf::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pinconf
)

# (Optional) Export from build tree for testing
export(EXPORT pinconfTargets
    FILE ${CMAKE_BINARY_DIR}/pinconfTargets.cmake
    NAMESPACE pinconf::
)
