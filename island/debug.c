#include "defs.h"
#include "helper.h"
#include "emulator.h"
#include "debug.h"

DEBUG_BLOCK DebugBlock = { 0 };

IDebugEventCallbacks DebugEventCallbacks = { 0 };
IDebugEventCallbacksVtbl DebugEventCallbacksVtbl = { 0 };

ULONG
CALLBACK
DebugAddRef(
    PDEBUG_EVENT_CALLBACKS EventCallback
)
{
    return 1;
}

ULONG
CALLBACK
DebugRelease(
    PDEBUG_EVENT_CALLBACKS EventCallback
)
{
    return 1;
}

HRESULT
CALLBACK
DebugGetInterestMask(
    PDEBUG_EVENT_CALLBACKS EventCallback,
    PULONG Mask
)
{
    *Mask = DEBUG_EVENT_BREAKPOINT;
    return S_OK;
}

HRESULT
CALLBACK
DebugBreakpoint(
    PDEBUG_EVENT_CALLBACKS EventCallback,
    PDEBUG_BREAKPOINT Breakpoint
)
{
    ULONG64 BreakpointOffset = 0;
    DEBUG_VALUE RspValue = { 0 };
    ULONG64 SystemImageBasePtr = 0;
    ULONG64 SystemImageBase = 0;
    IMAGE_DOS_HEADER DosHeader = { 0 };
    IMAGE_NT_HEADERS NtHeaders = { 0 };

    ULONG DebugState = DEBUG_STATUS_GO;
    ULONG UcState = UC_ERR_OK;
    HRESULT Result = S_OK;
    CONTEXT Context = { 0 };
    USHORT TaskRegister = 0x40;
    ULONG64 GsBase = 0;
    PVOID ImageSnapshoot = NULL;
    CHAR SymbolsName[MAX_PATH] = { 0 };
    ULONG64 ReturnAddress;
    ULONG CurrentThreadId = 0;

    DebugSystemObjects->lpVtbl->GetCurrentThreadId(
        DebugSystemObjects,
        &CurrentThreadId);

    Breakpoint->lpVtbl->GetOffset(Breakpoint, &BreakpointOffset);

    if (BreakpointOffset == DebugBlock.MmLoadSystemImage ||
        BreakpointOffset == DebugBlock.MmLoadSystemImageEx) {
        RemoveDebugBreakPoint(DebugBlock.OnceBreakpoint);

        GetRegisterValue(DEBUG_REG_RSP, &RspValue);

        ReadMemory(
            RspValue.I64,
            &DebugBlock.LoadImageRetAddress,
            sizeof(DebugBlock.LoadImageRetAddress),
            NULL);

        DebugBlock.OnceBreakpoint = AddDebugBreakPoint(DebugBlock.LoadImageRetAddress);

        DebugBlock.LoadImageStack = RspValue.I64 + 0x30;

        return DEBUG_STATUS_GO;
    }

    if (BreakpointOffset == DebugBlock.LoadImageRetAddress) {
        RemoveDebugBreakPoint(DebugBlock.OnceBreakpoint);

        ReadMemory(
            DebugBlock.LoadImageStack,
            &SystemImageBasePtr,
            sizeof(SystemImageBasePtr),
            NULL);

        if (0 != SystemImageBasePtr) {
            ReadMemory(
                SystemImageBasePtr,
                &SystemImageBase,
                sizeof(SystemImageBase),
                NULL);

            ReadMemory(
                SystemImageBase,
                &DosHeader,
                sizeof(DosHeader),
                NULL);

            ReadMemory(
                SystemImageBase + DosHeader.e_lfanew,
                &NtHeaders,
                sizeof(NtHeaders),
                NULL);

            DebugBlock.OnceBreakpoint = AddDebugBreakPoint(SystemImageBase + NtHeaders.OptionalHeader.AddressOfEntryPoint);

            return DEBUG_STATUS_GO;
        }
    }

    if (BreakpointOffset == DebugBlock.TraceAddress) {
        if (CurrentThreadId != DebugBlock.TraceThreadId) {
            return DEBUG_STATUS_NO_CHANGE;
        }

        RemoveDebugBreakPoint(DebugBlock.OnceBreakpoint);

        if (FALSE == DebugBlock.TraceStop) {
            InitializeEmulator();

            if (UC_ERR_OK == UcReadyEmulatorGdtr()) {
                GetDebugContext(&Context);
                UcLoadContext(&Context);

                UcWriteRegister(UC_X86_REG_TR, &TaskRegister);

                ReadMsr(IA32_GS_BASE, &GsBase);
                UcWriteMsr(IA32_GS_BASE, GsBase);

                DebugBlock.TraceImageBase = GetFunctionImageBase(
                    Context.Rip, &DebugBlock.TraceSizeOfImage);

                if (0 != DebugBlock.TraceImageBase) {
                    ImageSnapshoot = RinHeapAlloc(DebugBlock.TraceSizeOfImage);

                    ReadMemory(
                        DebugBlock.TraceImageBase,
                        ImageSnapshoot,
                        DebugBlock.TraceSizeOfImage,
                        NULL);

                    UcMapMemoryFromPtr(
                        DebugBlock.TraceImageBase,
                        ImageSnapshoot,
                        DebugBlock.TraceSizeOfImage,
                        UC_PROT_ALL);

                    RinHeapFree(ImageSnapshoot);

                    UcSetCallback(
                        NULL, NULL, NULL, NULL, NULL, NULL,
                        EmulatorMemoryNotify);

                    UcState = UcEmulatorStart(Context.Rip, 0);

                    if (UC_ERR_FETCH_UNMAPPED == UcState) {
                        UcSaveContext(&Context);

                        if (TraceTypeBefore == DebugBlock.TraceType) {
                            Result = DebugSymbols->lpVtbl->GetNameByOffset(
                                DebugSymbols,
                                Context.Rip,
                                SymbolsName,
                                sizeof(SymbolsName),
                                NULL,
                                NULL);

                            if (strstr(SymbolsName, DebugBlock.TraceFunction)) {
                                DebugBlock.TraceStop = TRUE;

                                DebugBlock.OnceBreakpoint = AddDebugBreakPoint(Context.Rip);
                            }
                            else {
                                DebugBlock.TraceStop = FALSE;

                                UcReadMemory(
                                    Context.Rsp,
                                    &DebugBlock.TraceAddress,
                                    sizeof(DebugBlock.TraceAddress));

                                DebugBlock.OnceBreakpoint = AddDebugBreakPoint(DebugBlock.TraceAddress);
                            }
                        }
                        else {
                            Result = DebugSymbols->lpVtbl->GetNameByOffset(
                                DebugSymbols,
                                Context.Rip,
                                SymbolsName,
                                sizeof(SymbolsName),
                                NULL,
                                NULL);

                            if (strstr(SymbolsName, DebugBlock.TraceFunction)) {
                                DebugBlock.TraceStop = TRUE;

                                UcReadMemory(
                                    Context.Rsp,
                                    &ReturnAddress,
                                    sizeof(ReturnAddress));

                                DebugBlock.OnceBreakpoint = AddDebugBreakPoint(ReturnAddress);
                            }
                            else {
                                DebugBlock.TraceStop = FALSE;

                                UcReadMemory(
                                    Context.Rsp,
                                    &DebugBlock.TraceAddress,
                                    sizeof(DebugBlock.TraceAddress));

                                DebugBlock.OnceBreakpoint = AddDebugBreakPoint(DebugBlock.TraceAddress);
                            }
                        }
                    }
                    else {
                        DebugState = DEBUG_STATUS_NO_CHANGE;
                    }
                }
                else {
                    dprintf("[Island] Please reload symbols\n");
                }
            }

            UninitializeEmulator();
        }

        return DebugState;
    }

    return DEBUG_STATUS_NO_CHANGE;
}

HRESULT
WINAPI
InitializeDebugEvent(
    VOID
)
{
    HRESULT Result = S_OK;

    DebugEventCallbacks.lpVtbl = &DebugEventCallbacksVtbl;
    DebugEventCallbacks.lpVtbl->AddRef = DebugAddRef;
    DebugEventCallbacks.lpVtbl->Release = DebugRelease;
    DebugEventCallbacks.lpVtbl->GetInterestMask = DebugGetInterestMask;
    DebugEventCallbacks.lpVtbl->Breakpoint = DebugBreakpoint;

    Result = DebugClient->lpVtbl->SetEventCallbacks(
        DebugClient,
        &DebugEventCallbacks);

    return Result;
}

PDEBUG_BREAKPOINT
WINAPI
AddDebugBreakPoint(
    __in ULONG64 Address
)
{
    HRESULT Result = S_OK;
    PDEBUG_BREAKPOINT Breakpoint = NULL;

    Result = DebugControl->lpVtbl->AddBreakpoint(
        DebugControl,
        DEBUG_BREAKPOINT_DATA,
        DEBUG_ANY_ID,
        &Breakpoint);

    if (FALSE != SUCCEEDED(Result)) {
        Breakpoint->lpVtbl->SetOffset(Breakpoint, Address);
        Breakpoint->lpVtbl->SetFlags(Breakpoint, DEBUG_BREAKPOINT_ENABLED);
    }

    return Breakpoint;
}

HRESULT
WINAPI
RemoveDebugBreakPoint(
    __in PDEBUG_BREAKPOINT Breakpoint
)
{
    HRESULT Result = S_OK;

    Result = DebugControl->lpVtbl->RemoveBreakpoint(
        DebugControl,
        Breakpoint);

    return Result;
}