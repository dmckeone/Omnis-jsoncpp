Omnis-jsoncpp
Copyright (c) 2010 David McKeone
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

---------------------------------------------------------------

- DESCRIPTION

---------------------------------------------------------------

This project is a wrapper of the jsoncpp library for reading/writing JSON files.

It is required to download the SDK from TigerLogic in order to setup the build environment variables 
required for this project: 
http://www.tigerlogic.com/tigerlogic/omnis/products/components/buildyourown.jsp

---------------------------------------------------------------

- DESCRIPTION OF FILES

---------------------------------------------------------------

../include/jsoncpp.he
../src/jsoncpp.cpp     : Main entry point for Omnis external.  These files control the dispatch of messages to appropriate non-visual objects and the definition of non-visual objects.

../include/NVObjBase.he
../src/NVObjBase.cpp    : These files hold the base class, NVObjBase, to be used for all non-visual objects.  It provides basic methods for defining a non-visual object.

../include/OmnisTools.he
../src/OmnisTools.cpp    : These files hold a series of helper methods that I've found useful while creating components.

../src/jsoncpp.rc : This file holds the "resources" -- string table for method, property, and constant descriptions -- that Omnis uses for the component.  (Resource 32000 is the name of the entry point method in NVObjTemplate.cpp)

../src/jsoncpp.def : This file holds the entry point name for Windows components (Should match resource 32000 in jsoncpp)


---------------------------------------------------------------

- BUILD TREE

---------------------------------------------------------------


Windows and Mac projects are configured to look at directories within the "MyProjects" folder, so no
additional work should be required for compiling.  It's better to avoid spaces in paths as the Omnis 
build system doesn't properly recognize them.


Omnis External Dependencies:
-----------------------------
See "Read Before Compiling.txt" for instructions on setting up the OMNIS_LIB_PATH environment variable
$OMNIS_LIB_PATH/Mac       = Omnis external component dependencies for Mac (complib and frameworks folders)
$(OMNIS_LIB_PATH)/Windows = Omnis external component dependencies for Windows (COMPLIB, EXTLIB, and ULIBS folders)

Source Code Locations:
-----------------------------
include = Header fields for component NVObjTemplate
src     = Source files for component NVObjTemplate 
proj    = Platform dependant project files 


Project Locations:
-----------------------------
proj    = Platform dependant project files 


---------------------------------------------------------------

- BUILD OUTPUT

---------------------------------------------------------------


Debug / Development output puts the component in the Omnis Studio xcomp folder (See OMNIS DIRECTORY).


Release output puts the component in the deploy folder.
deploy/Mac/     = Mac component
deploy\Windows\ = Windows component 



---------------------------------------------------------------

- OMNIS DIRECTORY

---------------------------------------------------------------

The Omnis directory must be setup prior to compiling any debug versions of the external.  It is read 
from an Environment Variable, OMNIS_XCOMP_PATH.

For windows this is like: C:\TigerLogic\OS50\xcomp
- Set under System Properties (Access by Right-Clicking My Computer and then selecting Properties)
- Requires close and re-open of Visual Studio


For Mac this is like: HD:Applications:OmnisStudio:Contents:MacOS:xcomp
- Set in the environment.plist file in the user home directory ~/.MacOSX/environment.plist 
- Requires log off of OS X

