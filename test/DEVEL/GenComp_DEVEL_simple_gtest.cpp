/** @file GenComp_DEVEL_simple_gtest.cpp
 *  @brief The main file for the test program for testing
 * SystemC-implemented modules using Google-test
 * This main program assumes nothing specific, just SystemC modules
 * and shows how tose modules can be tested used Google-Test
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
// All systemc modules should include systemc.h header file
#include <systemc>
#include "gtest/gtest.h"
#include <chrono>
// Do not log execution in unit testing  mode
//#define SUPPRESS_LOGGING    // In unit testing mode
//#define DEBUGGING       // In debug mode
//#include "Config.h"
//#include "version.h"
#include "utils.h"
//#undef SUPPRESS_LOGGING

#define MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
#include "MacroTimeBenchmarking.h"    // Must be after the define to have its effect
#define SC_MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
#include "scMacroTimeBenchmarking.h"    // Must be after the define to have its effect

//#include "Project.h"
#include "scGTestModule_simple.h"
#include "Utils.h"
unsigned errors = 0;
GenCompDEVEL_simpleTB_t* GenCompDEVEL_simpleTB;

//extern bool OBJECT_FILE_PRINTED;
extern    string SC_TIME_UNIT[];

using namespace std; using namespace sc_core;
string simulation_name = "Test GenComp units without using a bus";
//#define MAX_CLOCK_CYCLES 1000

int sc_main(int argc, char* argv[]) {
    // We rely on the default clearing of the values of time benchmarking
    chrono::steady_clock::time_point t, absolutestart;
    std::chrono::duration<int64_t, nano> x,s=(std::chrono::duration< int64_t, nano>)0;
    sc_core::sc_time SC_t, SC_x, SC_s;
    BENCHMARK_TIME_RESET(&t,&x,&s);
    SC_BENCHMARK_TIME_RESET(&SC_t,&SC_x,&SC_s);
     sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",
                                               sc_core::SC_DO_NOTHING );

    BENCHMARK_TIME_BEGIN(&t,&x);
      testing::InitGoogleTest(&argc, argv);
    BENCHMARK_TIME_END(&t,&x,&s);
    std::cerr  << "  -Elapsed for initializing GTest : " << x.count()/1000 << " usec" << endl;

//    sc_set_time_resolution(SCTIME_RESOLUTION);
    //Do whatever setup here you will need for your tests here

      // Begin testing
    BENCHMARK_TIME_BEGIN(&t,&x);
    GTestModule_simple InitGTest("gtest_simple"); // Set up SystemC related testing
    //Run the Simulation for "MAX_CLOCK_CYCLES  nanosecnds"
    BENCHMARK_TIME_END(&t,&x,&s);

    std::cerr  << "  -Elapsed for setting up testing : " << x.count()/1000 << " usec" << endl;
    // About to start testing wit GTest
    std::cerr << ">>> Entering " << GetProjectName().toStdString() << "_DEVEL/simple V" << getGitVersion().toStdString() << " SystemC unit testing" << endl;
    //
    // Make anything before starting unit testing
    //!! all SC-related object and connections must be established before calling sc_start
    //
    BENCHMARK_TIME_BEGIN(&t,&x);
    SC_BENCHMARK_TIME_BEGIN(&SC_t,&SC_x);
    sc_start(MAX_CLOCK_CYCLES ,SC_US);
    // Return here when no more events remained
    BENCHMARK_TIME_END(&t,&x,&s);
    SC_BENCHMARK_TIME_END(&SC_t,&SC_x,&SC_s);
    std::cerr  << "<<< Exiting " << GetProjectName().toStdString() << "_DEVEL/simple V" << getGitVersion().toStdString() << " SystemC unit testing with ";
    int returnValue = GenCompDEVEL_simpleTB->ErrorCode_Get();
    if(returnValue)
        std::cerr << " error code " << returnValue << endl;
    else
        std::cerr << "no error"  << endl;
    std::cerr  << "SystemC unit testing took " << s.count()/1000/1000. << " msec CLOCK time" << "//"
               << sc_time_String_Get(SC_s, SC_TIME_UNIT_DEFAULT) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT] << " SIMULATED time" << endl;
    return(returnValue);
}

