cmake_minimum_required(VERSION 3.16)

# Point to the companders component (parent repo)
set(EXTRA_COMPONENT_DIRS "../../")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(espidf_compander)
