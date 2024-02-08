# This is the main CMakeLists.txt file of the 'cli' library
#
# @author János Végh

# Generate the basic module library

message(HIGHLIGHTED "Configuring GenComp 'cli' library")

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE MY_SRCS
    *.cpp
)

set(MY_SRCS ${MY_SRCS})

file(GLOB_RECURSE MY_HEADERS
     include/*.h
)

include_directories(
       include
       ../stuff/include
       ${Qt6Widgets_INCLUDE_DIRS}
       ${SystemC_INCLUDE_DIRS}
)

link_directories(${SystemC_LIBRARY_DIRS})

ADD_LIBRARY(GenCompCli  STATIC
    ${MY_SRCS}
)

target_link_libraries(GenCompCli
                      Qt6::Widgets
#                      ${SystemC_LIBRARIES}
                      ${Pthread}
 )

set_target_properties(GenCompCli
                      PROPERTIES OUTPUT_NAME GenCompCli
                      )
target_include_directories(GenCompCli PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


# Collect packages
INSTALL(FILES  ${MY_SRCS} CMakeLists.txt
        DESTINATION modules
        COMPONENT srcs)

INSTALL(FILES  ${MY_HEADERS}
        DESTINATION modules/include
        COMPONENT srcs)

