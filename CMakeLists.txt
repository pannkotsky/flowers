cmake_minimum_required(VERSION 3.10)
project(flowers)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

include_directories(models)
add_executable(flowers
        main.cpp
        models/date.h
        models/flower.h
        models/sale_entity.h
        models/transaction.h
        models/assortment.h
        helpers.h
        models/balance.h models/container.h)
target_link_libraries(flowers ${CURSES_LIBRARIES})
target_link_libraries(flowers menu)
