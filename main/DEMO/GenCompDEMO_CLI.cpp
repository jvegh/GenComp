/**
 * @file GenCompDEMO_CLI.cpp
*  @ingroup GENCOMP_MODULE_STUFF
 *
 * @brief The main file of the command line version of the SystemC-based basic electronic simulator
 *
 * @param[in] argc Number of parameters
 * @param[in] argv parameters, #1 is the name of the command file
 * @return int The result of the execution
 */

//#include <QApplication>
//#include <QTextEdit>
//extern QTextEdit *Simulator_LogWindow; // By default and for CLI, we have no QTextEdit

#include <systemc>
#include <chrono>
#include "Project.h"

#define MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
#include "MacroTimeBenchmarking.h"    // Must be after the define to have its effect
#define SC_MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
#include "scMacroTimeBenchmarking.h"    // Must be after the define to have its effect


// This section configures debug and log printing; must be located AFTER the other includes
//#define SUPPRESS_LOGGING // Suppress all log messages
#define DEBUG_EVENTS    ///< Print event debug messages  for this module
#define DEBUG_PRINTS    ///< Print general debug messages for this module
// Those defines must be located before 'DebugMacros.h", and are undefined in that file
#include "DebugMacros.h"
// These are 'per unit' settings, so set up them in the used modules individually

//#include "scClusterBusSimulator.h"

//??scClusterBusSimulator* TheSimulator;
//??string ListOfIniFiles;

#include "scGenComp_PU_AbstractDEMO.h"
//#include "scGenComp_PU_BioDEMO.h"
//#include "scGenComp_PU_Bio_IzhikevichDEMO.h"
#include "scGenComp_Simulator.h"

//    sc_set_time_resolution(SCTIME_RESOLUTION);
extern string GenCompStatesString[];   // Just for debugging

scGenComp_PU_AbstractDEMO* MyAbstractDEMO;
//scGenComp_PU_BioDEMO* MyBioDEMO;
//scGenComp_PU_Bio_IzhikevichDEMO* MyIzhikevichDEMO;
scGenComp_Simulator* MySimulator;

// Prepare sxXXX modules and instantiate them
int32_t scPrepareGenCompObjects(int32_t ObjectSelector)
{
    switch(ObjectSelector)
    {
        case 0: MyAbstractDEMO = new scGenComp_PU_AbstractDEMO("MyAbstractDEMO"); break;
//        case 1: MyBioDEMO = new scGenComp_PU_BioDEMO("MyBioDEMO"); break;
        default:
        {
            MyAbstractDEMO = new scGenComp_PU_AbstractDEMO("MyAbstractDEMO");
//            MyBioDEMO = new scGenComp_PU_BioDEMO("MyBioDEMO");
        }; break;
    }
    return 0;
}

bool UNIT_TESTING = false; // Used internally for debugging
//QTextEdit *Simulator_LogWindow = 0; // By default and for CLI, we have no QTextEdit

// Using sc_main() is mandatory for using SystemC; equivalent with main()
int sc_main(int argc, char* argv[])
{
    bool UseSimulator = true;   // Can use either scGenComp_Simulator or stand-alone unit operating modes
    int returnValue=0;
    // We rely on the default clearing of the values of time benchmarking
    chrono::steady_clock::time_point t;
    std::chrono::duration<int64_t, nano> x,s=(std::chrono::duration< int64_t, nano>)0;
    sc_core::sc_time SC_t, SC_x, SC_s;
    BENCHMARK_TIME_RESET(&t,&x,&s);
    SC_BENCHMARK_TIME_RESET(&SC_t,&SC_x,&SC_s);
    sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",
                                            sc_core::SC_DO_NOTHING );
    // About to start
    std::cerr  << "\n Demonstrates using GenComp with SystemC" << "\n>>> Entering " << PROJECT_NAME << "_DEMO/CLI V"
              << PROJECT_VERSION;
    if(UseSimulator)
        std::cout << " with simulator";
    std::cout << endl;

    // Do whatever setup you will need for your program here
    //
    // You may use your object without a frame (without using a simulator)
    // Just prepare your sc_modules in a wrapper; the functionality is defined in its Initialize method
     // ------Make anything before starting
    BENCHMARK_TIME_BEGIN(&t,&x);
    SC_BENCHMARK_TIME_BEGIN(&SC_t,&SC_x);
    if(!UseSimulator)
    {
        // !! all SC-related object and connections must be established before calling the simulator!!
        int32_t ObjectSelector = -1;    // Select all test objects
        returnValue = scPrepareGenCompObjects(ObjectSelector);
    }
    else
    {// Create a simulator
        MySimulator = new scGenComp_Simulator("MySim");
        // Create all units you need

        MyAbstractDEMO = new scGenComp_PU_AbstractDEMO("MyAbstract");
//        MyBioDEMO = new scGenComp_PU_BioDEMO("MyBio");
//        MyIzhikevichDEMO = new scGenComp_PU_Bio_IzhikevichDEMO("MyIzhikevich",sc_core::sc_time(200,SC_US));
        // All units must be created before registering
//        MySimulator->RegisterPU(MyBioDEMO);
        MySimulator->RegisterPU(MyAbstractDEMO);
//        MySimulator->RegisterPU(MyIzhikevichDEMO);
    }
    BENCHMARK_TIME_END(&t,&x,&s);
    SC_BENCHMARK_TIME_END(&SC_t,&SC_x,&SC_s);
    // ------!!No instantiation of objects from classes scXXX is allowed outside this section!!
    std::cerr  << "  -Elapsed for preparing simulation: " << x.count()/1000/1000. << " msec CLOCK time" << "//" << sc_time_String_Get(SC_s,SC_TIME_UNIT_DEFAULT) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT] << " SIMULATED time" << endl;

    if(!returnValue)
    {   // It was all right with the preparation, prepare simulation
        //  Set up here the simulator
        BENCHMARK_TIME_BEGIN(&t,&x);
        SC_BENCHMARK_TIME_BEGIN(&SC_t,&SC_x);
        if(!UseSimulator)
            sc_start(); // Will run until dies
        else
        {   // This is the real simulation cycle: runs until end
            // but allows updating any state of the simulated object
            while(MySimulator->Run(gcsm_Eventwise,1))
            {   // This portion is run repeatedly by the simulator
                MySimulator->Update();   // Do updating if anything is to be updated
                // Update display here
            }
//            DEBUG_SC_LOG("Processing time was: " << sc_time_String_Get(sc_core::sc_timeStamp(),SC_TIME_UNIT_DEFAULT));

        };
        BENCHMARK_TIME_END(&t,&x,&s);
        SC_BENCHMARK_TIME_END(&SC_t,&SC_x,&SC_s);
        std::cerr  << "  -Elapsed for running the simulation: " << MySimulator->Time_Get().count()/1000/1000. << " msec CLOCK time" << "//" << sc_time_String_Get(SC_s, SC_TIME_UNIT_DEFAULT) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT] << " SIMULATED time" << endl;
    }
    else
        std::cerr  << "  -Error " << returnValue << " during preparing objects" << endl;

     std::cerr  << "<<< Exiting " << PROJECT_NAME << "_DEMO/CLI V" << PROJECT_VERSION;
    if(returnValue)
        std::cerr << ", error code " << returnValue << endl;
    else
        std::cerr << " with no error"  << endl;
    if(UseSimulator)
        std::cerr  << "GenComp/DEMO by scGenComp_Simulator " << MySimulator->Time_Get().count()/1000/1000. << " msec CLOCK time" << "//"
                  <<  sc_time_String_Get(MySimulator->scTime_Get(), SC_TIME_UNIT_DEFAULT) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT] << " SIMULATED time" << endl;
    else
        std::cerr  << "GenComp/DEMO simulation " << s.count()/1000/1000. << " msec CLOCK time" << "//" << sc_time_String_Get(SC_s, SC_TIME_UNIT_DEFAULT)
                   << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT] << " SIMULATED time" << endl;
    return(returnValue);
}

