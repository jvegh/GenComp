# This is the CMakeLists file for making GenComp DEMO main programs
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh


# Maybe you need also
#if(YourCoundition)
#  add_subdirectory(YourMainSubdirectory)	# The specific simulator stuff
#endif(YourCoundition)

set(MY_SRCS
#    ${PROJECT_NAME}_CLI.cpp
)

file(GLOB_RECURSE MY_HEADERS
    include/*.h
    ${CMAKE_SOURCE_DIR}/modules/include/*.h
)

# Maybe you need something similar
#file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/data/obj")

#file(GLOB MY_OBJECTS "${CMAKE_SOURCE_DIR}/Myfiles/source/*.E*")

#file(COPY ${MY_OBJECTS} DESTINATION "${CMAKE_BINARY_DIR}/data/obj")



set(CMAKE_AUTOMOC ON)
include_directories(
        include
        forms
        ${CMAKE_SOURCE_DIR}/modules/include
        ../../..//modules/include
        ${CMAKE_SOURCE_DIR}/3rdParty/QCustomPlot/include
        ${CMAKE_SOURCE_DIR}/3rdParty/Minuit/include
        ${Qt6Core_INCLUDE_DIRS}
        ${Qt6Gui_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
        ${Qt6PrintSupport_INCLUDE_DIRS}
        ${SystemC_INCLUDE_DIRS}
#        ${SCV_INCLUDE_DIRS}
)
    link_directories(
        ${CMAKE_SOURCE_DIR}/3rdParty
        ${SystemC_LIBRARY_DIRS}
        ${SCV_LIBRARY_DIRS}
)


message(HIGHLIGHTED "                    DEMO CLI exutable")

add_executable(${PROJECT_NAME}DEMO_CLI #../QtGUI/resources/images.qrc
        ${PROJECT_NAME}DEMO_CLI.cpp
        ${MY_SRCS}
        ${hdr_moc} # The MOC headers, generated
        )
target_link_libraries(${PROJECT_NAME}DEMO_CLI
     GenCompModules  Minuit
     ${SystemC_LIBRARIES}
        Qt6::Widgets
        Qt6::PrintSupport
)



INSTALL(FILES	${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}DEMO_CLI
        DESTINATION MyFiles/bin
        COMPONENT apps)

if ( "something" STREQUAL "Nothing" )

message(HIGHLIGHTED "                   BASIC GUI exutable")

set(RESOURCE_FILE ${CMAKE_SOURCE_DIR}/main/BASIC/resources/images.qrc)
if(EXISTS ${RESOURCE_FILE})
    qt_add_resources(RC_SRC ${RESOURCE_FILE})
else()
    message("resource file " ${RESOURCE_FILE} " NOT found")
endif()

add_executable(${PROJECT_NAME}Basic_GUI ${RC_SRC}
        ${PROJECT_NAME}Basic_GUI.cpp
       ${MY_SRCS}
 	${hdr_moc} # The MOC headers, generated
        )
target_link_libraries(${PROJECT_NAME}Basic_GUI
     BasicModules
                      ${SystemC_LIBRARIES}
     libBasicQtGUI
     libQCustomPlot
        Qt6::Widgets
        Qt6::PrintSupport
#         Qt6::Concurrent
)
endif ( "something" STREQUAL "Nothing" )

if ( "something" STREQUAL "Nothing" )

message(HIGHLIGHTED "                   BASIC Gates CLI exutable")

add_executable(${PROJECT_NAME}BasicGates_CLI #../QtGUI/resources/images.qrc
        ${PROJECT_NAME}BasicGates_CLI.cpp
       ${MY_SRCS}
        ${hdr_moc} # The MOC headers, generated
        )
target_link_libraries(${PROJECT_NAME}BasicGates_CLI
     BasicModules
                      ${SystemC_LIBRARIES}
        Qt6::Widgets
        Qt6::PrintSupport
)


INSTALL(FILES	${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}Basic_GUI
        DESTINATION MyFiles/bin
        COMPONENT apps)



INSTALL(FILES ../CMakeLists.txt
	DESTINATION ..
	COMPONENT srcs)

endif ( "something" STREQUAL "Nothing" )
#INSTALL(  FILES CMakeLists.txt
#	DESTINATION ThirdParty
#	COMPONENT srcs)

