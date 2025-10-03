# Setup for RtMidi for Windows build
1. Add RtMidi.h and RtMidi.cpp file into Solution  
2. Add the following two lines at the start of the RtMidi.h file  
	#define __WINDOWS_MM__  
	#pragma comment (lib, "winmm.lib")  
3. In the Solutions Properties:  
	Linker > Input >> Additional dependencies: add winmm.lib 
