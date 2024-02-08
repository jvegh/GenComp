# This is the CMakeLists file for making GenComp CLI/GUI
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "                    GenComp Qt GUI library")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
set(CMAKE_AUTOUIC ON)
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#set(QOBJECT_HEADERS
#    include/dialog.h
#)

    file(GLOB QOBJECT_SOURCES *.cpp)
    file(GLOB QOBJECT_HEADERS include/*.h)
#    file(GLOB QOBJECT_UIS  ${CMAKE_SOURCE_DIR}/QtGUI/forms/*.ui)

#file(GLOB_RECURSE QOBJECT_SOURCES
#    ${RESOURCE_FILE}
#    mainwindow.cpp
#    SimulatorWindow.cpp
#     modules/BASIC/scSimulatorThread.cpp
#    SimulatorWindowBasic.cpp
#    SignalPlotWindowBasic.cpp
#    scSimulatorThread.cpp
#    codeeditor.cpp
#    treemodelcompleter.cpp
#    EMPARegisterFileWidget.cpp
#    EMPASimulatorWidget.cpp
#    RegisterWidget.cpp
#    EMPAMemoryWidget.cpp
#    syntaxhighlighter.cpp
#    Y86RegisterFileWidget.cpp
#    aboutwidget.cpp
#)

#file(GLOB_RECURSE QOBJECT_UIS
#     forms/mainwindow.ui
#    )

include_directories(
      ${Qt6Widgets_INCLUDE_DIRS}
       /opt/systemc/include
       ../3rdParty/QCustomPlot/include
       ../3rdParty/fancytabbar
        include
#        forms
        ../../modules/include
)

ADD_LIBRARY(GenCompQtGUI #SHARED
#    ../../BASIC/QtGUI/QStuff.cpp
#    ${QOBJECT_UIS}
    ${QOBJECT_SOURCES}
    ${QOBJECT_HEADERS}
    ${RC_SRC} # The resources file
)

#set_target_properties(GenCompQtGUI PROPERTIES AUTOMOC TRUE)

#https://stackoverflow.com/questions/28492423/how-to-instruct-cmake-automoc-to-find-external-header


file(GLOB_RECURSE HEADERS_TO_MOC include/ *.h)
#qt5_wrap_cpp(PROCESSED_MOCS
#             ${HEADERS_TO_MOC}
#             TARGET GenCompQtGUI
#             OPTIONS --no-notes) # Don't display a note for the headers which don't produce a moc_*.cpp

target_sources(GenCompQtGUI PRIVATE ${PROCESSED_MOCS}) # This adds generated moc cpps to target
target_sources(GenCompQtGUI PRIVATE ${QOBJECT_UIS}) # This adds generated ui cpps to target

  # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
#add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Printer_DEFINITIONS})

# Use the Widgets module from Qt 5.
target_link_libraries(GenCompQtGUI
    GenCompModules fancytabbar_lib  GenCompCli
    QCustomPlot
)
set_target_properties(GenCompQtGUI
                      PROPERTIES OUTPUT_NAME GenCompQtGUI
                      )

target_include_directories(GenCompQtGUI PUBLIC
      ${Qt6Widgets_INCLUDE_DIRS}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

INSTALL(FILES  ${QOBJECT_SOURCES} resources/images.qrc CMakeLists.txt
        DESTINATION QtGUI
        COMPONENT srcs)


    INSTALL(FILES  ${QOBJECT_IMAGES}  ${MY_IMAGES} resources/Baselogo.png
            DESTINATION QtGUI/images
            COMPONENT srcs)

INSTALL(  FILES CMakeLists.txt
	DESTINATION QtGUI
	COMPONENT srcs)
