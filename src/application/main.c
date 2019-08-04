// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "api/application.h"
#if defined(_WIN32) && defined(_MSC_VER)
#include <vld.h>
#endif

int main(int argc, char* argv[]) {

#if _WIN32
	// _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    DEBUG_PRINT("Starting Sandbox v0.1.\n");

    AApplication_Run(argc, argv);

    DEBUG_PRINT("Goodbye!\n");

#if _WIN32
	// _CrtDumpMemoryLeaks();
#endif

    return 0;

}