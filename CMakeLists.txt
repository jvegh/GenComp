##############################################################
#
#  This is the major CMake configuration file for the SystemC
#  TimeAwareSimulation of the Generalized Computingproject.
#  To use it you need CMake which can be
#  downloaded from here:
#    http://www.cmake.org/cmake/resources/software.html
#
# The directory structure (and other docs) can be found in cmake/Docs
##############################################################
#
# @author János Végh
#[ --- These are mandatory introduction offset
cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake
                      ${CMAKE_CURRENT_SOURCE_DIR}/..cmake)

# Protect against using bad work directory
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed.\
    Please make a new directory (called a build directory) and run CMake from there.\
    You may need to remove CMakeCache.txt." )
endif()
#[ --- End of mandatory introduction offset

#[ --- Set the name and version of your project here
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0007 NEW)
include(Colorize)   # Just to make some beauty
    set(CPACK_PACKAGE_NAME "GenComp")
    set(MyVersion "0.0.2")
    set(MyDate "2024.01.19")
    project(GenComp VERSION ${MyVersion} LANGUAGES C CXX)

include(BuildType)

# ---[ Set up CMake options for the simulator
# Choose if clocked physical bus is to be used; slows down simulation
  option (USE_CLOCKED_BUS "Simulate clocked physical bus" OFF)
# Choose whether you need debugging support for development or making a release
  option (DEBUG_MODE "Include debug support for the package"  OFF)
# Choose whether the stand-alone unit testing is to be built
  option (BUILD_TESTS "Include unit tests for the package"  ON)
# Choose whether to make documention as well
  option (BUILD_DOCS "Prepare also HTML/CHM/PDF documentation" ON)
# Choose whether you need internal information to docs
  option (DOXY_INTERNAL "Include internal info in the docs"  YES)
#  include(BuildType)
# ---[ End up CMake options for the user


# ---[ DO NOT EDIT BELOW THIS LINE UNTIL YOU KNOW WHAT YOU ARE DOING
set(GENCOMP_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(GENCOMP_BINARY_DIR ${CMAKE_BINARY_DIR})

#[- Find the SystemC basic component; it is a must
# Temporary hack:
set(SystemC_INCLUDE_DIRS "/opt/systemc/include")
set(SystemC_LIBRARY_DIRS "/opt/systemc/lib")
find_package(SystemC REQUIRED)
if(SystemC_FOUND)
    include_directories(${SystemC_INCLUDE_DIRS})
    link_directories(${SystemC_LIBRARY_DIRS})
else()
    message( FATAL_ERROR "SystemC library not found." )
endif()

#[- Find the SystemC verification library component; optional
#if(SCV_FOUND)
#    add_definitions(-DWITH_SCV)
#    include_directories(${SCV_INCLUDE_DIRS})
#    link_directories(${SCV_LIBRARY_DIRS})
#else()
#    message( FATAL_ERROR "SystemC Verification library not found." )
#endif(SCV_FOUND)



#     "C++ standard to build all targets. Supported values are 98, 11, and 14.")
#set (CMAKE_CXX_STANDARD_REQUIRED ${SystemC_CXX_STANDARD_REQUIRED} CACHE BOOL
# Set c++ standard forcefully
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#[ --. Now decide if to enable assertions
if(DEBUG_MODE)
else(DEBUG_MODE)
  add_definitions(-DNDEBUG)
endif(DEBUG_MODE)




##############################################################
## Build SystemC
##############################################################
if(NOT DEFINED ENV{SYSTEMC_HOME})

	find_program (CONAN_EXE NAMES conan REQUIRED)

	if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
	  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
	                "${CMAKE_BINARY_DIR}/conan.cmake"
	                TLS_VERIFY ON)
	endif()

	include(${CMAKE_BINARY_DIR}/conan.cmake)
	conan_add_remote(NAME minres URL https://git.minres.com/api/packages/Tooling/conan)
 	conan_cmake_configure(REQUIRES systemc/2.3.3 systemc-cci/1.0.0 systemc-scv/2.0.1
                                       gtest/1.13.0
                                       xkbcommon/1.4.0
                                       opengl/system
	                      OPTIONS systemc-cci:shared=False
	                      SETTINGS compiler.cppstd=${CMAKE_CXX_STANDARD}
	                      GENERATORS cmake_find_package)

	conan_cmake_autodetect(settings)

	conan_cmake_install(PATH_OR_REFERENCE .
	                    BUILD missing
	                    SETTINGS ${settings} compiler.cppstd=${CMAKE_CXX_STANDARD})

	find_package(SystemCLanguage)
#	find_package(systemc-scv)
#	find_package(systemc-cci)

else()
	find_package(SystemC REQUIRED)
endif()


##############################################################
### build Qt                                                                                                 cmake
##############################################################
# Ideas from https://github.com/Andrew9317/qt-cmake-template/blob/main/CMakeLists.txt
set(QT_MAJOR_VERSION 6)
#See if the environment var is set
if(DEFINED ENV{Qt${QT_MAJOR_VERSION}_HOME})
    message(STATUS "Looking for Qt in: " $ENV{Qt${QT_MAJOR_VERSION}_HOME})
else()
    message(STATUS "Qt${QT_MAJOR_VERSION}_HOME environment variable not set. Checking default paths.")
endif()

set(CMAKE_AUTOMOC ON)
#set(CMAKE_PREFIX_PATH "~/Qt/6.4.2/gcc_64")
set(CMAKE_PREFIX_PATH "~/Qt6/6.6.1/gcc_64")
set(CMAKE_MODULE_PATH "${CMAKE_PREFIX_PATH}/lib/cmake/")
#find_package should find everything fine so long as the ENV Variable is set or, for linux systems,
#it is in the default install path.
find_package(Qt${QT_MAJOR_VERSION} COMPONENTS Widgets PrintSupport REQUIRED PATHS $ENV{Qt${QT_MAJOR_VERSION}_HOME})
##############################################################
# Copy the version and other configuration info to the source
##############################################################
  message(HIGHLIGHTED "You are setting up a time-aware electronic simulator for modern computing")
  message(HIGHLIGHTED "Configuring ${PROJECT_NAME} V${PROJECT_VERSION} system with release date @${MyDate}")
  # Enable to configure from the root
  configure_file(${CMAKE_SOURCE_DIR}/Project.h.in ${CMAKE_SOURCE_DIR}/modules/include/Project.h)
  configure_file(${CMAKE_SOURCE_DIR}/HWConfig.h.in ${CMAKE_SOURCE_DIR}/modules/include/HWConfig.h)
##############################################################
# Add 3-rd party modules
##############################################################
  add_subdirectory(3rdParty)    # Add QSysC, QCustomPlot
  add_subdirectory(modules)	# Hardware module library
  add_subdirectory(main)	# The executables for SystemC&Qt based simulators
  add_subdirectory(QtGUI)	# The Qt-based graphics routines
##  add_subdirectory(examples)	# Examples for simulations

##############################################################
# Build the testing environment
##############################################################
if(BUILD_TESTS)
find_package(GTest REQUIRED)
  add_subdirectory(test)	# Tests for the tool library
endif(BUILD_TESTS)
#file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/data")

##############################################################
# build documentation
##############################################################
if(BUILD_DOCS)
  find_package(Doxygen)
  add_subdirectory(doc) # The doxygen documentation
  set(doxyfile_in "${GENCOMP_SOURCE_DIR}/doc/Doxyfile.in")
endif(BUILD_DOCS)

##############################################################
# build packages
##############################################################
# ---[ Target 'package' builds everything
include(cmake/Packages.cmake)

##############################################################
# ---[ Print configuration summary
##############################################################
include(cmake/Summary.cmake)
include(cmake/Utils.cmake)
caffe_print_configuration_summary()
# https://wiki.qt.io/Install_Qt_5_on_Ubuntu

# Many useful pieces!
# https://github.com/anassar?tab=repositories
# https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/
# https://stackoverflow.com/questions/54753326/cmake-add-external-project

