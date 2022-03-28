#include "defs.h"
#include "common.h"
#include "debug.h"

IDebugEventCallbacks DebugEventCallbacks = { 0 };
IDebugEventCallbacksVtbl DebugEventCallbacksVtbl = { 0 };
ULONG64 MmLoadSystemImageRet = 0;
ULONG64 SystemImageBaseStack = 0;

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

    Breakpoint->lpVtbl->GetOffset(Breakpoint, &BreakpointOffset);

    if (BreakpointOffset == MmLoadSystemImage ||
        BreakpointOffset == MmLoadSystemImageEx) {
        GetRegisterValue(DEBUG_REG_RSP, &RspValue);

        ReadMemory(
            RspValue.I64,
            &MmLoadSystemImageRet,
            sizeof(MmLoadSystemImageRet),
            NULL);

        AddDebugBreakPoint(
            MmLoadSystemImageRet,
            DEBUG_BREAKPOINT_ENABLED | DEBUG_BREAKPOINT_ONE_SHOT);

        SystemImageBaseStack = RspValue.I64 + 0x30;

        return DEBUG_STATUS_GO;
    }

    if (BreakpointOffset == MmLoadSystemImageRet) {
        ReadMemory(
            SystemImageBaseStack,
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

            AddDebugBreakPoint(
                SystemImageBase + NtHeaders.OptionalHeader.AddressOfEntryPoint,
                DEBUG_BREAKPOINT_ENABLED | DEBUG_BREAKPOINT_ONE_SHOT);

            return DEBUG_STATUS_GO;
        }
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
    __in ULONG64 Address,
    __in ULONG Flags
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
        Breakpoint->lpVtbl->SetFlags(Breakpoint, Flags);
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