// Basically, the debug and log messages have two operating modes
///  @ingroup GENCOMP_MODULE_PROCESS

// During unit testing, all log messages are suppressed
//#include "Utils.h"
//#include <iostream>
#include <iomanip>      // std::setfill, std::setw

// Do not produce log messages during unit testing or if explicitly suppressed
#ifdef SUPPRESS_LOGGING
//  #define LOG_FATAL(x)
  #define LOG_CRITICAL(x)
  #define LOG_WARNING(x)
  #define LOG_INFO(x)
  #define LOG_INFO_SC(x)
  #define LOG_ONLY(x)

#else // Logging is not suppressed
  #include <QDebug>
using namespace std;
  #define IF_TO_LOG if(!UNIT_TESTING)
  // Now use id to make logging
//  #define LOG_FATAL(x) IF_TO_LOG qFatal().noquote().nospace() << x
  #define LOG_CRITICAL(x) IF_TO_LOG qCritical().noquote().nospace() << x
  #define LOG_WARNING(x)  IF_TO_LOG qWarning().noquote().nospace() << x
  #define LOG_INFO(x)     IF_TO_LOG qInfo().noquote().nospace() << "| " << x
  #define LOG_INFO_OBJECT(x)     IF_TO_LOG qInfo().noquote().nospace() << PrologString_Get().c_str() << "| " << x
  #define LOG_INFO_SC(x)  IF_TO_LOG qInfo().noquote().nospace() << "@" << sc_time_to_nsec_Get().c_str() << "|" << name() << " " << x
  #define LOG_ONLY(x) x
#endif

#ifdef MAKE_SIGNAL_TRACING // Generating signal trace only if requested
/*#define LOG_TRACE_SIGNAL(TX,VALUE) \
    if(!UNIT_TESTING){std::cerr << PrologString_Get(PC_Get()).c_str() <<  std::string(TX).c_str() << "^"
                    std::cerr << VALUE ? "/" : "\\"; std::cerr << VALUE;\
                                 if(VALUE) std::cerr << "(0x" << hex << VALUE << dec << ")";\
                     std::cerr << VALUE ? "\\" :  "/"; \
                      std::cerr << " @line " << LOG_LINENO(PC_Get()) << ":\n" \
                                << "     " << LOG_SOURCELINE(PC_Get()) << "\n";}
*/
#else
#define LOG_TRACE_SIGNAL(TX,VALUE)
#endif // MAKE_SIGNAL_TRACING


#ifdef MAKE_LOG_TRACING // Generating trace only if requested
    #define LOG_LINENO(PC) \
        MainMemory->findLinenoToAddress(PC)
    #define LOG_SOURCELINE(PC) \
        MainMemory->getSourceLine((LOG_LINENO(PC)))
    #define LOG_TRACE(TX) \
        if(!UNIT_TESTING){std::cerr << '@' << StringOfTime_Get()  << ": " << PrologString_Get().c_str() << "\"" << std::string(TX).c_str() << "\"" << DEBUG_LOCATION << "\"" <<endl;}
    #define LOG_TRACE_SOURCE(TX) \
        if(!UNIT_TESTING){std::cerr << '@' << StringOfTime_Get()  << ": " << PrologString_Get().c_str() << "\"" << std::string(TX).c_str() << "\"" << "\"" "@line " << LOG_LINENO(PC_Get()) << ":\n" \
        << "      " << LOG_SOURCELINE(PC_Get()) << "\n";}
    #define LOG_TRACE_FETCH_SOURCE(TX) \
        if(!UNIT_TESTING){std::cerr << '@' << StringOfTime_Get()  << ": " << Prologstring_Get().c_str() << "\"" << std::string(TX).c_str() << "\"" "@line " << LOG_LINENO(fetchPC_Get()) << ":\n" \
        << "     " << LOG_SOURCELINE(fetchPC_Get()) << "\n";}
    #define LOG_TRACE_SOURCE_CONDITIONAL(B,TX) if ((B && !UNIT_TESTING)) {LOG_TRACE_SOURCE(TX);}
    #define LOG_VALUE(TX,VALUE) \
        if(!UNIT_TESTING){std::cerr << PrologString_Get().c_str() << " !! " << std::string(TX).c_str() << VALUE << "\\n";}
    #define LOG_TRACE_VALUE(TX,VALUE) \
        if(!UNIT_TESTING){std::cerr << '@' << StringOfTime_Get()  << ": " << PrologString_Get().c_str() << " !! " << std::string(TX).c_str() << VALUE;\
                          if(VALUE) std::cerr << "(0x" << hex << VALUE << dec << ")";\
                          std::cerr << " @line " << LOG_LINENO(PC_Get()) << ":\n" \
                                    << "     " << LOG_SOURCELINE(PC_Get()) << "\n";}
    #define LOG_HEXA(TX,VALUE) \
        if(!UNIT_TESTING){std::cerr << '@' << StringOfTime_Get()  << ": " << PrologString_Get().c_str() << " !! " << std::string(TX).c_str() << hex << "0x" << VALUE <<  dec << "(" << VALUE << ")\"\n";}
    #define LOG_TRACE_HEXA(TX,VALUE) \
        if(!UNIT_TESTING){std::cerr << '@' << StringOfTime_Get()  << ": " << " !! " << std::string(TX).c_str() << hex << "0x" << VALUE << dec << "\" @line " << LOG_LINENO(PC_Get()) << ":\n" \
                    << "     " << LOG_SOURCELINE(PC_Get()) << "\n";}
    #define LOG_TRACE_HEXA_CONDITIONAL(B,TX,VALUE) if ((B && !UNIT_TESTING)) LOG_TRACE_HEXA(TX,VALUE);
    #define LOG_TRACE_VALUE_CONDITIONAL(B,TX,VALUE) if ((B && !UNIT_TESTING)) LOG_TRACE_VALUE(TX,VALUE);
    #define LOG_HEXA_CONDITIONAL(B,TX,VALUE) if ((B && !UNIT_TESTING)) LOG_HEXA(TX,VALUE);
    #define LOG_VALUE_CONDITIONAL(B,TX,VALUE) if ((B && !UNIT_TESTING)) LOG_VALUE(TX,VALUE);
#else
    #define LOG_LINENO(PC)
    #define LOG_SOURCELINE(PC)
    #define LOG_TRACE(TX)
    #define LOG_TRACE_SOURCE(TX)
    #define LOG_TRACE_SOURCE_CONDITIONAL(B,TX)
    #define LOG_VALUE(TX,VALUE)
    #define LOG_HEXA(TX,VALUE)
    #define LOG_TRACE_VALUE(TX,VALUE)
    #define LOG_TRACE_HEXA(TX,VALUE)
    #define LOG_HEXA_CONDITIONAL(B,TX,VALUE)
    #define LOG_VALUE_CONDITIONAL(B,TX,VALUE)
#endif // MAKE_LOG_TRACING
#undef MAKE_LOG_TRACING

/*!
  \def DEBUG_EVENT_OBJECT(x)
  Prints object event \a x to the standard err output

  \def DEBUG_EVENT(x)
  Prints non-object event \a x to the standard err output

*/

// During unit testing, all event tracing messages are suppressed
#define DEBUG_LOCATION " //<" <<  string(__FILE__).substr(string(__FILE__).find_last_of("/") + 1) << ':'  << dec << __LINE__  
#ifdef DEBUG_EVENTS
//    #define DEBUG_EVENT_OBJECT(x)    if(!UNIT_TESTING)
//        std::cerr   << "EVT@"<< sc_time_to_nsec_Get().c_str() << ": " << PrologString_Get().c_str() << " " << x  << DEBUG_LOCATION << std::endl
    #define DEBUG_EVENT_PROC(x)    if(!UNIT_TESTING) \
    std::cerr   << "EVT_PRC@"<< sc_time_to_nsec_Get().c_str() << ": " << PrologString_Get().c_str() << " " << x  << DEBUG_LOCATION << std::endl
    #define DEBUG_EVENT_GRID(x)    if(!UNIT_TESTING) \
    std::cerr   << "EVT_GRD@"<< sc_time_to_nsec_Get().c_str() << ": " << PrologString_Get().c_str() << " " << x  << DEBUG_LOCATION << std::endl
    #define DEBUG_EVENT_THREAD(x)    if(!UNIT_TESTING) \
    std::cerr   << "EVT_THR@"<< sc_time_to_nsec_Get().c_str() << ": " << PrologString_Get().c_str() << " " << x  << DEBUG_LOCATION << std::endl
    #define DEBUG_EVENT_MEMORY(x)    if(!UNIT_TESTING) \
    std::cerr   << "EVT_MEM@"<< sc_time_to_nsec_Get().c_str() << ": " << PrologString_Get().c_str() << " " << x  << DEBUG_LOCATION << std::endl
    #define DEBUG_EVENT_SYNAPTIC(x)    if(!UNIT_TESTING) \
    std::cerr   << "EVT_SYN@"<< sc_time_to_nsec_Get().c_str() << ": " << PrologString_Get().c_str() << " " << x  << DEBUG_LOCATION << std::endl
//  #define DEBUG_FETCH_EVENT(x)    if(!UNIT_TESTING)
//        std::cerr  << "EVT@"<< sc_time_to_nsec_Get().c_str() << ": " << PrologString_Get().c_str() << " " << x << " //" << " <" << string(__FILE__).substr(string(__FILE__).find_last_of("/") + 1) << ':'  << dec << __LINE__  << std::endl
  #define DEBUG_EVENT(x) if(!UNIT_TESTING) \
        std::cerr  << "EVT@"<< sc_time_to_nsec_Get(2, 6, sc_time_stamp()).c_str() << ": "   <<  x << " :" << name() <<DEBUG_LOCATION  << std::endl
#else // DEBUG_EVENTS not printed
  #define DEBUG_EVENT(x)
//  #define DEBUG_EVENT_OBJECT(x)
    #define DEBUG_EVENT_PROC(x)
    #define DEBUG_EVENT_GRID(x)
    #define DEBUG_EVENT_THREAD(x)
    #define DEBUG_EVENT_MEMORY(x)
    #define DEBUG_EVENT_SYNAPTIC(x)
//    #define DEBUG_FETCH_EVENT(x)
    #define DEBUG_EVENT_SC(x)
#endif // DEBUG_EVENTS
#undef DEBUG_EVENTS

//if(!UNIT_TESTING)
#ifdef DEBUG_PRINTS
    #define DEBUG_ONLY(x) x;
    // Will print a basic message info in form
    // Register message: (7.N:NE)==>Msg:Reg(2,0x11)==>(8.SE) (payload length, mask)
    // Memory message: (7.N:NE)==>Msg:Mem(1,(0.H),0x100)==>(8.SE) (length, answer address, memory address)
    #define PRINT_MESSAGE(M) std::cerr << Processor->StringOfMessage_Get(M)  << "\n";

    // Print routing information: who sent, what message, to whom
//    #define PRINT_ROUTING(FROM, M, TO) std::cerr << Processor->StringOfRouting_Get(FROM, M, TO) << '\n';
        // Print the proxy GridPoint if used
//    #define PRINT_PROXY(C) std::cerr << Processor->StringOfProxy _Get(C);
    //    if(C->mClusterAddress.Proxy) {std::cerr << ":" << ClusterMembers[C->mClusterAddress.Proxy].name;}
    // Print the cluster with cluster address; print a '!' if the cluster head has a phantom head
    #define PRINT_CLUSTER(C) std::cerr << Processor->StringOfCluster_Get(C);




    #define DEBUG_PRINT_MESSAGE(x,M) \
        {std::cerr << "DBG" << "PrologString_Get()" <<" " << x << Processor_Get()->StringOfMessage_Get(M) << DEBUG_LOCATION  << std::endl;}
    #define DEBUG_PRINT_WITH_SOURCE(TX) \
        std::cerr << "DBG" << PrologString_Get().c_str() << "\"" << std::string(TX).c_str() << "\" @line " << LOG_LINENO(PC_Get()) << DEBUG_LOCATION << "\n" \
        << "     " << LOG_SOURCELINE(PC_Get()) << std::endl
    #define DEBUG_PRINT_OBJECT(x)  if(!UNIT_TESTING) \
        {std::cerr  << "DBG" << PrologString_Get() << " "  << x << DEBUG_LOCATION  << std::endl;}
    #define DEBUG_PRINT_IGP_MESSAGE(x)  if(!UNIT_TESTING) \
        {std::cerr  << "DBG" << " IGP MESSAGE " << " "  << x << DEBUG_LOCATION  << std::endl;}
    #define DEBUG_PRINT_SC(x)  if(!UNIT_TESTING) \
        {std::cerr  << "DBG@" << sc_time_to_nsec_Get().c_str() << sc_core::name() << ": " <<  x  << DEBUG_LOCATION  << std::endl;}
    #define DEBUG_PRINT(x)  if(!UNIT_TESTING) \
        {std::cerr  << "DBG@" << sc_time_to_nsec_Get().c_str() << " " <<  x  << DEBUG_LOCATION  << std::endl;}
    #define DEBUG_PRINT_IF_DIFFERENT(x,A,B)  if(!UNIT_TESTING) \
         {if(A!=B) std::cerr  << "DBG@" << sc_time_to_nsec_Get(2, 6, sc_time_stamp()).c_str() << "ns|" << name() << ":>> " <<  x  << DEBUG_LOCATION  << std::endl;}
    #define DEBUG_PRINT_OBJECT_IF_DIFFERENT(x,A,B)  if(!UNIT_TESTING) \
        {if(A!=B) std::cerr  << "DBG" << PrologString_Get() << " "  << x << DEBUG_LOCATION  << std::endl;}
#else // DEBUG_PRINTS not printed
    #define DEBUG_ONLY(x)
    #define PRINT_MESSAGE(M)
//    #define PRINT_ROUTING(FROM, M, TO)
//    #define PRINT_PROXY(C)

    #define DEBUG_PRINT_WITH_SOURCE(TX)
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINT_OBJECT(x)
    #define DEBUG_PRINT_MESSAGE(x,M)
    #define DEBUG_PRINT_IF_DIFFERENT(x,A,B)
#endif // DEBUG_PRINTS
#undef DEBUG_PRINTS


#ifdef USE_PERFORMANCE_DIAGRAM
    #define PLOT_FETCH_BALL(TIME,LENGTH) \
    Plot->PlotQTfetch(ID_Get(),atoi(sc_time_to_nsec_Get(TIME,0).c_str()),fetch.PC, atoi(sc_time_to_nsec_Get(LENGTH,0).c_str()))
/*    #define PLOT_FETCH_WAIT_BALL(TIME,LENGTH) \
        Plot->PlotQTfetchwait(ID_Get(),atoi(sc_time_to_nsec_Get(TIME,0).c_str()),fetch.PC, atoi(sc_time_to_nsec_Get(LENGTH,0).c_str()))
    #define PLOT_EXEC_BALL(TIME,LENGTH) \
        Plot->PlotQTexec(ID_Get(),atoi(sc_time_to_nsec_Get(TIME,0).c_str()),exec.PC, atoi(sc_time_to_nsec_Get(LENGTH,0).c_str()))
    #define PLOT_EXEC_WAIT_BALL(TIME,LENGTH) \
        Plot->PlotQTexec(ID_Get(),atoi(sc_time_to_nsec_Get(TIME,0).c_str()),exec.PC, atoi(sc_time_to_nsec_Get(LENGTH,0).c_str()))
    #define PLOT_META_BALL(TIME,LENGTH) \
        Plot->PlotQTmeta(ID_Get(),atoi(sc_time_to_nsec_Get(TIME,0).c_str()),exec.PC, atoi(sc_time_to_nsec_Get(LENGTH,0).c_str()))
    #define PLOT_ALLOCATON_BEGIN_SET(CORE) \
        CORE->AllocationBegin_Set(sc_time_stamp())
    #define PLOT_QT_FIGURE() \
        Plot->PlotQTfigure(QTID_Get(), sc_time_to_nsec_Get(sc_time_stamp()-AllocationBegin_Get()+sc_time_Get(1,SC_NS),0), ID_Get(), \
                                 sc_time_to_nsec_Get(AllocationBegin_Get(), 0), 0)
    #define PLOT_QT_REGISTER_MASK_IN(C,MASK) \
        Plot->PlotQTregisterMask(C, sc_time_to_nsec_Get(C->AllocationBegin_Get(),0), MASK, true)
    #define PLOT_QT_REGISTER_MASK_OUT(C,MASK) \
        Plot->PlotQTregisterMask(C, sc_time_to_nsec_Get(sc_time_stamp(),0), MASK, false)
    #ifdef DEBUG_PRINTS
        #define PLOT_QT_REGISTER_MASK_DEBUG(C,MASK,BOOL) \
        Plot->PlotQTregisterMask(C, sc_time_to_nsec_Get(sc_time_stamp(), 0), MASK, BOOL)
        #define PLOT_QT_MEMORY_DEBUG(C,A,V,BOOL) \
        Plot->PlotQTmemory(C, A, V, sc_time_to_nsec_Get(sc_time_stamp(), 0), BOOL)
    #else
        #define PLOT_QT_REGISTER_MASK_DEBUG(C,MASK,BOOL)
        #define PLOT_QT_MEMORY_DEBUG(C,A,V,BOOL)
    #endif
    #define PLOT_QT_WAIT(LENGTH) \
        Plot->PlotQTwait(ID_Get(),atoi(sc_time_to_nsec_Get(sc_time_stamp()-LENGTH,0).c_str()),exec.PC, atoi(sc_time_to_nsec_Get(LENGTH,0).c_str()))
#define DEBUG_PRINT(x)  if(!UNIT_TESTING) \
    {std::cerr  << "DBG@" << sc_time_to_nsec_Get(2, 6, sc_core::sc_time_stamp()).c_str() << " " <<  x  << DEBUG_LOCATION  << std::endl;}
#define DEBUG_PRINT_IF_DIFFERENT(x,A,B)  if(!UNIT_TESTING) \
     {if(A!=B) std::cerr  << "DBG@" << sc_time_to_nsec_Get(2, 6, sc_time_stamp()).c_str() << "ns|" << name() << ":>> " <<  x  << DEBUG_LOCATION  << std::endl;}
#define DEBUG_PRINT_OBJECT_IF_DIFFERENT(x,A,B)  if(!UNIT_TESTING) \
    {if(A!=B) std::cerr  << "DBG" << PrologString_Get() << " "  << x << DEBUG_LOCATION  << std::endl;}
*/
#else
#define PLOT_ALLOCATON_BEGIN_SET(CORE)
/*#define DEBUG_PRINT(x)  if(!UNIT_TESTING) \
    {std::cerr  << "DBG@" << sc_time_to_nsec_Get(2, 6, sc_core::sc_time_stamp()).c_str() << " " <<  x  << DEBUG_LOCATION  << std::endl;}
#define DEBUG_PRINT_IF_DIFFERENT(x,A,B)  if(!UNIT_TESTING) \
     {if(A!=B) std::cerr  << "DBG@" << sc_time_to_nsec_Get(2, 6, sc_time_stamp()).c_str() << "ns|" << name() << ":>> " <<  x  << DEBUG_LOCATION  << std::endl;}
#define DEBUG_PRINT_OBJECT_IF_DIFFERENT(x,A,B)  if(!UNIT_TESTING) \
    {if(A!=B) std::cerr  << "DBG" << PrologString_Get() << " "  << x << DEBUG_LOCATION  << std::endl;}
*/
#endif // MAKE_PERFORMANCE_DIAGRAM

#define WAIT_GATE {wait(SCTIME_GATE); }

// At the end, undefine all used macros
#undef DEBUG_PRINTS
