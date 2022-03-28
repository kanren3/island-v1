#include "defs.h"
#include "common.h"

VOID
WINAPI
CommandExecute(
	__in PCSTR Command
)
{
	DebugControl->lpVtbl->Execute(
		DebugControl,
		DEBUG_OUTCTL_IGNORE,
		Command,
		DEBUG_EXECUTE_NOT_LOGGED);
}

ULONG64
WINAPI
GetOffsetByName(
	__in PCSTR Symbol
)
{
	ULONG64 Offset = 0;

	DebugSymbols->lpVtbl->GetOffsetByName(
		DebugSymbols,
		Symbol,
		&Offset);

	return Offset;
}

HRESULT
WINAPI
GetRegisterValue(
	__in DEBUG_REG_TYPE Register,
	__in PDEBUG_VALUE Value
)
{
	HRESULT Result = S_OK;

	Result = DebugRegisters->lpVtbl->GetValue(
		DebugRegisters,
		Register,
		Value);

	return Result;
}