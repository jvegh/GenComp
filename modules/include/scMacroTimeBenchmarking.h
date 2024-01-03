/** @file scMacroTimeBenchmarking.h
 *  @brief Macros for SystemC  (simulated) execution time (simulated) handling in benchmarks
 *  @ingroup GENCOMP_MODULE_STUFF
 *
    The "scMacroTimeBenchmarking.h" file shall be included in the header of the module in question.
    The results of the time measurements will be returned in user-defined variables.
    As the arguments are user-provided, with consistent use several independent
    measurement can be carried out within the source scope. The macros have source-module scope.
    All variables must be passed by reference.

    Notice: this is SIMULATED  time, returned by the SystemC engine; not CLOCK time returned by the processor.

    The operating time offset due to benchmarking is approx 55-85 nanosecs, per macro pairs.


<b>Usage:</b>
    If the module includes this header, it must be preceded by defining
    @code{.cpp}
#define SC_MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
    @endcode
   If this name is not defined, no code is generated for the module.
    The variables, however, must be defined (although they will be
    optimized out as unused ones).
    Alternatively, generating those variables may be protected with
    @code{.cpp}
#ifdef MAKE_TIME_BENCHMARKING
    @endcode

<b>Example:</b>
    in your module, between the '\#include' files write

    @code{.cpp}
#define SC_MAKE_TIME_BENCHMARKING  // comment out if you do not want to benchmark
#include "scMacroTimeBenchmarking.h"    // Must be after the define to have its effect
    @endcode

     In your class (or member function) define
    @code{.cpp}
    sc_core::sc_time SC_t, SC_x, SC_s;
    @endcode
    and use the macros as as
    @code{.cpp}
    SC_BENCHMARK_TIME_RESET(&SC_t,&SC_x,&SC_s); // Reset at the very begining, say in the constructor
    @endcode
    Later put the benchmarked code between macros, used as brackets
    @code{.cpp}
    SC_BENCHMARK_TIME_BEGIN(&SC_t,&SC_x);    // Begin the benchmarking here
    SC_BENCHMARK_TIME_END(&SC_t,&SC_x,&SC_s);   // End benchmarking here
    @endcode
    After using these macros, the benchmarked time value (since BEGIN) x is returned:
    and the sum of all benchmarked time (since RESET) in s (nanoseconds).
    Access the result as
    @code{.cpp}
    std::cerr << std::fixed << std::setfill (' ') << std::setprecision(d) << std::setw(w)
              << SC_x.to_seconds()*1000.;
    @endcode
    or, if confortable
    can be accessed as
    @code{.cpp}
    sc_time_String_Get(SC_TIME_UNIT_DEFAULT,SC_x)
    @endcode
 */
/*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

/*!
  \def SC_BENCHMARK_TIME_END(t,x,s)
  Changes the user-provided  \a t timepoint.
  Sets the user-provided \a x to the duration since the last TIME_BEGIN \a t value.
  Sets the user-provided \a s to the duration since the last TIME_RESET \a t value.
  ('t' is a don't care, BUT DON'T CHANGE, value).
  // Variables must be passed by reference
*/

/*!
  \def SC_BENCHMARK_TIME_BEGIN(t,x)
  Changes the user-provided  \a t timepoint
  Sets the user-provided \a x to the last TIME_BEGIN \a t value
*/

/*!
  \def SC_BENCHMARK_TIME_RESET(t,x,s)
  Clears the user-provided \a x and \a s duration values
  Sets the user-provided \a x to the duration since the last TIME_BEGIN \a t value.
  Sets the user-provided \a s to the duration since the last TIME_RESET \a t value.
  @param t for internal use, the absolute beginning of time (do not touch)
  @param x deliver clock duration since TIME_BEGIN (only read)
  @param s deliver clock duration since TIME_RESET (only read)
*/

#ifdef SC_MAKE_SCTIME_BENCHMARKING
#include <systemc>
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
