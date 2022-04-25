#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdio.h>
#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <capstone/capstone.h>
#include <unicorn/unicorn.h>
#include <winternl.h>

#ifndef NOTHING
#define NOTHING
#endif

#ifndef _WIN64
#define KDEXT_32BIT
#else
#define KDEXT_64BIT
#endif
#include <initguid.h>
#include <wdbgexts.h>
#include <dbgeng.h>

#include "private.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern PDEBUG_CLIENT DebugClient;
	extern PDEBUG_CONTROL DebugControl;
	extern PDEBUG_REGISTERS DebugRegisters;
	extern PDEBUG_SYMBOLS DebugSymbols;
	extern PDEBUG_SYSTEM_OBJECTS DebugSystemObjects;
	extern PDEBUG_DATA_SPACES DebugDataSpaces;
	extern ULONG64 MmLoadSystemImage;
	extern ULONG64 MmLoadSystemImageEx;

#ifdef __cplusplus
}
#endif

#endif
