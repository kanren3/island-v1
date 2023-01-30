#include "defs.h"
#include "helper.h"

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

VOID
WINAPI
GetDebugContext(
	__out PCONTEXT Context
)
{
	DEBUG_VALUE Value;

	GetRegisterValue(DEBUG_REG_RAX, &Value);
	GetValueFormPointer(Context->Rax, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_RCX, &Value);
	GetValueFormPointer(Context->Rcx, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_RDX, &Value);
	GetValueFormPointer(Context->Rdx, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_RBX, &Value);
	GetValueFormPointer(Context->Rbx, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_RSP, &Value);
	GetValueFormPointer(Context->Rsp, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_RBP, &Value);
	GetValueFormPointer(Context->Rbp, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_RSI, &Value);
	GetValueFormPointer(Context->Rsi, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_RDI, &Value);
	GetValueFormPointer(Context->Rdi, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_R8, &Value);
	GetValueFormPointer(Context->R8, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_R9, &Value);
	GetValueFormPointer(Context->R9, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_R10, &Value);
	GetValueFormPointer(Context->R10, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_R11, &Value);
	GetValueFormPointer(Context->R11, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_R12, &Value);
	GetValueFormPointer(Context->R12, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_R13, &Value);
	GetValueFormPointer(Context->R13, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_R14, &Value);
	GetValueFormPointer(Context->R14, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_R15, &Value);
	GetValueFormPointer(Context->R15, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_RIP, &Value);
	GetValueFormPointer(Context->Rip, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_EFL, &Value);
	GetValueFormPointer(Context->EFlags, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM0, &Value);
	GetValueFormPointer(Context->Xmm0, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM1, &Value);
	GetValueFormPointer(Context->Xmm1, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM2, &Value);
	GetValueFormPointer(Context->Xmm2, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM3, &Value);
	GetValueFormPointer(Context->Xmm3, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM4, &Value);
	GetValueFormPointer(Context->Xmm4, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM5, &Value);
	GetValueFormPointer(Context->Xmm5, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM6, &Value);
	GetValueFormPointer(Context->Xmm6, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM7, &Value);
	GetValueFormPointer(Context->Xmm7, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM8, &Value);
	GetValueFormPointer(Context->Xmm8, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM9, &Value);
	GetValueFormPointer(Context->Xmm9, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM10, &Value);
	GetValueFormPointer(Context->Xmm10, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM11, &Value);
	GetValueFormPointer(Context->Xmm11, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM12, &Value);
	GetValueFormPointer(Context->Xmm12, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM13, &Value);
	GetValueFormPointer(Context->Xmm13, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM14, &Value);
	GetValueFormPointer(Context->Xmm14, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_XMM15, &Value);
	GetValueFormPointer(Context->Xmm15, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_MXCSR, &Value);
	GetValueFormPointer(Context->MxCsr, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_DR0, &Value);
	GetValueFormPointer(Context->Dr0, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_DR1, &Value);
	GetValueFormPointer(Context->Dr1, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_DR2, &Value);
	GetValueFormPointer(Context->Dr2, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_DR3, &Value);
	GetValueFormPointer(Context->Dr3, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_DR6, &Value);
	GetValueFormPointer(Context->Dr6, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_DR7, &Value);
	GetValueFormPointer(Context->Dr7, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_ES, &Value);
	GetValueFormPointer(Context->SegEs, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_CS, &Value);
	GetValueFormPointer(Context->SegCs, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_SS, &Value);
	GetValueFormPointer(Context->SegSs, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_DS, &Value);
	GetValueFormPointer(Context->SegDs, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_FS, &Value);
	GetValueFormPointer(Context->SegFs, Value.RawBytes);

	GetRegisterValue(DEBUG_REG_GS, &Value);
	GetValueFormPointer(Context->SegGs, Value.RawBytes);
}

ULONG64
WINAPI
GetFunctionImageBase(
	__in ULONG64 Function,
	__out PULONG SizeOfImage
)
{
	ULONG64 ImageBase = 0;
	IMAGE_DOS_HEADER DosHeader = { 0 };
	IMAGE_NT_HEADERS NtHeaders = { 0 };

	DebugSymbols->lpVtbl->GetModuleByOffset(
		DebugSymbols,
		Function,
		0,
		NULL,
		&ImageBase);

	if (0 != ImageBase) {
		ReadMemory(
			ImageBase,
			&DosHeader,
			sizeof(DosHeader),
			NULL);

		ReadMemory(
			ImageBase + DosHeader.e_lfanew,
			&NtHeaders,
			sizeof(NtHeaders),
			NULL);

		*SizeOfImage = NtHeaders.OptionalHeader.SizeOfImage;
	}

	return ImageBase;
}