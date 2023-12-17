/** @file scMacroTimeBenchmarking.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief Macros for SystemC execution time handling in benchmarks
 */
 /*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
/*! This module defines the benchmarking macros for making time measurements.
    This file shall be included in the header of the module in question.
    If the module includes this header, it must be preceded by defining
    SC_MAKE_TIME_BENCHMARKING
    If it is not defined, no code is generated; the variables, however,
    must be defined. They will be optimized out as unused ones,
    or they may be protected with "\#ifdef SC_MAKE_TIME_BENCHMARKING"
    The returned values are 'sc_core::sc_time x',
    can be accessed as 'x.count()' in nanoseconds, as int64_t values.
@verbatim

    That is: in your module, between the '\#include' files write
\#define SC_MAKE_TIME_BENCHMARKING  // comment out if you do not want to benchmark
\#include "MacroScTimeBenchmarking.h"    // Must be after the define to have its effect

    In your class (or member function) define
    sc_core::sc_time t, x, s;
     and use the macros as as
    SC_BENCHMARK_TIME_RESET(&t,&x,&s); // Reset at the very begining, say in the constructor
and later put the benchmarked code between macros, used as brackets
    SC_BENCHMARK_TIME_BEGIN(&t,&x);    // Begin the benchmarking here
    SC_BENCHMARK_TIME_END(&t,&x,&s);   // End benchmarking here

    Now you have the benchmarked time (since BEGIN) in x (nanoseconds)
    and the sum of all benchmarked time (since RESET) in s (nanoseconds)
@endverbatim

 */
#ifdef MAKE_SCTIME_BENCHMARKING
#include <systemc>

// Return the time since the last call in x, and sums the elapsed time in s
// Both variables must be passed by reference
// ('t' is a don't care, BUT DON'T CHANGE, value).
#define SC_BENCHMARK_TIME_END(t,x,s)\
    *x = sc_time_stamp() - *t; \
    *t = sc_time_stamp(); *s += *x;
#define SC_BENCHMARK_TIME_RESET(t,x,s)\
    SC_BENCHMARK_TIME_BEGIN(t,x); *s=*x;
#define SC_BENCHMARK_TIME_BEGIN(t,x)\
    *t = sc_time_stamp(); *x=sc_time(SC_ZERO_TIME);
#else // The time measurement not needed, do nothing
// The macros with empty functionality will be optimized out by the compiler
#define SC_BENCHMARK_TIME_RESET(t,x,s)
#define SC_BENCHMARK_TIME_BEGIN(x,t)
#define SC_BENCHMARK_TIME_END(t,x,s)
#endif //SC_MAKE_TIME_BENCHMARKING
#undef SC_MAKE_TIME_BENCHMARKING // Make macro definition file-scope wide
