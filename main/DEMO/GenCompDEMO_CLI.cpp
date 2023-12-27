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

#include "scBioGenComp_PU.h"

class scSimulator : public sc_core::sc_module
{
public:
    SC_HAS_PROCESS(scSimulator);
     scSimulator(sc_core::sc_module_name nm);
    /**
     * @brief Run until the next simulated event
     * @return
     */
    bool Run();
     /**
      * @brief Register
      * @param Module the activity of which must be watched
      */
    void Register(sc_core::sc_module* Module);
    /**
     * @brief scLocalTime_Set
     * @param T The beginning of the simulated time of the recent operation
     */
    void scLocalTime_Set(sc_core::sc_time T = sc_core::sc_time_stamp()){    mLocalTimeBase = T;}
    sc_core::sc_time scTimeBase_Get(void){return mLocalTimeBase;}
    bool HasMoreToDo(void){ return mMoreEvents;}
protected:
    sc_core::sc_time mLocalTimeBase;    // The beginning of the local computing
    bool mMoreEvents; ///< If we have more events to simulate
    vector<sc_core::sc_module*> mWatchedModules;   /// Store the registered objects here
};


scSimulator::scSimulator(sc_core::sc_module_name nm)
    : sc_core::sc_module( nm)
    ,mLocalTimeBase(sc_core::SC_ZERO_TIME)
    ,mMoreEvents(false)
{
}

bool scSimulator::Run()
{
    // We are in the process of simulating
    sc_process_handle T1 =  sc_get_current_process_handle();
    sc_object* P1 = T1.get_parent_object();
    sc_core::sc_time MyT = sc_time_to_pending_activity() ;
    DEBUG_SC_PRINT("Next time =" << MyT);
    sc_start( sc_time_to_pending_activity() );     // Run up to the next activity}
    sc_process_handle T =  sc_get_current_process_handle();
    sc_object* P = T.get_parent_object();
    if(P)
    {
    DEBUG_SC_PRINT(" handle of process '" << P->name() << "' received");
    }
    else
    {
        DEBUG_SC_PRINT(" Invalid process handle received");
    }
    mMoreEvents = sc_pending_activity();
    DEBUG_SC_PRINT(" Running continues= " << mMoreEvents);
    return mMoreEvents;
}

// Register only already created modules !!!
void scSimulator::Register(sc_core::sc_module* Module)
{
    mWatchedModules.push_back(Module);
    DEBUG_SC_PRINT("Module " << Module->name() << " registered");
}


//    sc_set_time_resolution(SCTIME_RESOLUTION);
extern string GenCompStates[];   // Just for debugging

class scDemoBioGenComp_PU : public scBioGenComp_PU
{
public:
    scDemoBioGenComp_PU(sc_core::sc_module_name nm):
        scBioGenComp_PU(nm)
    {
        typedef scDemoBioGenComp_PU SC_CURRENT_USER_MODULE;
        SC_THREAD(PrintState);
//        sensitive << scPrintEvent;
        // dont_initialize(); // Allow calling it at start
        //PrintState();   // Begins with printing, but waits sometimes
    }
    void Initialize()
    {   // The PU starts immediatley and receives three spikes

#define INPUTS_MESSAGE  "Unit is in state '" << GenCompStates[mStateFlag] << "' and received " << Inputs.size() << " inputs"
                DEBUG_SC_PRINT(">>>");
                // These DEBUG_XXX macros compile to nothing if
        EVENT_GenComp.InputReceived.notify(SC_ZERO_TIME);
                DEBUG_SC_EVENT_LOCAL(INPUTS_MESSAGE);
        EVENT_GenComp.InputReceived.notify(13,SC_US);     // The BPU starts to receive spikes
                DEBUG_SC_EVENT(INPUTS_MESSAGE);
        EVENT_GenComp.InputReceived.notify(15,SC_MS);     // The BPU starts to receive spikes
                 DEBUG_SC_EVENT(INPUTS_MESSAGE);
                DEBUG_SC_PRINT("Initialize exited");
                DEBUG_SC_PRINT_LOCAL("<<<");
    }
    void PrintState()
    {   // Unit receives inputs at 0 and 28; resets at 23
        DEBUG_SC_PRINT(INPUTS_MESSAGE);
        wait(15,SC_US);
        DEBUG_SC_PRINT(INPUTS_MESSAGE);
        wait(8,SC_US);
        scBioGenComp_PU::Initialize();                             // Reset local time
        DEBUG_SC_PRINT_LOCAL(INPUTS_MESSAGE);
    }
};

// Prepare sxXXX modules and instantiate them
scDemoBioGenComp_PU* MyBio;
scSimulator* MySimulator;
int32_t scPrepareGenCompObjects(int32_t ObjectSelector)
{
    switch(ObjectSelector)
    {
        case 0: MyBio = new scDemoBioGenComp_PU("MyBio"); break;
        default:
        {
            MyBio = new scDemoBioGenComp_PU("MyBioAll");
        }; break;
    }
    return 0;
}


bool UNIT_TESTING = false; // Used internally for debugging
//QTextEdit *Simulator_LogWindow = 0; // By default and for CLI, we have no QTextEdit

// Using sc_main() is mandatory for using SystemC; equivalent with main()
int sc_main(int argc, char* argv[])
{ 
    // We rely on the default clearing of the values of time benchmarking
    chrono::steady_clock::time_point t;
    std::chrono::duration<int64_t, nano> x,s=(std::chrono::duration< int64_t, nano>)0;
    sc_core::sc_time SC_t, SC_x, SC_s;
    BENCHMARK_TIME_RESET(&t,&x,&s);
    SC_BENCHMARK_TIME_RESET(&SC_t,&SC_x,&SC_s);
    sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",
                                            sc_core::SC_DO_NOTHING );
    // About to start
    std::cerr  << "\n Demonstrates using GenComp with SystemC" << "\n>>> Entering " << PROJECT_NAME << "_DEMO/CLI V" << PROJECT_VERSION << endl;

    // Do whatever setup you will need for your program here
    //

    bool UseSimulator = true;
    int returnValue=0;
    // You may use your object without a frame (without using a simulator)
    // Just prepare your sc_modules in a wrapper; the functionality is defined in its Initialize method
     // ------Make anything before starting
    BENCHMARK_TIME_BEGIN(&t,&x);
    SC_BENCHMARK_TIME_BEGIN(&SC_t,&SC_x);
    if(!UseSimulator)
    {
        // !! all SC-related object and connections must be established before calling the simulator!!
        int32_t ObjectSelector = -1;    // Select one of the test objects or all
        returnValue = scPrepareGenCompObjects(ObjectSelector);
    }
    else
    {// Create a simulator
        // Must be created before registering
        MyBio = new scDemoBioGenComp_PU("MyBio");
        MySimulator = new scSimulator("MySim");
        MySimulator->Register(MyBio);
    }
    BENCHMARK_TIME_END(&t,&x,&s);
    SC_BENCHMARK_TIME_END(&SC_t,&SC_x,&SC_s);
    // ------!!No instantiation of objects from classes scXXX is allowed outside this section!!
    std::cerr  << "  -Elapsed for preparing simulation: " << x.count()/1000/1000. << " msec CLOCK time" << "//" << sc_time_String_Get(SC_TIME_UNIT_DEFAULT,SC_s) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT] << " SIMULATED time" << endl;

    if(!returnValue)
    {   // It was all right with the preparation, prepare simulation
        //  Set up here the simulator
        BENCHMARK_TIME_BEGIN(&t,&x);
        SC_BENCHMARK_TIME_BEGIN(&SC_t,&SC_x);
        if(!UseSimulator)
            sc_start();
        else
        {    // We are at the beginning, just make a call to set up the SystemC engine
            sc_start(SC_ZERO_TIME);
            while(MySimulator->Run())
            {
                DEBUG_PRINT("Again");
            }
        };
        BENCHMARK_TIME_END(&t,&x,&s);
        SC_BENCHMARK_TIME_END(&SC_t,&SC_x,&SC_s);
        std::cerr  << "  -Elapsed for running the simulation: " << x.count()/1000/1000. << " msec CLOCK time" << "//" << sc_time_String_Get(SC_TIME_UNIT_DEFAULT,SC_s) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT] << " SIMULATED time" << endl;
    }
    else
        std::cerr  << "  -Error " << returnValue << " during preparing objects" << endl;

     std::cerr  << "<<< Exiting " << PROJECT_NAME << "_DEMO/CLI V" << PROJECT_VERSION;
    if(returnValue)
        std::cerr << ", error code " << returnValue << endl;
    else
        std::cerr << " with no error"  << endl;
    std::cerr  << "GenComp/DEMO simulation " << s.count()/1000/1000. << " msec CLOCK time" << "//" << sc_time_String_Get(SC_TIME_UNIT_DEFAULT,SC_s) << " " << SC_TIME_UNIT[SC_TIME_UNIT_DEFAULT] << " SIMULATED time" << endl;
    return(returnValue);
}

