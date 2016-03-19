////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2015 - 2016, Paul Beckingham, Federico Hernandez.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <cmake.h>
#include <Log.h>
#include <format.h>
#include <timew.h>
#include <iostream>
#include <sstream>

#ifdef HAVE_COMMIT
#include <commit.h>
#endif

////////////////////////////////////////////////////////////////////////////////
int CmdDiagnostics (Database& database, Log& log)
{
  std::stringstream out;
  out << "\n"
      << PACKAGE_STRING
      << "\n";

  out << "       Platform: " << osName ()
      << "\n\n";

  // Compiler.
  out << "Compiler:\n"
#ifdef __VERSION__
      << "        Version: " << __VERSION__ << "\n"
#endif
      << "           Caps:"
#ifdef __STDC__
      << " +stdc"
#endif
#ifdef __STDC_HOSTED__
      << " +stdc_hosted"
#endif
#ifdef __STDC_VERSION__
      << " +" << __STDC_VERSION__
#endif
#ifdef _POSIX_VERSION
      << " +" << _POSIX_VERSION
#endif
#ifdef _POSIX2_C_VERSION
      << " +" << _POSIX2_C_VERSION
#endif
#ifdef _ILP32
      << " +ILP32"
#endif
#ifdef _LP64
      << " +LP64"
#endif
      << " +c"      << 8 * sizeof (char)
      << " +i"      << 8 * sizeof (int)
      << " +l"      << 8 * sizeof (long)
      << " +vp"     << 8 * sizeof (void*)
      << " +time_t" << 8 * sizeof (time_t)
      << "\n";

  // Compiler compliance level.
  std::string compliance = "non-compliant";
#ifdef __cplusplus
  int level = __cplusplus;
  if (level == 199711)
    compliance = "C++98/03";
  else if (level == 201103)
    compliance = "C++11";
  else
    compliance = format (level);
#endif
  out << "     Compliance: "
      << compliance
      << "\n\n";

  out << "Build Features\n"

  // Build date.
      << "          Built: " << __DATE__ << " " << __TIME__ << "\n"
#ifdef HAVE_COMMIT
      << "         Commit: " << COMMIT << "\n"
#endif
      << "          CMake: " << CMAKE_VERSION << "\n";

  out << "     Build type: "
#ifdef CMAKE_BUILD_TYPE
      << CMAKE_BUILD_TYPE
#else
      << "-"
#endif
      << "\n\n";

  // Config: .taskrc found, readable, writable
  out << "Configuration\n";

  char* env = getenv ("TIMEWARRIORDB");
  out << "  TIMEWARRIORDB: "
      << (env ? env : "-")
      << "\n";

  out << "       Database: "
      << database.location ()
      << "\n";

  // Determine rc.editor/$EDITOR/$VISUAL.
  char* peditor;
  if ((peditor = getenv ("VISUAL")) != NULL)
    out << "        $VISUAL: " << peditor << "\n";
  else if ((peditor = getenv ("EDITOR")) != NULL)
    out << "        $EDITOR: " << peditor << "\n";

  out << "\n";
  std::cout << out.str ();
  log.write ("info", out.str ());

  return 0;
}

////////////////////////////////////////////////////////////////////////////////