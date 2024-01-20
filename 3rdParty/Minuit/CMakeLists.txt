# This is the CMakeLists file generating Qt6-based QCustomPlot library stuff
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "Configuring ${PROJECT_NAME} QCustomPlot library")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)


ADD_LIBRARY(Minuit  STATIC
    GaussFcn.cpp
    GaussFcn2.cpp
    GaussDataGen.cpp
    include/GaussFunction.h
    include/GaussRandomGen.h
    include/FlatRandomGen.h
)

set_target_properties(Minuit
                      PROPERTIES OUTPUT_NAME Minuit
                      )

target_include_directories(Minuit PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

INSTALL(FILES  GaussFcn.cpp GaussFcn2.cpp GaussDataGen.cpp DemoGaussSim.cpp
	DESTINATION 3rdParty/Minuit
	COMPONENT srcs)

INSTALL(FILES  include/GaussFcn.h include/GaussFcn2.h include/GaussFunction.h
               include/GaussRandomGen.h include/FlatRandomGen.h
               include/FCNBase.h include/GaussDataGen.h
               include/GenericFunction.h include/MnConfig.h
	DESTINATION 3rdParty/Minuit/include
	COMPONENT srcs)


