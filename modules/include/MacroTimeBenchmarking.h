/*! @file MacroTimeBenchmarking.h

 *  Macros for execution (clock) time handling in benchmarks
 */
/*
 *  @author János Végh (jvegh)
 *  @ingroup GENCOMP_MODULE_STUFF
 *  @bug No known bugs.
 */

/*!
@verbatim
    This module defines the benchmarking macros for making computing (clock) time measurements.
    This file shall be included in the header of the module in question.
    If the module includes this header, it must be preceded by defining

    MAKE_TIME_BENCHMARKING

    If it is not defined, no code is generated. The variables, however,
    must be defined. They will be optimized out as unused ones,
    or they may be protected with

    "\#ifdef MAKE_TIME_BENCHMARKING"

    The returned values are

    'std::chrono::duration<int64_t, nano> x',

    can be accessed as

    x.count()

    in nanoseconds, as int64_t values.


    That is: in your module, between the '\#include' files write

\#define MAKE_TIME_BENCHMARKING  // comment out if you do not want to benchmark
\#include "MacroTimeBenchmarking.h"    // Must be after the define to have its effect

    In your class (or member function) define

    chrono::steady_clock::time_point t;
    std::chrono::duration<int64_t, nano> x,s=(std::chrono::duration< int64_t, nano>)0;

    and use the macros as as

    BENCHMARK_TIME_RESET(&t,&x,&s); // Reset at the very begining, say in the constructor

Later put the benchmarked code between macros, used as brackets

    BENCHMARK_TIME_BEGIN(&t,&x);    // Begin the benchmarking here
    BENCHMARK_TIME_END(&t,&x,&s);   // End benchmarking here

    Now you have the benchmarked time (since BEGIN) in x (nanoseconds)
    and the sum of all benchmarked time (since RESET) in s (nanoseconds)

    I.e. use it like
    std::cerr  << "Simulation took " << s.count()/1000/1000. << " msec CLOCK time" << endl;

    Notice: this is CLOCK time, returned by the processor; not SIMULATED time
@endverbatim
 */


  \def BENCHMARK_TIME_RESET(t,x,s)
@verbatim
  Clears the user-provided \a x and \a s duration values
  Initializes the user-provided  \a t timepoint

    This macro group defines the benchmarking macros for making time measurements.
    The file "MacroTimeBenchmarking.h" shall be included in the header of the module
    where the benchmark is to be used.
    If the module includes this header, and it is preceded by
    '\#define MAKE_TIME_BENCHMARKING',
    the results of the time measurements will be returned in the user-defined variables.
    If this macro is not defined, no code is generated;
  the variables, however, must be defined (although they will be
  optimized out as unused ones).
  Alternatively, the macros may be protected with "\#ifdef MAKE_TIME_BENCHMARKING".
  The macros have source-module scope. All variables must be passed by reference.

  The returned values are of type 'std::chrono::duration< int64_t, nano> x',
  their value can be accessed as 'x.count()' in nanoseconds, as int64_t values.
  ('t' is a don't care, BUT DON'T CHANGE, value; contains internal tick values).

  Usage:
    In your class (or member function) define

    That is: in your module, between the '\#include' files write
    @code{.cpp}
#define MAKE_TIME_BENCHMARKING  // comment out if you do not want to benchmark
#include "MacroTimeBenchmarking.h"    // Must be after the define to have its effect
    @endcode
    You should also define variables (such as)

    @code{.cpp}
    chrono::steady_clock::time_point t =chrono::steady_clock::now();
    std::chrono::duration< int64_t, nano> x,s=(std::chrono::duration< int64_t, nano>)0;
    @endcode

    Before starting using the macros, you shall initialize them:
    @code{.cpp}
    BENCHMARK_TIME_RESET(&t,&x,&s); // Reset at the very begining, say in the constructor
    @endcode
and later put the benchmarked code between macros, used as brackets
    @code{.cpp}
    BENCHMARK_TIME_BEGIN(&t,&x);    // Begin the benchmarking here
    BENCHMARK_TIME_END(&t,&x,&s);   // End benchmarking here
    @endcode

    After using these macros, the benchmarked time are returned:
    (since BEGIN) in x (nanoseconds)
    and the sum of all benchmarked time (since RESET) in s (nanoseconds).
    For example, in an object, you can RESET in the constructor,
    in the member functions between BEGIN and END measure the
    one-time utilization, and in the destructor to read out the total benchmarked time.

    The offset due to benchmarking is approx 55-85 nanosecs

    As the arguments are user-provided, with consistent use several independent
    measurement can be carried out within the source scope.
@endverbatim

    @see BENCHMARK_TIME_RESET
    @see BENCHMARK_TIME_BEGIN
    @see BENCHMARK_TIME_END
*/


/*!
  \def BENCHMARK_TIME_BEGIN(t,x)
  Changes the user-provided  \a t timepoint
  Sets the user-provided \a x to the last TIME_BEGIN \a t value
*/

/*!
  \def BENCHMARK_TIME_END(t,x,s)
  Changes the user-provided  \a t timepoint.
  Sets the user-provided \a x to the duration since the last TIME_BEGIN \a t value.
  Sets the user-provided \a s to the duration since the last TIME_RESET \a t value.
*/

#ifdef MAKE_TIME_BENCHMARKING
#include <ctime>
#include <ratio>
#include <chrono>

// Return the time since the last call in x, and sums the elapsed time in s
#define BENCHMARK_TIME_END(t,x,s)\
    *x = std::chrono::duration< int64_t, nano>(std::chrono::steady_clock::now() - *t); \
    *t = std::chrono::steady_clock::now(); *s += *x;
#define BENCHMARK_TIME_RESET(t,x,s)\
    BENCHMARK_TIME_BEGIN(t,x); *s=*x;
#define BENCHMARK_TIME_BEGIN(t,x)\
    *t = std::chrono::steady_clock::now(); *x=*t-*t;
#else // The time measurement not needed, do nothing
// The macros with empty functionality
#define BENCHMARK_TIME_RESET(t,x,s)
#define BENCHMARK_TIME_BEGIN(x,t)
#define BENCHMARK_TIME_END(t,x,s)
#endif //MAKE_TIME_BENCHMARKING
#undef MAKE_TIME_BENCHMARKING // Make macro definition file-scope wide

