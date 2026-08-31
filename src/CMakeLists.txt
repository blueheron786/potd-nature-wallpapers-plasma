cmake_minimum_required(VERSION 3.16)

project(potd-nature-wallpapers LANGUAGES CXX)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(Qt6 REQUIRED COMPONENTS Core Gui)
find_package(KF6 REQUIRED COMPONENTS CoreAddons KIO)
find_package(PlasmaPotdProvider REQUIRED)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/lib"
)

# Set Qt options
set(QT_QMAKE_OUTPUT_OVERRIDDEN 1)

kcoreaddons_add_plugin(
    plasma_potd_natureprovider
    SOURCES natureprovider.cpp
    INSTALL_NAMESPACE "potd"
)

target_link_libraries(
    plasma_potd_natureprovider
    Qt6::Core
    Qt6::Gui
    plasmapotdprovidercore
    KF6::KIOCore
)

# Install plugin metadata
install(FILES natureprovider.json
        DESTINATION ${KDE_INSTALL_KSERVICETYPES5DIR}
        RENAME plasma_potd_natureprovider.json
)

# Enable testing
option(BUILD_TESTING "Build unit tests" ON)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
