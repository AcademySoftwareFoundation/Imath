///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

/*
-------------------------------------------------------------------------------
CreateDLL.exe
-------------------------------------------------------------------------------
version 1.0
-------------------------------------------------------------------------------

Nick Porcino
Osaka, Dec 23/2004

-------------------------------------------------------------------------------

Create a DLL from an MSVC linked library with all public symbols exported. 
Makes a DLL have the same link semantics as a *nix DSO.

Eliminates the need to declspec everything.
This program also successfully exports templated objects, and all STL objects.

-------------------------------------------------------------------------------
No warranty expressed or implied, supplied as is including all faults!
-------------------------------------------------------------------------------

You must have already created a DLL using MSVC. Specify the MAP option when 
you build the DLL.

This program 
    reads in MAP file generated by your iinitial link, 
    parses all the symbols, 
    prepares a DEF file with the mangled symbols
    sniffs for required OBJS and LIBS
    creates a linker response file
    relinks your DLL using the new DEF file

you must set up an environment variable called MSVCLINK which is the full path 
to the MSVC linker. eg:
     C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\bin\link.exe

If you have a BOOST_ROOT environment variable, CreateDLL will automatically 
include BOOST_ROOT\stage\lib in the link path.

the arguments for CreateDLL are- 

    full path to the .map file.

    full path to the place where you keep all your libs that you will link against. 

    name of the generated import library. Typically it will be the same as the 
    dll, eg Imath.dll + Imath.lib. However, if the library you're working on has
    a DLL version, and a static lib version, you will probably want to distinguish
    the name of the import library like this: ImathDLL.lib, or similar, so that
    there is no confusion about what the lib actually is.

Use quotes if there are spaces. Otherwise, quotes are optional.

Example

CreateDLL "C:\coding\vc7\Dev\Ae\Ae.map" "C:\coding\objects" "C:\coding\objects\Ae.lib"

objects is the place where all the .libs have been copied to. If you have a lot 
of third party bits, eg., jpeg.lib, copy them there too.

Now your DLL has all its symbols exported! The new DLL will overwrite the
original DLL in place.

-------------------------------------------------------------------------------

Extra Magic: template instantiation

You may need some extra magic to export some templates.

If you get a link error that indicates that a templated class is not found -

Let's say you have templated class Foo, specialized on int, and Foo<int> is
needed to link your DLL.

You will need a preprocessor definition such as MYMODULE_EXPORTS which
indicates that your module is being use. Please be aware that the old schtick
of a DLL_EXPORT is not sufficient if you have more than one DLL in play. Use
this method instead.

#ifdef MSC_VER
#ifndef MYMODULE_EXPORTS
extern template class Foo<int>;
#endif
#endif

This generates a warning, but it is the microsoft sanctioned solution to this
problem.

In Foo.cpp you must also

#ifdef MSC_VER
template class Foo<int>;
#endif

-------------------------------------------------------------------------------

Extra Magic: static member variables

This program does not fix the problem that the MS dynamic loader can't
resolve static member variables of classes properly. This issue will be
the most important thing to watch out for.

example -
class Foo
{
public:
static int bar;
};

the dynamic loader puts bar in the wrong memory space. This is a fundamental
problem with the architecture of DLL's. There is no workaround, except to
change the code; example, in the header file -

#ifdef MSC_VER
extern __declspec(dllexport) int foo_bar;
#endif

and in the .cpp file, 

#ifdef MSC_VER
__declspec(dllexport) int foo_bar;
#endif

You'll get multiple declarations of foo_bar, but the linker will resolve
them down to one when the DLL is linked.

-------------------------------------------------------------------------------

Extra Magic: exporting globals

Finally, you'll still need to use declspecs for global variables.

example - 
see static member variables example for foo_bar.

-------------------------------------------------------------------------------

Extra Magic: vector deleting destructors

If you REALLY need to use a vector deleting destructor, you'll need to
declspec your class, and all the nested classes, in the traditional way.

What's the issue? The linker strips all vector deleting destructors whether
you want it to or not. declspecing the class provides a default implementation,
as opposed to the you really want.

This is a built-in hack in the linker dating back to VC5. The only workaround
is to use STL vectors instead, or don't use vector deleting destructors at all.

example -

class Foo
{
public:
   ~Foo(int size) { } // linker will discard. it can't be done.
};

===============================================================================
 */

#include "OptionParser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <process.h>				// for _spawnvp()
#include <windows.h>				// needed to create processes
#include <sys/stat.h>

using std::cerr;
using std::cout;
using std::endl;
using std::set;
using std::string;
using std::vector;

//#define DEBUGGING
#define MAX_ARGS 1000

char* filterSymbols[] = {
 //   "__real",
    "??_",
    "_== ",		    // a strange symbol from fltk
    "AEPAXI@Z",		    // all vector deleting destructors have this pattern in them
    "boost@@2_NB",	    // a boost template metaprogramming side effect that generates 1000's of unneeded symbols
    //"??0out_of_range@std@@QAE@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@1@@Z"
    //"??1out_of_range@std@@UAE@XZ",
    //"??0out_of_range@std@@QAE@ABV01@@Z",
    "out_of_range@std@@",
    "?_Checked_iterator_base@?$_Vector_const_iterator@",
    "??$_Checked_base@V?$_Vector_const_iterator@",
    "??0?$_Vector_const_iterator@",
    "??0?$_Vector_iterator@",
    //"??$_Uninit_copy@PBV?$vector@_KV?$allocator@_K@std@@",
    //"??$_Uninit_copy@PB_KPA_KV?$allocator@_K@std@@",
    "??$_Uninit_copy@PB",
    "??0?$_Tree_ptr@V?$_Tmap_traits@",
    "?_Myptr@?$basic_string@DU?$char_traits@D@std@@",
    "__imp_??1_Container_base@std", // non-existent symbols in the std
    0
};


static bool isStartSymbol(char c)
{
    return (c == '?') || (c == '_');	// ? are C++ symbols, _ are C symbols
}

static bool isWhitespace(char c)
{
    return (c == ' ') || (c == '\t') || (c == 0xa) || (c == 0xd) || (c == 26);
}

static vector<string> getSymbols(char* buf, const int length) 
{
    vector<string> strings;

    const char* eof = strstr(buf, "Static symbols");
    if (eof == 0) {
        eof = buf + length;
    }

    while (buf < eof) {

        // find a symbol
        while (buf < eof) {
            ++buf;
            if (isStartSymbol(*buf))
                break;
        }

        // if preceeding character is not ' ', skip to end of line
        if (*(buf-1) != ' ') {
            buf = strchr(buf, '\n');
            if (!buf)
                break;
        }
        // symbol starts with two underbars, skip it
        else if (buf[0] == '_' && buf[1] == '_') {
            buf = strchr(buf, '\n');
            if (!buf)
                break;
        }
        else {
            // got a symbol
            char* end = buf;
            while (end < eof) {
                ++end;
                if (*end == ' ')
                    break;
            }

            if (end < eof) {
                // is it an import or export symbol?
                char* lineend = strchr(end, '\n');
                if (lineend != 0) {
                    *lineend = '\0';

                    char* owner = strchr(end, ':');

                    // if there is no colon, it could be ours (if there is a colon it definitely isn't)
                    if (owner == 0) {

                        // if the symbol came from an obj, it is an export
                        if (*(lineend-4) == 'o' && *(lineend-3) == 'b' && *(lineend-2) == 'j') {

                            bool accept = true;
                            int filterNum = 0;
                            while (accept && filterSymbols[filterNum] != 0) {
                                if (0 != strstr(buf, filterSymbols[filterNum])) {
                                    accept = false;
                                }
                                ++filterNum;
                            }

                            if (accept) {
                                *end = '\0';
                                if (*buf == '_')    // if it's a C symbol, don't put an _ in the DEF file
                                    ++buf;
                                string temp(buf);
                                strings.push_back(temp);
                                *end = ' ';
                            }
                        }
                    }
                    *lineend = '\n';
                }
            }
            buf = end;
        }

    }

    return strings;
}


void createDef(string moduleName, FILE* mapFile, FILE* defFile)
{
    if (mapFile) {
        fseek(mapFile, 0, SEEK_END);
        int length = ftell(mapFile);
        fseek(mapFile, 0, SEEK_SET);
        char* buf = new char[length+1];
        fread(buf, 1, length, mapFile);

        char* curr = buf;

        vector<string> strings = getSymbols(buf, length);

        cout << "CreateDLL found " << (int) strings.size() << " symbols" << endl;

        fprintf(defFile, "LIBRARY\t%s\nEXPORTS\n", moduleName.c_str());
        for (size_t i = 0; i < strings.size(); ++i) {
            fprintf(defFile, "\t%s\n", strings[i].c_str());
        }

        delete [] buf;
    }
}



static bool isEOF(char c)
{
    return (c == '\0') || (c == 26);
}



static void getLibsFromMap(char* buf, const int length, set<string>& libs)
{
    const char* eof = strstr(buf, "Static symbols");
    if (eof == 0) {
        eof = buf + length;
    }

    while (buf < eof) {

        // find a symbol
        while (buf < eof) {
            ++buf;
            if (isStartSymbol(*buf))
                break;
        }

        // if preceeding character is not ' ', skip to end of line
        if (*(buf-1) != ' ') {
            buf = strchr(buf, '\n');
            if (!buf)
                break;
        }
        // symbol starts with two underbars, skip it
        /*	else if (buf[0] == '_' && buf[1] == '_') {
        buf = strchr(buf, '\n');
        if (!buf)
        break;
        } */
        else {
            // got a symbol
            char* end = buf;
            while (end < eof) {
                ++end;
                if (*end == ' ')
                    break;
            }

            if (end < eof) {
                // is it an import or export symbol?
                char* lineend = strchr(end, '\n');

                // eat all the whitespace at the end
                while (isWhitespace(*lineend))
                {
                    --lineend;
                }
                ++lineend;

                if (lineend != 0) {
                   char *le = lineend; // copy pointer as it will change before we write back the \n here!
                    *le = '\0';

                    char* owner = strchr(end, ':');

                    // if there is no colon, it could be ours (if there is a colon it definitely isn't)
                    if (owner != 0) {

                        // if the symbol came from an obj, it is an export
                        if (*(lineend-3) == 'd' && *(lineend-2) == 'l' && *(lineend-1) == 'l') {

                            lineend = lineend - 6;  // point one character before ".dll"
                            while (!isWhitespace(*lineend))
                            {
                                --lineend;
                            }
                            ++lineend;

                            // if the object isn't in the set, add it
                            string object(lineend);
                            string::size_type pos = object.find(':');
                            object = object.substr(0, pos) + ".lib";
                            if (libs.find(object) == libs.end())
                            {
                                libs.insert(object);
                            }
                        }
                    }
                    *le = '\n';
                }
            }
            buf = end;
        }
    }
}



static void addLibsFromVector(set<string>& libs, vector<string>& morelibs)
{
    for (size_t i = 0; i < morelibs.size(); ++i)
    {
        if (libs.find(morelibs[i]) == libs.end())
        {
            libs.insert(morelibs[i]);
        }
    }
}



static void getObjsFromMap(char* buf, const int length, set<string>& objs)
{
    const char* eof = strstr(buf, "Static symbols");
    if (eof == 0) {
        eof = buf + length;
    }

    while (buf < eof) {

        // find a symbol
        while (buf < eof) {
            ++buf;
            if (isStartSymbol(*buf))
                break;
        }

        // if preceeding character is not ' ', skip to end of line
        if (*(buf-1) != ' ') {
            buf = strchr(buf, '\n');
            if (!buf)
                break;
        }
        // symbol starts with two underbars, skip it
        else if (buf[0] == '_' && buf[1] == '_') {
            buf = strchr(buf, '\n');
            if (!buf)
                break;
        }
        else {
            // got a symbol
            char* end = buf;
            while (end < eof) {
                ++end;
                if (*end == ' ')
                    break;
            }

            if (end < eof) {
                // is it an import or export symbol?
                char* lineend = strchr(end, '\n');

                // eat all the whitespace at the end
                while (isWhitespace(*lineend))
                {
                    --lineend;
                }
                ++lineend;

                if (lineend != 0) {
                   char *le = lineend; // copy pointer as it will change before we write back the \n here!
                    *le = '\0';

                    char* owner = strchr(end, ':');

                    // if there is no colon, it could be ours (if there is a colon it definitely isn't)
                    if (owner == 0) {

                        // if the symbol came from an obj, it is an export
                        if (*(lineend-3) == 'o' && *(lineend-2) == 'b' && *(lineend-1) == 'j') {

                            lineend = lineend - 6;  // point one character before ".obj"
                            while (!isWhitespace(*lineend))
                            {
                                --lineend;
                            }
                            ++lineend;

                            // if the object isn't in the set, add it
                            string object(lineend);
                            if (objs.find(object) == objs.end())
                            {
                                objs.insert(object);
                            }
                        }
                    }
                    *le = '\n';
                }
            }
            buf = end;
        }

    }
}




static void getSymbolsFromMap(char* buf, const int length, vector<string>& strings) 
{
    const char* eof = strstr(buf, "Static symbols");
    if (eof == 0) {
        eof = buf + length;
    }

    while (buf < eof) {

        // find a symbol
        while (buf < eof) {
            ++buf;
            if (isStartSymbol(*buf))
                break;
        }

        // if preceeding character is not ' ', skip to end of line
        if (*(buf-1) != ' ') {
            buf = strchr(buf, '\n');
            if (!buf)
                break;
        }
        // symbol starts with two underbars, skip it
        else if (buf[0] == '_' && buf[1] == '_') {
            buf = strchr(buf, '\n');
            if (!buf)
                break;
        }
        else {
            // got a symbol
            char* end = buf;
            while (end < eof) {
                ++end;
                if (*end == ' ')
                    break;
            }

            if (end < eof) {
                // is it an import or export symbol?
                char* lineend = strchr(end, '\n');
                if (lineend != 0) {
                    *lineend = '\0';

                    char* owner = strchr(end, ':');

                    // if there is no colon, it could be ours (if there is a colon it definitely isn't)
                    if (owner == 0) {

                        // if the symbol came from an obj, it is an export
                        if (*(lineend-4) == 'o' && *(lineend-3) == 'b' && *(lineend-2) == 'j') {

                            bool accept = true;
                            int filterNum = 0;
                            while (accept && filterSymbols[filterNum] != 0) {
                                if (0 != strstr(buf, filterSymbols[filterNum])) {
                                    accept = false;
                                }
                                ++filterNum;
                            }

                            if (accept) {
                                *end = '\0';
                                if (*buf == '_')    // if it's a C symbol, don't put an _ in the DEF file
                                    ++buf;
                                string temp(buf);
                                strings.push_back(temp);
                                *end = ' ';
                            }
                        }
                    }
                    *lineend = '\n';
                }
            }
            buf = end;
        }

    }
}


static void createResponseFile(string& repFile, string& path, string& moduleName, vector<string>& libpath, string& implib, set<string>& libs, set<string>& objs, bool noDefaultLibs)
{
    FILE* r = fopen(repFile.c_str(), "wb");
    if (r != 0)
    {
        fprintf(r, "/OUT:\"%s\\%s.dll\" /NOLOGO ", path.c_str(), moduleName.c_str());

        // add boost if the user has it. todo, is stage/lib always the right place?
        char* root = getenv("BOOST_ROOT");
        if (root != 0)
            fprintf(r, "/LIBPATH:\"%s\\stage\\lib\" ", root);

        for (size_t i = 0; i < libpath.size(); ++i)
        {
            // add user lib path
            fprintf(r, "/LIBPATH:\"%s\" ", libpath[i].c_str());
        }

        // add .obj path
        fprintf(r, "/LIBPATH:\"%s\" ", path.c_str());

        // import lib
        fprintf(r, "/IMPLIB:\"%s\" ", implib.c_str());

        // /NODEFAULTLIB:library
        if (noDefaultLibs)
            fprintf(r, "/NODEFAULTLIB ");

        // misc stuff
        fprintf(r, "/DLL /DEBUG /PDB:\"%s\\%s.pdb\" /DEF:\"%s\\%s.map.DEF\" /MACHINE:X86 ", 
            path.c_str(), moduleName.c_str(),       // PDB
            path.c_str(), moduleName.c_str());      // DEF

        for (set<string>::iterator lib = libs.begin(); lib != libs.end(); ++lib)
        {
            fprintf(r, "%s ", (*lib).c_str());
        }

        for (set<string>::iterator obj = objs.begin(); obj != objs.end(); ++obj)
        {
            fprintf(r, "%s ", (*obj).c_str());
        }

        // clean up
        fclose(r);
    }
    else
    {
        std::cerr << "Unable to open response file C:\\response.txt" << endl;
    }
}

int main(int argC, char* argV[])
{
    OptionParser options(argV[0]);

    vector<string> libpaths;
    options.AddStringVectorOption("-l", "--libdir", libpaths, "semi-colon separated lib dirs");
    vector<string> morelibs;
    options.AddStringVectorOption("-o", "--morelibs", morelibs, "comma separated additional libs");
    string importlib;
    options.AddStringOption("-i", "--importlib", importlib, "the import lib to go with the DLL");
    string mapFileName;
    options.AddStringOption("-n", "--mapfile", mapFileName, "name of map file to generate");
    bool noDefaultLib = false;
    options.AddTrueOption("-d", "--nodefaultlib", noDefaultLib, "invoke /NODEFAULTLIB:library option");

    // make a command line less argV[0] which is the executable path
    std::string commandLine = Join(argC-1, &argV[1], " ");
    options.Parse(commandLine);

    string::size_type pos;

    std::cout << "CreateDLL v1.1" << std::endl;

    char tempDir[MAX_PATH];
    int rt = GetTempPath(MAX_PATH, tempDir);

    // 20 less because we have to have room for the filename
    if (rt == 0 || rt > MAX_PATH - 20)
    {
	    std::cerr << "Failed to get temp dir\n";
	    return 1;
    }

    std::string repFile(tempDir);
    repFile += "response.txt";

    std::string logName(tempDir);
    logName += "createDLL_out.txt";

    std::cout << "Linker response file is " << repFile << "\n";
    std::cout << "Log file is " << logName << "\n";

    // banner
    std::ofstream logfile(logName.c_str(), std::ios::out);
    logfile << "createDLL was built " << __DATE__ << " " << __TIME__ << endl;

    if (mapFileName == "" || importlib == "")
    {
        options.Usage();
    }
    else
    {
        string mapName = mapFileName; //GetArg("-n", args);
        const char* ext = strrchr(mapName.c_str(), '.');
        if (ext == 0)
        {
            mapName += ".MAP";
        }

        FILE* mapFile = fopen(mapName.c_str(), "rb");
        if (mapFile != 0)
        {
            logfile << "Processing " << mapName << std::endl;

            // read the map file in
            fseek(mapFile, 0, SEEK_END);
            int length = ftell(mapFile);
            fseek(mapFile, 0, SEEK_SET);
            char* buf = new char[length+1];
            fread(buf, 1, length, mapFile);
            fclose(mapFile);

            // get symbols
            vector<string> symbols;
            getSymbolsFromMap(buf, length, symbols);

            // get object files to link
            set<string> objs;
            getObjsFromMap(buf, length, objs);

            // get libs to link against
            set<string> libs;
            getLibsFromMap(buf, length, libs);

            //vector<string> morelibs;
            //GetArgs("-o", args, morelibs);
            //addLibsFromVector(libs, morelibs); 

            // add libs from the command line
            for (vector<string>::iterator i = morelibs.begin(); i != morelibs.end(); ++i)
            {
                std::string temp = *i;
                pos = temp.find(".lib");
                if (pos == string::npos)
                {
                    temp += ".lib";
                }

                if (libs.find(temp) == libs.end())
                {
                    libs.insert(temp);
                }
            }

            // helpful info in log
            logfile << "Input Object files: " << endl;
            for (set<string>::iterator objIter = objs.begin(); objIter != objs.end(); ++objIter)
            {
                logfile << *objIter << std::endl;
            }

            // helpful info in log
            logfile << "Input Libraries: " << endl;
            for (set<string>::iterator objIter = libs.begin(); objIter != libs.end(); ++objIter)
            {
                logfile << *objIter << std::endl;
            }

            // re-open the map file
            mapFile = fopen(mapName.c_str(), "rb");

            // create the def file
            string defName = mapName + ".DEF";
            FILE* defFile = fopen(defName.c_str(), "wb");

            // get the module name from the map name
            char const* sub = strrchr(mapName.c_str(), '/');
            if (sub == 0) sub = strrchr(mapName.c_str(), '\\');
            if (sub == 0) sub = mapName.c_str();
            if (*sub == '/' || *sub == '\\') ++sub;
            string moduleName(sub);
            if (strrchr(sub, '.'))
            {
                moduleName = moduleName.substr(0, moduleName.length() - 4);
            }

            // write out the def file
            createDef(moduleName, mapFile, defFile);

            // close files
            fclose(mapFile);
            fclose(defFile);

            // strip module name off map path
            string path(mapName);
            pos = path.rfind('/');
            if (pos != string::npos)
                path = path.substr(0, pos);
            pos = path.rfind('\\');
            if (pos != string::npos)
                path = path.substr(0, pos);

            // create a response file
            //vector<string> libpaths;
            //GetArgs("-l", args, libpaths);

            // if there is a .lib at the end, strip it off,
            // because in the future I probably want to use
            // the name of the implib to generate the pdb as well
            // I haven't done that yet.
            //string implib = GetArg("-i", args);
            pos = importlib.find(".lib");
            if (pos != string::npos)
            {
                importlib = importlib.substr(0, pos);
            }

            // silly placeholder code, given the comment above :\ 
            string impliblib = importlib + ".lib";

            createResponseFile(repFile, path, moduleName, libpaths, impliblib, libs, objs, noDefaultLib);

            // invoke linker
            cerr << "Exporting DLL" << endl;

            // run the linker
            char**  new_argv = new char *[3];
            size_t  new_argc = 0;
	    std::string repFileArg("@");
	    repFileArg += repFile;
			
            new_argv[0] = "createDLL";
            new_argv[1] = (char*)repFileArg.c_str();
            new_argv[2] = 0;

            int status = -1;
            const char* linkerString = getenv("MSVCLINK");
            string nativeLinker;
			if ( linkerString == NULL )
				linkerString = "link.exe";
            if (linkerString != 0)
            {
                nativeLinker.assign(linkerString);
                status = (int) _spawnvp(_P_WAIT, nativeLinker.c_str(), new_argv);
            }
            else
            {
                MessageBox(0, "createDLL: MSVCLINK environment variable must set to the full path to the msvc linker", "CreateDLL", MB_OK);

                logfile << "MSVCLINK environment variable must be set to the full path to the msvc linker?" << endl;
                cerr << "MSVCLINK environment variable must be set to the full path to the msvc linker?" << endl;
                status = 1;
            }

            if (linkerString == 0 || status == -1)
            {
                MessageBox(0, "createDLL: Failure to start linker.", "CreateDLL", MB_OK);

                logfile << "createDLL: Failure to start linker named: " << nativeLinker << " (does your PATH include it?)" << endl;
                cerr << "createDLL: Failure to start linker named: " << nativeLinker << " (does your PATH include it?)" << endl;
                status = 1;
            }
            else
            {
                cerr << "createDLL: linking success" << endl;
            }

            // clean up
            delete [] new_argv;
            delete [] buf;
        }
        else
        {
            logfile	<< "Specified mapfile " << mapName << " not found" << endl;
            std::cerr	<< "Specified mapfile " << mapName << " not found" << endl;
        }
    }

    // end log
    logfile << endl;
    logfile.close();
}