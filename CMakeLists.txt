cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(DYHighMass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ROOT REQUIRED)
find_package(yaml-cpp REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB sources
  ${PROJECT_SOURCE_DIR}/src/*.cc
)

file(GLOB headers
  ${PROJECT_SOURCE_DIR}/include/*.h
)

include(${ROOT_USE_FILE})

add_library(DYHighMassLIB SHARED ${sources})

target_include_directories(DYHighMassLIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS} ${YAML_CPP_INCLUDE_DIR})

target_link_libraries(
  DYHighMassLIB
  ${ROOT_LIBRARIES}
  ${YAML_CPP_LIBRARIES}
)

target_include_directories(DYHighMassLIB PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(DYHighMassLIB PROPERTIES PUBLIC_HEADER "${headers}")

install(TARGETS DYHighMassLIB EXPORT DYHighMassTarget
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev
)

add_executable(analysis main/analysis.cc ${sources} ${headers})

target_link_libraries(
  analysis
  ${ROOT_LIBRARIES}
  ${YAML_CPP_LIBRARIES}
  ${CMAKE_DL_LIBS}
)

install(FILES
  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

install(TARGETS analysis DESTINATION bin)
