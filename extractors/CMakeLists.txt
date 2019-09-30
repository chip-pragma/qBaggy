cmake_minimum_required(VERSION 3.14)
project(BaggyExtractors)

include(cmake/ExtractorTarget.cmake)

# set dirs
set(BP_STD_CORE_DIR "" CACHE PATH "BaggyCore build directory")

# params
set(BP_STD_PLUGIN_DIRECTORY "" CACHE PATH "Baggy plugin directory")

# packs
find_package(BaggyCore REQUIRED PATHS ${BP_STD_CORE_DIR} NO_DEFAULT_PATH)

baggy_init_cache()

baggy_find_qt5()
baggy_find_ros(PACKAGES sensor_msgs OpenCV cv_bridge)

# target
extractor_init()

extractor_add_target(SensorImuExtractor source/SensorImuExtractor.h
        SOURCES source/SensorImuExtractor.cpp CATKIN_PACKAGE sensor_msgs)
extractor_add_target(SensorImageExtractor source/SensorImageExtractor.h
        SOURCES source/SensorImageExtractor.cpp CATKIN_PACKAGE sensor_msgs OpenCV cv_bridge)
extractor_add_target(SensorLaserScanExtractor source/SensorLaserScanExtractor.h
        SOURCES source/SensorLaserScanExtractor.cpp CATKIN_PACKAGE sensor_msgs)
extractor_add_target(SensorMagneticFieldExtractor source/SensorMagneticFieldExtractor.h
        SOURCES source/SensorMagneticFieldExtractor.cpp CATKIN_PACKAGE sensor_msgs)