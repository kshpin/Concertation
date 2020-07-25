cmake_minimum_required(VERSION 3.14)
project(ConcertationRealtime)

set(CMAKE_CXX_STANDARD 17)

add_executable(ConcertationRealtime Concertation.cpp SoundProxy.h Processor.h Processor.cpp Renderer.h Renderer.cpp network.h inputOutput/KeyManager.cpp inputOutput/KeyManager.h inputOutput/Key.cpp inputOutput/Key.h Mode.h inputOutput/Button.cpp inputOutput/Button.h inputOutput/ButtonPanel.cpp inputOutput/ButtonPanel.h)

#SFML
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/sfml_cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(ConcertationRealtime ${SFML_LIBRARIES})
endif()

#FFTW
find_library(FFTW_LIB fftw3)
target_link_libraries(ConcertationRealtime ${FFTW_LIB})
