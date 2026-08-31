cmake_minimum_required(VERSION 3.16)

project(potd-nature-wallpapers-tests LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Test)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

add_executable(test_wallpaper_calculation test_wallpaper_calculation.cpp)

target_link_libraries(test_wallpaper_calculation Qt6::Core Qt6::Test)

add_test(NAME wallpaper_calculation COMMAND test_wallpaper_calculation)

# Install test (optional, for CI)
# install(TARGETS test_wallpaper_calculation DESTINATION ${KDE_INSTALL_LIBEXECDIR})