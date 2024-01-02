/** @file scMacroTimeBenchmarking.h
 *  @ingroup GENCOMP_MODULE_PROCESS
 *  @brief Macros for SystemC  (simulated) execution time (simulated) handling in benchmarks
 */
/*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
/*!
@verbatim
    This macro group defines the benchmarking macros for making simulated time measurements.
    The file "scMacroTimeBenchmarking.h" shall be included in the header of the module
    where the benchmark is to be used.
    If the module includes this header, it must be preceded by defining

    If the module includes this header, and it is preceded by
    '\#define SC_MAKE_TIME_BENCHMARKING',
    the results of the time measurements will be returned in the user-defined variables.

    If this macro is not defined, no code is generated.
    The variables, however, must be defined (although they will be
  optimized out as unused ones).
  Alternatively, the macros may be protected with
  '\#ifdef SC_MAKE_TIME_BENCHMARKING'.
  The macros have source-module scope. All variables must be passed by reference.



    can be accessed as

    sc_time_String_Get(SC_TIME_UNIT_DEFAULT,SC_x)

    in nanoseconds, as int64_t values.

    That is: in your module, between the '\#include' files write

       @code{.cpp}
\#define SC_MAKE_TIME_BENCHMARKING  // comment out if you do not want to benchmark
\#include "scMacroTimeBenchmarking.h"    // Must be after the define to have its effect
    @endcode


    In your class (or member function) define

    @code{.cpp}
    sc_core::sc_time SC_t, SC_x, SC_s;
    @endcode

    Before starting using the macros, you shall initialize them:
    @code{.cpp}
    SC_BENCHMARK_TIME_RESET(&SC_t,&SC_x,&SC_s); // Reset at the very begining, say in the constructor
    @endcode
    and use the macros as as

and later put the benchmarked code between macros, used as brackets
    @code{.cpp}
    SC_BENCHMARK_TIME_BEGIN(&SC_t,&SC_x);    // Begin the benchmarking here
    SC_BENCHMARK_TIME_END(&SC_t,&SC_x,&SC_s);   // End benchmarking here
    @endcode

    Later put the benchmarked code between macros, used as brackets


    The returned values are

    'sc_core::sc_time x',
    Now you have the benchmarked time (since BEGIN) in SC_x (nanoseconds)
    and the sum of all benchmarked time (since RESET) in SC_s (nanoseconds)
    For example, in an object, you can RESET in the constructor,
    in the member functions between BEGIN and END measure the
    one-time utilization, and in the destructor to read out the total benchmarked time.

    std::cerr << std::fixed << std::setfill (' ') << std::setprecision(d) << std::setw(w)
              << SC_x.to_seconds()*1000.;

    The offset due to benchmarking is approx 55-85 nanosecs

    As the arguments are user-provided, with consistent use several independent
    measurement can be carried out within the source scope.

    Notice: this is SIMULATED time, returned by SystemC engine; nor CLOCK time
@endverbatim
 */

#ifdef MAKE_SCTIME_BENCHMARKING
#include <systemc>

/*!
  \def BENCHMARK_TIME_END(t,x,s)
  Changes the user-provided  \a t timepoint.
  Sets the user-provided \a x to the duration since the last TIME_BEGIN \a t value.
  Sets the user-provided \a s to the duration since the last TIME_RESET \a t value.
  ('t' is a don't care, BUT DON'T CHANGE, value).
  // Variables must be passed by reference
*/
#define SC_BENCHMARK_TIME_END(t,x,s)\
    *x = sc_time_stamp() - *t; \
    *t = sc_time_stamp(); *s += *x;

#define SC_BENCHMARK_TIME_RESET(t,x,s)\
    SC_BENCHMARK_TIME_BEGIN(t,x); *s=*x;
/*!
  \def SC_BENCHMARK_TIME_BEGIN(t,x)
  Changes the user-provided  \a t timepoint
  Sets the user-provided \a x to the last TIME_BEGIN \a t value
*/
#define SC_BENCHMARK_TIME_BEGIN(t,x)\
    *t = sc_time_stamp(); *x=sc_time(SC_ZERO_TIME);
#else // The time measurement not needed, do nothing
// The macros with empty functionality will be optimized out by the compiler
#define SC_BENCHMARK_TIME_RESET(t,x,s)
#define SC_BENCHMARK_TIME_BEGIN(x,t)
#define SC_BENCHMARK_TIME_END(t,x,s)
#endif //SC_MAKE_TIME_BENCHMARKING
#undef SC_MAKE_TIME_BENCHMARKING // Make macro definition file-scope wide
