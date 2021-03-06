cmake_minimum_required(VERSION 2.8)
project(mock_data)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_CXX_FLAGS "-O3 -std=c++1y -Wall")
set(CMAKE_SHARED_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

find_package(Boost REQUIRED COMPONENTS program_options)
include_directories("${Boost_INCLUDE_DIRS}")
link_directories("${Boost_LIBRARY_DIR}")
MESSAGE("Boost information:") 
MESSAGE("  Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}") 
MESSAGE("  Boost_LIBRARIES: ${Boost_LIBRARIES}") 
MESSAGE("  Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}") 

find_package(ROOT REQUIRED)
include_directories("${ROOT_INCLUDE_DIR}")
link_directories("${ROOT_LIBRARY_DIR}")

option(ROOT6 "ROOT6" ON)
if(ROOT6)
  message("-- ROOT 6 CMake option set ON (default)")
else()
  message("-- ROOT 6 CMake option set OFF")
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()		     			

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include")

ROOT_GENERATE_DICTIONARY(
  AnalyzerDict
  include/Analyzer.h
  LINKDEF include/LinkDef.h
  )

file(GLOB analyzer_src
  "src/*.cxx"
  )

add_library(
  Analyzer SHARED
  ${analyzer_src}
  AnalyzerDict
  )

#add_executable(aye aye.cxx ${ROOT_LIBRARIES} ${Boost_LIBRARIES})
#target_link_libraries(develop LArgon ${ROOT_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(Analyzer ${ROOT_LIBRARIES})

if(ROOT6)
  install(FILES ${CMAKE_BINARY_DIR}/AnalyzerDict_rdict.pcm DESTINATION "${CMAKE_SOURCE_DIR}/lib")
endif()
