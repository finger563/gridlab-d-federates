cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

SET(execName GridlabDFederate)

project(${execName})

set ( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};./cpp-netlib-build )
find_package ( cppnetlib 0.11.0 REQUIRED )

if("$ENV{C2WTROOT}" MATCHES "^$")
  message(FATAL_ERROR "Need to set C2WTROOT ENV Var!")
endif()
set(C2WTROOT "$ENV{C2WTROOT}")

if("$ENV{RTI_HOME}" MATCHES "^$")
  message(FATAL_ERROR "Need to set RTI_HOME ENV Var!")
endif()
set(RTI_HOME "$ENV{RTI_HOME}")

message("Using c2wtroot: $ENV{C2WTROOT}")
message("Using rti: $ENV{RTI_HOME}")

include_directories(
  ${CPPNETLIB_INCLUDE_DIRS}
  ${RTI_HOME}/include/ng6
  ${C2WTROOT}/core/src/cpp/SynchronizedFederate
  ${C2WTROOT}/generated/GridlabDHelloWorld/cpp/GridlabDHelloWorld/
  ${C2WTROOT}/generated/GridlabDHelloWorld/cpp/GridlabDHelloWorld/include
  ${C2WTROOT}/core/src/cpp/include
  ${C2WTROOT}/generated/core/cpp
  ${C2WTROOT}/generated/core/cpp/include
  )

add_library(gldf_generated GeneratedSources.cpp)

link_directories(${C2WTROOT}/core/src/cpp/SynchronizedFederate)
link_directories(${RTI_HOME}/lib)

# add interface executable
add_executable (${execName} main.cpp)
target_link_libraries(${execName} 
  SynchronizedFederate
  mysqlcppconn 
  boost_thread 
  FedTime
  RTI-NG
  boost_system 
  gldf_generated
  ${CPPNETLIB_LIBRARIES}
  )
