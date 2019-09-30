set(ROOT_LIST ${CMAKE_CURRENT_LIST_DIR})

function(extractor_init)

    message(STATUS "Buggy plugins...")
    add_custom_target(extractor_libraries)

endfunction(extractor_init)

function(extractor_add_target class include)
    cmake_parse_arguments(arg "" "" "SOURCES;INCLUDE_DIRS;LIBRARIES;PROPERTIES;CATKIN_PACKAGE" ${ARGN})

    foreach (pkg ${arg_CATKIN_PACKAGE})
        list(APPEND catkin_include_dirs ${${pkg}_INCLUDE_DIRS})
        list(APPEND catkin_libraries ${${pkg}_LIBRARIES})
    endforeach (pkg)
    list(REMOVE_DUPLICATES catkin_include_dirs)
    list(REMOVE_DUPLICATES catkin_libraries)

    set(full_target_name lib${class})

    add_library(${full_target_name} SHARED
            ${arg_SOURCES}
            )
    target_include_directories(${full_target_name}
            PRIVATE
            "."
            ${BaggyCore_INCLUDE_DIRS}
            ${rosbag_INCLUDE_DIRS}
            ${catkin_include_dirs}
            ${arg_INCLUDE_DIRS}
            )
    target_link_libraries(${full_target_name}
            PRIVATE
            Qt5::Core
            ${BaggyCore_LIBRARIES}
            ${rosbag_LIBRARIES}
            ${catkin_libraries}
            ${arg_LIBRARIES}
            )
    target_compile_definitions(${full_target_name}
            PRIVATE
            EXTRACTOR_VERSION_MAJOR=${BaggyCore_VERSION_MAJOR}
            EXTRACTOR_VERSION_MINOR=${BaggyCore_VERSION_MINOR}
            )

    set_target_properties(${full_target_name}
            PROPERTIES
            OUTPUT_NAME "${class}"
            LIBRARY_OUTPUT_DIRECTORY ${BP_STD_PLUGIN_DIRECTORY}
            ${arg_PROPERTIES}
            )

    add_dependencies(extractor_libraries ${full_target_name})

    message(STATUS "+++ ${class}")
endfunction(extractor_add_target)