cmake_minimum_required(VERSION 3.16)

project(potd-nature-wallpapers-plasma
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "KDE Plasma Nature Wallpapers Picture of the Day Provider"
    HOMEPAGE_URL "https://github.com/blueheron786/potd-nature-wallpapers-plasma"
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# Options
option(BUILD_TESTING "Build unit tests" ON)

# Find required packages
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_MODULE_PATH})

# Include KDE settings
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMAddTests)
include(ECMInstallIcons)
include(GenerateExportHeader)

# Add source directory
add_subdirectory(src)

# Enable testing
if(BUILD_TESTING)
    enable_testing()
endif()

# Install documentation
install(FILES README.md
        DESTINATION ${KDE_INSTALL_DOCDIR}/potd-nature-wallpapers-plasma
)