// The configured options and settings for GenComp SystemC developments
// Will be used to configure modules/include/HWConfig.h
/** @file HWConfig.h
 *  @brief Basic hardware configuration information for all GenCompC packages
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#include <assert.h>     /* assert */
#include <stdint.h>   // The standard 'int' types
#include <iomanip>      // std::setfill, std::setw
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>
#include <systemc>    // For sc_time at least
using namespace sc_core; using namespace sc_dt;
using namespace std;
// A safety timeout limit for clock cycles
#define MAX_CLOCK_CYCLES  10000

// Define string for the settings
#define MY_COMPANY "vjSoft"
#define MY_DOMAIN "https://githubt.org/jvegh/generalcomputingmodel/"

/// Not using clock explicitly makes simulation faster and more simple
//#define MUST_USE_CLOCK_SIGNAL

// Hardware configuration
// The grid points are arranged in a grid topology
// Define the topology: !! Placing algorithm must be prepared
// Presently only 10x6 grid can be selected!
#define GRID_SIZE_X 10
#define GRID_SIZE_Y 6
// The number of wires for transmitting grid number
#define GRID_BUS_WIDTH 6
#define GRIDPOINT_MASK_WIDTH (1 << GRID_BUS_WIDTH)
#define GRID_ID_TYPE uint32_t
// The maximum available number of cores
#define MAX_GRIDPOINTS_LIMIT (1 << GRID_BUS_WIDTH)
#define MAX_GRIDPOINTS GRID_SIZE_X*GRID_SIZE_Y
// The actual number of gridpoints
#define NUMBER_OF_GRIDPOINTS MAX_GRIDPOINTS

// The gridpoints are organized into clusters
#define CLUSTER_BUS_WIDTH 4       ///< Cluster bus width
#define MAX_CLUSTERS_LIMIT (1 << CLUSTER_BUS_WIDTH)-1
// The maximum available number of clusters
#define CLUSTERS_LIMIT 8

#define TOPOLOGY_BUS_WIDTH 4
#define MAX_TOPOLOGIES_LIMIT (1 << TOPOLOGY_BUS_WIDTH)-1
#define CARD_BUS_WIDTH 4
#define MAX_CARDS_LIMIT (1 << CARD_BUS_WIDTH)-1
#define RACKS_BUS_WIDTH 4
#define MAX_RACKS_LIMIT (1 << RACKS_BUS_WIDTH)-1

// The computing units in gridpoints handle several HTreads
// (the HW gets frequently blocked by memory or I/O, or missing condition)
#define HTHREAD_BUS_WIDTH 4
#define MAX_HTHREADS (1 << HTHREAD_BUS_WIDTH)
#define MAX_HTHREADS_LIMIT (MAX_HTHREADS-1)
// The maximum available number of HThreads : only prepared, but not really used
// Define memory features
// We may have 'register' memory, type 0
#define RMEMORY_ADDRESS_WIDTH 4
// The maximum size of the simulated memory; must be 2**N
#define RMAX_MEMORY_SIZE (1 << RMEMORY_ADDRESS_WIDTH)

// We may have 'dynamic' memory, type 1
#define DMEMORY_ADDRESS_WIDTH 10
// The maximum size of the simulated memory; must be 2**N
#define DMAX_MEMORY_SIZE (1 << DMEMORY_ADDRESS_WIDTH)
#define DMEMORY_READ_TIME sc_core::sc_time(10,SC_NS)

// We may have 'buffer' memory, type 2
#define BMEMORY_ADDRESS_WIDTH 10
// The maximum size of the simulated memory; must be 2**N
#define BMAX_MEMORY_SIZE (1 << BMEMORY_ADDRESS_WIDTH)
#define BMEMORY_READ_TIME sc_core::sc_time(20,SC_NS)

// We may have 'far' memory, type 3
#define FMEMORY_ADDRESS_WIDTH 16
// The maximum size of the simulated memory; must be 2**N
#define FMAX_MEMORY_SIZE (1 << FMEMORY_ADDRESS_WIDTH)
#define FMEMORY_READ_TIME sc_core::sc_time(60,SC_NS)


// The word size of the registers and memories
#define WORD_SIZE 32

#define  CORE_DEFAULT_ADDRESS (SC_ADDRESS_TYPE)-1
#define  MEMORY_DEFAULT_ADDRESS  (SC_ADDRESS_TYPE)-1

// if true, the prolog will use SystemC module name as reference
#define USE_MODULE_NAMES false

// Define size of message buffer for inter-gridpoint communication
#define MAX_IGPCBUFFER_SIZE 16

// Operational characteristics

// Resolution of time  and clock frequency
#define SCTIME_RESOLUTION 10,sc_core::SC_PS
// Gates operating time
#define SCTIME_GATE sc_time(20,sc_core::SC_PS)
// Frequency of the system clock
#define SCTIME_CLOCKTIME sc_time(100,sc_core::SC_PS)
// The biology subsystem uses a kind of clock signal for its internal operation
#define SCBIOLOGY_CLOCKTIME sc_time(10,sc_core::SC_US)

#define USE_DEBUG_DATA_TYPES

#ifdef USE_DEBUG_DATA_TYPES
// Use normal C++ variables, to make debugging easier
    typedef uint8_t  SC_HTHREAD_ID_TYPE;
    typedef uint32_t SC_ADDRESS_TYPE;
    typedef uint32_t SC_GRIDPOINT_ID_TYPE;
    typedef uint32_t SC_WORD_TYPE;
    typedef uint32_t SC_REGISTER_MASK_TYPE;
    typedef uint64_t SC_GRIDPOINT_MASK_TYPE;
    typedef uint64_t SC_HTHREAD_MASK_TYPE;

#else
    typedef sc_dt::sc_uint<HTHREAD_BUS_WIDTH>  SC_HTHREAD_ID_TYPE;
    typedef sc_dt::sc_uint<MEMORY_ADDRESS_WIDTH> SC_ADDRESS_TYPE;
    typedef sc_dt::sc_uint<CORE_BUS_WIDTH> GRID_BUS_WIDTH;
    typedef sc_dt::sc_uint<WORD_SIZE> SC_WORD_TYPE;
    typedef sc_dt::sc_uint<MAX_GRIDPOINTS>  SC_REGISTER_MASK_TYPE;
    typedef sc_dt::sc_uint<MAX_GRIDPOINTS>  SC_GRIDPOINT_MASK_TYPE;
    typedef sc_dt::sc_uint<MAX_HTHREADS>  SC_HTHREAD_MASK_TYPE;
#endif //


// These defines that the modules shall test performance
#define MEASURE_DATA_TYPES_USING true
#define MEASURE_USING_MEMORY_TYPES_USING true
#define PREPARE_PERFORMACE_PLOT true
// These defined some internal operation options
#define MAKE_PARALLEL_PREFETCH true

// These defines measures the memory
#define MEASURE_NETWORK_TRANSFERS true
#define MEASURE_DIRECT_TRANSFERS true
#define MEASURE_PROXY_TRANSFERS true
#define MEMORY_TRANSFER true
#define MEMORY_ACCESS_TIME true
#define MEMORY_TOTAL_ACCESS_TIME true

// Define if to measure memory separately
#define MEASURE_MEMORY0_TRANSFERS true
#define MEASURE_MEMORY1_TRANSFERS true
#define MEASURE_MEMORY2_TRANSFERS true
#define MEASURE_MEMORY3_TRANSFERS true

// Define if to make benhmarking time measurements
#define MAKE_BENCHMARKING true

