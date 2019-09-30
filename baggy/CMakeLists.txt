cmake_minimum_required(VERSION 3.14)
string(TIMESTAMP today "%y%m")
project(Baggy VERSION 0.7.${today})
message(STATUS "== qBaggy ${PROJECT_VERSION} ==")

set(CMAKE_CXX_STANDARD 14)

# info
string(TIMESTAMP year "%Y")
add_compile_definitions(
        BAGGY_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        BAGGY_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        BAGGY_VERSION_PATCH=${PROJECT_VERSION_PATCH}
        BAGGY_AUTHOR="Ivan Cheltsov <chip.pragma@gmail.com> © ${year}"
)

include(cmake/common.cmake)

baggy_init_cache()

baggy_find_qt5(COMPONENTS Gui Quick QuickControls2)
baggy_find_ros(PACKAGES std_msgs geometry_msgs)

# core
set(core_prefix source/core)
file(GLOB_RECURSE core_sources "${core_prefix}/*.cpp")
add_library(baggy_core SHARED ${core_sources})
target_include_directories(baggy_core
        PUBLIC ${core_prefix} include/
        ${rosbag_INCLUDE_DIRS}
        ${std_msgs_INCLUDE_DIRS}
        ${geometry_msgs_INCLUDE_DIRS})
target_compile_definitions(baggy_core
        PUBLIC BAGGY_CORE_VERSION="${PROJECT_VERSION}")
target_link_libraries(baggy_core
        PUBLIC
        Qt5::Core
        ${rosbag_LIBRARIES}
        ${std_msgs_LIBRARIES}
        ${geometry_msgs_LIBRARIES})
set_target_properties(baggy_core
        PROPERTIES
        OUTPUT_NAME "BaggyCore"
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib/BaggyCore/
        )

configure_file(
        template/BaggyCoreConfig.cmake.in
        ${CMAKE_CURRENT_LIST_DIR}/cmake/BaggyCoreConfig.cmake
        @ONLY
)

# app
set(app_prefix source/app)
file(GLOB_RECURSE app_sources "${app_prefix}/*.cpp")
file(GLOB_RECURSE app_resources "${app_prefix}/*.qrc")
add_executable(baggy_app ${app_sources} ${app_resources})

add_dependencies(baggy_app baggy_core)
target_include_directories(baggy_app
        PRIVATE
        ${app_prefix}
        )
target_link_libraries(baggy_app
        baggy_core
        Qt5::Gui
        Qt5::Quick
        Qt5::QuickControls2
        )
set_target_properties(baggy_app
        PROPERTIES
        OUTPUT_NAME "qbaggy"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
        )

# cli
set(cli_prefix source/cli)
file(GLOB_RECURSE cli_sources "${cli_prefix}/*.cpp")
add_executable(baggy_cli ${cli_sources})

add_dependencies(baggy_cli baggy_core)
target_include_directories(baggy_cli
        PRIVATE
        ${cli_prefix}
        )
target_link_libraries(baggy_cli
        baggy_core
        )
set_target_properties(baggy_cli
        PROPERTIES
        OUTPUT_NAME "qbcli"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
        )