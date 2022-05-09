//////////////////////////////////////////////////////////////////////
// StirMark Benchmark - StirMarkBenchmark.cpp
//
// Contents: Main SMB program.
//
// Purpose:  This is the main StirMark Benchmark program.
//           More information about its purpose is available from:
//           www.cl.cam.ac.uk/~fapp2/watermarking/stirmark/benchmark/
//
// Created:  Nazim Fat�s, Microsoft Research, May 2000
//
// Modified: Fabien A. P. Petitcolas, Microsoft Research
//
// History:
//
// Copyright (c) 2000-2002, Microsoft Research Ltd , Institut National
// de Recherche en Informatique et Automatique (INRIA), Institut Eur�com
// and the Integrated Publication and Information Systems Institute at
// GMD - Forschungszentrum Informationstechnik GmbH (GMD-IPSI).
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted for non-commercial research and academic
// use only, provided that the following conditions are met:
// 
// - Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer. Each
//   individual file must retain its own copyright notice.
// 
// - Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions, the following disclaimer and the
//   list of contributors in the documentation and/or other materials
//   provided with the distribution.
// 
// - Modification of the program or portion of it is allowed provided
//   that the modified files carry prominent notices stating where and
//   when they have been changed. If you do modify this program you
//   should send to the contributors a general description of the changes
//   and send them a copy of your changes at their request. By sending
//   any changes to this program to the contributors, you are granting a
//   license on such changes under the same terms and conditions as
//   provided in this license agreement. However, the contributors are
//   under no obligation to accept your changes.
// 
// - All non-commercial advertising materials mentioning features or use
//   of this software must display the following acknowledgement:
// 
//     This product includes software developed by Microsoft Research
//     Ltd, Institut National de Recherche en Informatique et Automatique
//     (INRIA), Institut Eur�com and the Integrated Publication and
//     Information Systems Institute at GMD - Forschungszentrum
//     Informationstechnik GmbH (GMD-IPSI).
// 
// - Neither name of Microsoft Research Ltd, INRIA, Eur�com and GMD-IPSI
//   nor the names of their contributors may be used to endorse or
//   promote products derived from this software without specific prior
//   written permission.
// 
// - If you use StirMark Benchmark for your research, please cite:
// 
//     Fabien A. P. Petitcolas, Martin Steinebach, Fr�d�ric Raynal, Jana
//     Dittmann, Caroline Fontaine, Nazim Fat�s. A public automated
//     web-based evaluation service for watermarking schemes: StirMark
//     Benchmark. In Ping Wah Wong and Edward J. Delp, editors,
//     proceedings of electronic imaging, security and watermarking of
//     multimedia contents III, vol. 4314, San Jose, California, U.S.A.,
//     20-26 January 2001. The Society for imaging science and
//     technology (I.S.&T.) and the international Society for optical
//     engineering (SPIE). ISSN 0277-786X.
// 
// and
// 
//     Fabien A. P. Petitcolas. Watermarking schemes
//     evaluation. I.E.E.E. Signal Processing, vol. 17, no. 5,
//     pp. 58-64, September 2000.
// 
// THIS SOFTWARE IS NOT INTENDED FOR ANY COMMERCIAL APPLICATION AND IS
// PROVIDED BY MICROSOFT RESEARCH LTD, INRIA, EUR�COM, GMD-IPSI AND
// CONTRIBUTORS 'AS IS', WITH ALL FAULTS AND ANY EXPRESS OR IMPLIED
// REPRESENTATIONS OR WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE, TITLE OR NONINFRINGEMENT OF INTELLECTUAL
// PROPERTY ARE DISCLAIMED. IN NO EVENT SHALL MICROSOFT RESEARCH LTD,
// INRIA, EUR�COM, GMD-IPSI OR THEIR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// THE USE OF THIS SOFTWARE FOR CIRCUMVENTING WITHOUT AUTHORITY ANY
// EFFECTIVE TECHNOLOGICAL MEASURES DESIGNED TO PROTECT ANY COPYRIGHTS OR
// ANY RIGHTS RELATED TO COPYRIGHT AS PROVIDED BY LAW OR THE SUI GENERIS
// RIGHT PROVIDED BY SOME COUNTRIES IS STRICTLY PROHIBITED.
//
// $Header: /home/cvs/StirmarkBench/StirMark_Bench/Base/StirMarkBenchmark.cpp,v 1.20 2002/04/19 10:23:55 petitcolas Exp $
//////////////////////////////////////////////////////////////////////



// STL headers
// DO NOT USE standard iostream headers (not compatible)
// See: http://www.sgi.com/Technology/STL/
#include <iostream>
#if _MSC_VER > 1020  // if later than VC++ 4.2
using namespace std; // the std C++ libs are in namespace std
#endif

// Other standard headers
#include <stdio.h>
// #define CHANGE_WORKING_FOLDER
// Used when profiling as the profiler does not allow to set
// a working folder
#ifdef CHANGE_WORKING_FOLDER
#include <direct.h>
#endif // CHANGE_WORKING_FOLDER
#ifdef WIN32
// Memory leak detection
// Technical article: Edward Wright, "Detecting and Isolating Memory Leaks
// Using Microsoft Visual C++", MSDN, Microsoft Corporation, May 1999
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // WIN32

// SMB headers
#include "SMBBench.h"
#include "SMBTools.h"
#include "SMBConfig.h"
#include "SMBException.h"
#include "SMBError.h"



//////////////////////////////////////////////////////////////////////
// Usage function
static void _Usage(TString strProgramName)
{
#ifdef WIN32
  cout << "Usage: StirMarkBenchmark [[-? | /?] | [INIFILE.ini [LIBRARY.dll [LOGFILE.log]]]]"
       << endl;
#endif
#ifdef __UNIX__
  cout << "Usage: StirMarkBenchmark [[-? | /?] | [INIFILE.ini [LIBRARY.so [LOGFILE.log]]]]"
       << endl;
#endif

  cout << "Default: "
       << strProgramName << " "
       << SMB_DEFAULT_LIBRARY_FILE << " "
       << SMB_DEFAULT_SETTINGS_FILE << " "
       << SMB_DEFAULT_REPORT_FILE << " "
       << endl;
  exit(1);
}

//////////////////////////////////////////////////////////////////////
// intialisation of the Paths to the Lib/INI/LOG files 
// with the command line arguments
void SMBReadArguments(STR      *in_pszArg,
                      int       in_nszArg,
                      TString &out_strLibFilePath,
                      TString &out_strIniFilePath,
                      TString &out_strLogFilePath)
{
  switch (in_nszArg)
  {
  case 4:
    out_strLogFilePath = in_pszArg[3];
    out_strLibFilePath.AddExtension(".log");
    // Fall through

  case 3:
    out_strLibFilePath = in_pszArg[2];
#ifdef WIN32
    out_strLibFilePath.AddExtension(".dll");
#endif
#ifdef __UNIX__
    out_strLibFilePath.AddExtension(".so");
#endif
    // Fall through

  case 2:
    if ((in_pszArg[1][0] == '/' || in_pszArg[1][0] == '-') && 
        (in_pszArg[1][1] == '?'))
        _Usage(in_pszArg[0]);
    else
    {
      out_strIniFilePath = in_pszArg[1];
      out_strIniFilePath.AddExtension(".ini");
    }
    break;

  default:
    ; // Use default parameters
  }
}


//////////////////////////////////////////////////////////////////////
// StirMark Benchmark entry point
//////////////////////////////////////////////////////////////////////
int main(int nszArg, char **pszArg)
{
#ifdef WIN32
  // Dump memory leaks whenever the program exists
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF);
#endif

#ifdef CHANGE_WORKING_FOLDER
  _chdir("C:\\Local documents\\CVS Root\\StirmarkBench\\StirMark_Bench\\Bin\\Benchmark");
#endif

  TString strLogFilePath(SMB_DEFAULT_REPORT_FILE);
  TString strIniFilePath(SMB_DEFAULT_SETTINGS_FILE);
  TString strLibFilePath(SMB_DEFAULT_LIBRARY_FILE);

  // Get the path to log and ini file file
  SMBReadArguments(pszArg, nszArg, strLibFilePath, strIniFilePath, strLogFilePath);

  // Intro
  TString strUTC;
  SMBGetTimeDate(strUTC);
  
  SMBInformation("\n\n------------------------------", 0);
  SMBInformation(TString("  StirMark BenchMark ") + SMB_VERSION, 0);
  SMBInformation("------------------------------", 0);

  SMBInformation(TString("\nStarting Markbench test - ") + strUTC, 0);
      
  SMBInformation("\n  PATHS >>", 0);
  SMBInformation(TString("    LIB: ") + strLibFilePath, 0);
  SMBInformation(TString("    INI: ") + strIniFilePath, 0);
  SMBInformation(TString("    LOG: ") + strLogFilePath + TString("<time>") + SMB_DEFAULT_REPORT_FILE_EXT, 0);

  // Paths are passed to the Bench object
  CSMBBench myBench(strLibFilePath, strIniFilePath, strLogFilePath);
  try
  {
    myBench();
  }
  catch (CSMBException *in_exception)
  {
    in_exception->PrintMsg();
    cout << "Program will exit!" << endl;
    return SMBFAILURE;
  }
   
  cout << "\n\n\nStirMark Benchmark evaluation is finished." << endl;
  return SMBSUCCESS;
}

