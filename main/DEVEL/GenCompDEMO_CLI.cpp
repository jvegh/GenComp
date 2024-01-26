/**
 * @file GenCompDEVEL_CLI.cpp
 *  @ingroup GENCOMP_MODULE_STUFF
 *
 * @brief The main file of the command line version of the SystemC-based basic electronic simulator
 *
 * @param[in] argc Number of parameters
 * @param[in] argv parameters, #1 is the name of the command file
 * @return int The result of the execution
 */

#include <systemc>
#include <QApplication>
#include <QTextEdit>
#define MAKE_TIME_BENCHMARKING
// Those defines must be located before 'Macros.h", and are undefined in that file
//#include "Macros.h"

sc_time DelayUnit; // Set globally in the main program
extern QTextEdit *Simulator_LogWindow; // By default and for CLI, we have no QTextEdit


#include "Project.h"
#include <chrono>
#define MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
#include "MacroTimeBenchmarking.h"    // Must be after the define to have its effect

// This section configures debug and log printing
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    // Print event trace debug messages
#define DEBUG_PRINTS    // Print general debug messages
// Those defines must be located before 'Macros.h", and are undefined in that file
// #include "Macros.h"
#include "scClusterBusSimulator.h"

//??scClusterBusSimulator* TheSimulator;
//??string ListOfIniFiles;

ool UNIT_TESTING = false; // Used internally for debugging
QTextEdit *Simulator_LogWindow = 0; // By default and for CLI, we have no QTextEdit

int sc_main(int argc, char* argv[])
{
//    ListOfIniFiles = INI_FILES; // Provide the needed initialization information files
    int returnValue= 0;

    // The simulator makes self-timing
 /*   TheSimulator = new scClusterBusSimulator("BASIC/Adder", argc, argv);

    std::cerr << "Total time for setting up " << TheSimulator->SimulationName_Get() << " was "
              << TheSimulator->SimulationSumTime_Get()/1000/1000 << " msecs" << std::endl;
    sc_start(MAX_CLOCK_CYCLES,SC_NS);

    std::cerr << "Part time for setting running " << TheSimulator->SimulationName_Get() << " was "
              << TheSimulator->SimulationPartTime_Get()/1000/1000 << " msecs" << std::endl;
    std::cerr << "Total time for running " << TheSimulator->SimulationName_Get() << " was "
              << TheSimulator->SimulationSumTime_Get()/1000/1000 << " msecs" << std::endl;
    std::cerr << "Part time for cleaning up " << TheSimulator->SimulationName_Get() << " was "
              << TheSimulator->SimulationPartTime_Get()/1000/1000 << " msecs" << std::endl;
    std::cerr << "Total elapsed time for simulation " << TheSimulator->SimulationName_Get() << " was "
               << TheSimulator->SimulationSumTime_Get()/1000/1000 << " msecs" << std::endl;
     delete TheSimulator; */
    return returnValue;
}
