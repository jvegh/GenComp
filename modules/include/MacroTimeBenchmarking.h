/*! @file MacroTimeBenchmarking.h
 *  @brief Macros for (clock) execution time (simulated) handling in benchmarks
 *  @ingroup GENCOMP_MODULE_STUFF
 *
    These defined macros are the benchmarking macros for making computing (clock) time measurements.
    The "MacroTimeBenchmarking.h" file shall be included in the header of the module in question.
    The results of the time measurements will be returned in user-defined variables.
    As the arguments are user-provided, with consistent use several independent
    measurement can be carried out within the source scope. The macros have source-module scope.
    All variables must be passed by reference.

    Notice: this is CLOCK time, returned by the processor; not SIMULATED time.

    The operating time offset due to benchmarking is approx 55-85 nanosecs, per macro pairs.

<b>Usage:</b>
    If the module includes this header, it must be preceded by defining
    @code{.cpp}
#define MAKE_TIME_BENCHMARKING  // uncomment to measure the time with benchmarking macros
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
#define MAKE_TIME_BENCHMARKING  // comment out if you do not want to benchmark
#include "MacroTimeBenchmarking.h"    // Must be after the define to have its effect
    @endcode

     In your class (or member function) define
    @code{.cpp}
    chrono::steady_clock::time_point t =chrono::steady_clock::now();
    std::chrono::duration< int64_t, nano> x,s=(std::chrono::duration< int64_t, nano>)0;
    @endcode
    and use the macros as as
    @code{.cpp}
    BENCHMARK_TIME_RESET(&t,&x,&s); // Reset at the very begining, say in the constructor
    @endcode
    Later put the benchmarked code between macros, used as brackets
    @code{.cpp}
    BENCHMARK_TIME_BEGIN(&t,&x);    // Begin the benchmarking here
    BENCHMARK_TIME_END(&t,&x,&s);   // End benchmarking here
    @endcode
    After using these macros, the benchmarked time values are returned:
    (since BEGIN) in x (nanoseconds)
    and the sum of all benchmarked time (since RESET) in s (nanoseconds).
    Access the result as
    @code{.cpp}
    std::cerr  << "Simulation took " << s.count()/1000/1000. << " msec CLOCK time" << endl;
    @endcode

In an object, you can RESET in the constructor,
    in the member functions between BEGIN and END measure the
    one-time utilization, and in the destructor to read out the total benchmarked time.
 */
/*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */



/*!
  \def BENCHMARK_TIME_BEGIN(t,x)
  Changes the user-provided  \a t timepoint.
  Sets the user-provided \a x to the last TIME_BEGIN \a t value
  @param t for internal use, the absolute beginning of time  (do not touch)
  @param x deliver clock time of the beginning
*/

/*!
  \def BENCHMARK_TIME_END(t,x,s)
  Changes the user-provided  \a t timepoint.
  Sets the user-provided \a x to the duration since the last TIME_BEGIN \a t value.
  Sets the user-provided ime of the beginning\a s to the duration since the last TIME_RESET \a t value.
  @param t for internal use, the absolute beginning of time  (do not touch)
  @param x deliver clock duration since _BEGIN (only read)
  @param s deliver clock duration since _RESET (only read)
*/

/*!
  \def BENCHMARK_TIME_RESET(t,x,s)
  Clears the user-provided \a x and \a s duration values
  Sets the user-provided \a x to the duration since the last TIME_BEGIN \a t value.
  Sets the user-provided \a s to the duration since the last TIME_RESET \a t value.
  @param t for internal use, the absolute beginning of time (do not touch)
  @param x deliver clock duration since _BEGIN (only read)
  @param s deliver clock duration since _RESET (only read)
*/


#ifdef MAKE_TIME_BENCHMARKING
#include <ctime>
#include <ratio>
#include <chrono>

// Return the time since the last call TIME_BEGIN in x, and sums the elapsed time in s
#define BENCHMARK_TIME_END(t,x,s)\
    *x = std::chrono::duration< int64_t, nano>(std::chrono::steady_clock::now() - *t); \
    *t = std::chrono::steady_clock::now(); *s += *x;
// Initializes measurement summary
#define BENCHMARK_TIME_RESET(t,x,s)\
    BENCHMARK_TIME_BEGIN(t,x); *s=*x;
// Remembers beginning of time
#define BENCHMARK_TIME_BEGIN(t,x)\
    *t = std::chrono::steady_clock::now(); *x=*t-*t;
#else // The time measurement not needed, do nothing
// The macros with empty functionality
#define BENCHMARK_TIME_RESET(t,x,s)
#define BENCHMARK_TIME_BEGIN(x,t)
#define BENCHMARK_TIME_END(t,x,s)
#endif //MAKE_TIME_BENCHMARKING
#undef MAKE_TIME_BENCHMARKING // Make macro definition file-scope wide

