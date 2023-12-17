/** @file Stuff.h
 *  @ingroup GENCOMP_MODULE_STUFF
 *  @brief Function prototypes of useful stuff for the simulator
 *
 */
 /*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef GENCOMP_STUFF_H
#define GENCOMP_STUFF_H
//#include "Config.h"
#include <string>
extern bool GUI_MODE;	// This tells whether routine run in GUI mode
using namespace std;


/*!
   * \brief CheckArgumentList
   * Check the parameters of the starting command line
   * \param[in] S String at the beginning
   * \param argc Number of command line parameters as in main()
   * \param argv list of char* of the parameters as in main()
   * \returns the resulting message
   */
string
CheckArgumentList(string S, int argc, char** argv);


void
OpenSystemFiles(const char* FileName, const char* LogFileName, string Heading);

void CloseSystemFiles(string Trailing);

/**
 *  @brief  Assemble the root part of the filename used for different goals
 *
 *  @param[in]  DataFileName   Name of the simulator command file
 */
  string
GetFileNameRoot(string DataFileName="");

/**
 *  @brief  Return application name and version
 */
  std::string
GetAppName(void);

/*
 *  @brief  Return application name and version
 * /
  std::string
GetAppVersion(void);
*/

bool FinishProgram(void);

#endif //  GENCOMP_STUFF_H
