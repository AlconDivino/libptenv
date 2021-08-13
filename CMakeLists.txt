cmake_minimum_required(VERSION 3.20)
project(libptenv)

set(CMAKE_CXX_STANDARD 14)

include_directories(include ../Ressources/nlohmann ../Ressources/libtorch/include ../Ressources/libtorch/include/torch/csrc/api/include)

link_directories(/usr/local/lib)

add_library(libptenv SHARED
        src/indicators.cpp
        src/dataManager.cpp
        src/gaf.cpp
        src/threadSync.cpp
        src/environment.cpp
        src/gaf_single.cpp
)
