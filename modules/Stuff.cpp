/** @file Stuff.cpp
 *  @ingroup GENCOMP_MODULE_STUFF
 *  @brief Some utility routines for the time-aware computing simulator.
 */
/*  @author János Végh (jvegh)
 *  @bug No known bugs.
 */
#include <fstream>      // std::ifstream
#include <sstream>
//#include <gtest/gtest.h>
#include <stdlib.h>
#include "Stuff.h"
#include "Project.h"

extern bool UNIT_TESTING;
using namespace std;

bool GUI_MODE;	// This tells whether routines run in GUI mode

// Check the parameters of the starting command line
  string
CheckArgumentList(string S, int argc, char** argv)
{
  ostringstream  logintext;
  ostringstream StartString,NameString;
  S += " has been started as:\n'";
  for(int i = 0; i<argc; i++)
      S += argv[i] <+ " ";
  S += "'";
/*  if(argc < 3) // Sample extension
  {
    logintext  << " correct usage:\n" << argv[0] << " FileName.Eyo  NProc";
    cout << logintext.str() << endl; // Print out a kind of help, no logging yet
    exit (EXIT_FAILURE);
  }*/
  return S;
}

  /*!
 * \brief OpenSystemFiles
 * \param FileName The start-ut file name
 * \param LogFileName The real name of the logfile
 * \param Heading heading strint to th etop of the log file
 */

void
OpenSystemFiles(const char* FileName, const char* LogFileName, string Heading)
{
//  string FileRoot = GetFileNameRoot(FileName); // Prepare log file name
/*  FLAGS_alsologtostderr = 1; // Log also to the screen
  testing::internal::CaptureStdout();
  google::InstallFailureSignalHandler();
  google::SetLogDestination(0, (string(LogFileName)).c_str());
  google::InitGoogleLogging(FileName);
  LOG(INFO) << " " << Heading;
  LOG(INFO) << " Log file prefix for this run: \"" << LogFileName << "\"";
  LOG(INFO) << " <LOG message format: glog header] ><"  << GetAppName() << " message>";
*/
}

//http://stackoverflow.com/questions/3803465/how-to-capture-stdout-stderr-with-googletest

void CloseSystemFiles(string Trailing)
{
 /* std::string output = testing::internal::GetCapturedStdout(); // Write out the captured log info
  LOG(INFO) <<  endl << output;
  LOG(INFO) << " " << Trailing;
*/
}

  string
GetAppName(void)
{
  std::ostringstream appname;
  appname << PROJECT_NAME<< " V" << PROJECT_VERSION;
  return appname.str();
}


// Prepare a kind of file name fragment for created files
string GetFileNameRoot(string DataFileName)
{
  size_t fbegin = DataFileName.find_last_of('/');
  string FileNameRoot; 
  if(0==fbegin)
    FileNameRoot = DataFileName;
  else
    FileNameRoot = DataFileName.substr(fbegin+1); // Remove path first, if any
  fbegin = FileNameRoot.find_last_of('.');
  if(fbegin)
    FileNameRoot = FileNameRoot.substr(0,fbegin); // Remove extension, if any
  ostringstream FileNameFragment;
  FileNameFragment << GetAppName();
if(GUI_MODE)
    FileNameFragment << "_GUI";
else
    if(UNIT_TESTING)
      FileNameFragment << "_" << FileNameRoot << "_tst" ;
    else
      FileNameFragment  << "_txt_" << FileNameRoot;

  string S = FileNameFragment.str();
  size_t found = S.find(" "); // Replace " " with "-" in file name
  S.replace(found,1,"-");
  return S;
}

