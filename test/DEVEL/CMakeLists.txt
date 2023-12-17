# This is the CMakeLists file for testing the GecComp/DEVEL components
# The directory structure (and other docs) can be found in cmake/Docs
#
# @author János Végh

message(HIGHLIGHTED "                    GenComp/DEVEL tests")

include_directories(
    include
    ${CMAKE_SOURCE_DIR}/modules/include
    ${CMAKE_INSTALL_PREFIX}/include
    ${CMAKE_SOURCE_DIR}/test/include
    ${SystemC_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

file(GLOB_RECURSE TEST_INCLUDES
    include/*.h
)

file(GLOB_RECURSE TEST_SOURCES
    Test*.cpp
)
#TestUtils.cpp           # Utility funtions
#TestStuff.cpp           # Other processing stuff
#TestGenComp.cpp         # General computing base objects

link_directories(
    ${SystemC_LIBRARY_DIRS}
    ${SCV_LIBRARY_DIRS}
    ${GTEST_LIBRARIES}
)

message(HIGHLIGHTED "                         simple (bus-unrelated) executable")
ADD_EXECUTABLE(
    ${PROJECT_NAME}_DEVEL_simple_GTEST          # The prepared executable
    ${PROJECT_NAME}_DEVEL_simple_gtest.cpp	# Unit testing with gtest, no events
    sctestbench_simple.cpp  # The SystemC interface
    ${TEST_SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME}_DEVEL_simple_GTEST
    GenCompModules          # General computing base classes library
    ${SystemC_LIBRARIES}
    ${GTEST_LIBRARIES}
    pthread
)

install(TARGETS ${PROJECT_NAME}_DEVEL_simple_GTEST
   RUNTIME
   DESTINATION bin
   COMPONENT tests
)

INSTALL(FILES ${TEST_SOURCES} CMakeLists.txt  testbench_simple.cpp
              ${PROJECT_NAME}_DEVEL_simple_gtest.cpp
	DESTINATION test/DEVEL
	COMPONENT srcs)

INSTALL(FILES ${TEST_INCLUDES}
	DESTINATION test/DEVEL/include
	COMPONENT srcs)


if ( "something" STREQUAL "Nothing" )
    #[ -----------------------------
    #   The bus-related testing
    #[ -----------------------------

     #${PROJECT_NAME}Basic_bus_gtest.cpp
    message(HIGHLIGHTED "                          bus-related executable")

    set(TEST_SOURCES
        TestGenComp.cpp
        testbench_bus.cpp
        TestStuff.cpp
        TestUtils.cpp
    #    TestTopology.cpp
    #    TestscGridPoint.cpp
    #    TestRouting.cpp
    #    TestHThread.cpp
    ##    TestMemory.cpp
    #    TestProcessor.cpp
    )


    ADD_EXECUTABLE(
        ${PROJECT_NAME}Basic_GTEST
        ${PROJECT_NAME}Basic_gtest.cpp	# Unit testing with gtest, no events
        ${TEST_SOURCES}
    )


    target_link_libraries(
        ${PROJECT_NAME}Basic_GTEST
        BasicModules
           ${SystemC_LIBRARIES}
           ${GTEST_LIBRARIES}
           pthread
    )
                     link_directories(
                                          ${SystemC_LIBRARY_DIRS}
                                          ${SCV_LIBRARY_DIRS}
                                          )

    install(TARGETS ${PROJECT_NAME}DEVEL_bus_GTEST
       RUNTIME
       DESTINATION bin
       COMPONENT tests)

    endif ( "something" STREQUAL "Nothing" )

