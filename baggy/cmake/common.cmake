macro(baggy_init_cache)

    set(BAGGY_CORE_QT5_PATH "" CACHE PATH "Qt5 build directory")
    set(BAGGY_CORE_ROS_PATH "" CACHE PATH "Ros install directory")

endmacro(baggy_init_cache)

macro(baggy_find_qt5)
    cmake_parse_arguments(arg "" "" "COMPONENTS" ${ARGN})

    set(path ${BAGGY_CORE_QT5_PATH})
    list(APPEND CMAKE_PREFIX_PATH ${path})
    find_package(Qt5
            COMPONENTS Core ${arg_COMPONENTS}
            PATHS ${path} NO_DEFAULT_PATH)
    message(STATUS "Qt5 found: ${Qt5Core_VERSION}")


    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    unset(path)
endmacro(baggy_find_qt5)

macro(baggy_find_ros)
    cmake_parse_arguments(arg "" "" "PACKAGES" ${ARGN})

    set(path ${BAGGY_CORE_ROS_PATH})
    list(APPEND CMAKE_PREFIX_PATH ${path})

    list(APPEND arg_PACKAGES rosbag)
    foreach (pkg ${arg_PACKAGES})
        find_package(${pkg} REQUIRED PATHS ${path} NO_DEFAULT_PATH)
        message(STATUS "${pkg} found: ${${pkg}_VERSION}")
    endforeach (pkg)

    unset(path)
endmacro(baggy_find_ros)