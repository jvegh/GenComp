#ifndef INITGTESTDEVEL_simple_H
#define INITGTESTDEVEL_simple_H

#include "sctestbench_simple.h"

extern GenCompDEVEL_simpleTB_t* GenCompDEVEL_simpleTB;

/**
 * This module must generate all DUT modules, and connect their signals properly
*/
SC_MODULE(GTestModule_simple)
{
  // Constructor
  SC_HAS_PROCESS(GTestModule_simple);
  GTestModule_simple(sc_module_name nm)
  : sc_module(nm)
  {
     GenCompDEVEL_simpleTB = new GenCompDEVEL_simpleTB_t("GenCompDEVEL_simple");
  }
protected:
};

#endif // INITGTESTDEVEL_simple_H
