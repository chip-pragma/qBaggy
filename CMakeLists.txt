cmake_minimum_required(VERSION 3.14)
project(BaggyProject)

add_subdirectory(baggy)
add_subdirectory(extractors)

add_custom_target(baggy_cli_with_extractors)
add_dependencies(baggy_cli_with_extractors
        baggy_cli extractor_libraries)

add_custom_target(baggy_app_with_extractors)
add_dependencies(baggy_app_with_extractors
        baggy_app extractor_libraries)