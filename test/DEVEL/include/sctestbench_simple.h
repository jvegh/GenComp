#ifndef TESTBENCH_H
#define TESTBENCH_H
#include <systemc>
using namespace sc_core;

SC_MODULE(GenCompDEVEL_simpleTB_t)
{
    // channels
//    sc_clock C1 {"clk", 100, SC_PS};
  // Constructor
public:
  SC_HAS_PROCESS(GenCompDEVEL_simpleTB_t);
    public:
  GenCompDEVEL_simpleTB_t( sc_module_name nm) : sc_module(nm)//,C1("C1")
  {
    SC_THREAD(test_thread);
  }

  int32_t ErrorCode_Get(){    return ErrorCode;}
  protected:
   void test_thread();
  int32_t ErrorCode;
};

#endif
