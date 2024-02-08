# This is the main CMakeLists.txt file of the 'modules' library
#
# @author János Végh

# Generate the basic module library

message(HIGHLIGHTED "Configuring GenComp 'modules' library")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE MY_SRCS
    *.cpp
)

set(MY_SRCS ${MY_SRCS})

file(GLOB_RECURSE MY_HEADERS
    include/*.h
##    ${CMAKE_SOURCE_DIR}/3rdParty/Minuit/include/*.h
)

include_directories(
       include
       ${CMAKE_SOURCE_DIR}/3rdParty/Minuit/include

       ${Qt6Widgets_INCLUDE_DIRS}
       ${SystemC_INCLUDE_DIRS}
##       ../3rdParty/QSystemC/include
)

link_directories(${SystemC_LIBRARY_DIRS})

ADD_LIBRARY(GenCompModules  STATIC
    ${MY_SRCS}
)

target_link_libraries(GenCompModules
                      Qt6::Widgets
#                      ${SystemC_LIBRARIES}
                      ${Pthread}
 )

set_target_properties(GenCompModules
                      PROPERTIES OUTPUT_NAME GenCompModules
                      )
target_include_directories(GenCompModules PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


# Collect packages
INSTALL(FILES  ${MY_SRCS} CMakeLists.txt
        DESTINATION modules
        COMPONENT srcs)

INSTALL(FILES  ${MY_HEADERS}
        DESTINATION modules/include
        COMPONENT srcs)

