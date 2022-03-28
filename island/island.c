#include "defs.h"
#include "common.h"
#include "debug.h"
#include "island.h"

WINDBG_EXTENSION_APIS ExtensionApis = { 0 };

PDEBUG_CLIENT DebugClient = NULL;
PDEBUG_CONTROL DebugControl = NULL;
PDEBUG_REGISTERS DebugRegisters = NULL;
PDEBUG_SYMBOLS DebugSymbols = NULL;
PDEBUG_SYSTEM_OBJECTS DebugSystemObjects = NULL;
PDEBUG_DATA_SPACES DebugDataSpaces = NULL;
ULONG64 MmLoadSystemImage = 0;
ULONG64 MmLoadSystemImageEx = 0;

VOID
CALLBACK
DebugExtensionUninitialize(
    VOID
)
{
    if (NULL != DebugClient) {
        DebugClient->lpVtbl->Release(DebugClient);
    }
    if (NULL != DebugControl) {
        DebugControl->lpVtbl->Release(DebugControl);
    }
    if (NULL != DebugRegisters) {
        DebugRegisters->lpVtbl->Release(DebugRegisters);
    }
    if (NULL != DebugSymbols) {
        DebugSymbols->lpVtbl->Release(DebugSymbols);
    }
    if (NULL != DebugSystemObjects) {
        DebugSystemObjects->lpVtbl->Release(DebugSystemObjects);
    }
    if (NULL != DebugDataSpaces) {
        DebugDataSpaces->lpVtbl->Release(DebugDataSpaces);
    }
}

HRESULT
CALLBACK
DebugExtensionInitialize(
    __in PULONG Version,
    __in PULONG Flags
)
{
    HRESULT Result = S_OK;

    Result = DebugCreate(
        &IID_IDebugClient,
        &DebugClient);

    if (FALSE == SUCCEEDED(Result)) {
        DebugExtensionUninitialize();
        return Result;
    }

    Result = DebugClient->lpVtbl->QueryInterface(
        DebugClient,
        &IID_IDebugControl,
        &DebugControl);

    if (FALSE == SUCCEEDED(Result)) {
        DebugExtensionUninitialize();
        return Result;
    }

    Result = DebugClient->lpVtbl->QueryInterface(
        DebugClient,
        &IID_IDebugRegisters,
        &DebugRegisters);

    if (FALSE == SUCCEEDED(Result)) {
        DebugExtensionUninitialize();
        return Result;
    }

    Result = DebugClient->lpVtbl->QueryInterface(
        DebugClient,
        &IID_IDebugSymbols,
        &DebugSymbols);

    if (FALSE == SUCCEEDED(Result)) {
        DebugExtensionUninitialize();
        return Result;
    }

    Result = DebugClient->lpVtbl->QueryInterface(
        DebugClient,
        &IID_IDebugSystemObjects,
        &DebugSystemObjects);

    if (FALSE == SUCCEEDED(Result)) {
        DebugExtensionUninitialize();
        return Result;
    }

    Result = DebugClient->lpVtbl->QueryInterface(
        DebugClient,
        &IID_IDebugDataSpaces,
        &DebugDataSpaces);

    if (FALSE == SUCCEEDED(Result)) {
        DebugExtensionUninitialize();
        return Result;
    }

    ExtensionApis.nSize = sizeof(ExtensionApis);

#ifndef KDEXT_32BIT
    Result = DebugControl->lpVtbl->GetWindbgExtensionApis64(
        DebugControl,
        &ExtensionApis);
#else
    Result = DebugControl->lpVtbl->GetWindbgExtensionApis32(
        DebugControl,
        &ExtensionApis);
#endif

    if (FALSE == SUCCEEDED(Result)) {
        DebugExtensionUninitialize();
        return Result;
    }

    Result = InitializeDebugEvent();

    if (FALSE == SUCCEEDED(Result)) {
        DebugExtensionUninitialize();
        return Result;
    }

    dprintf("[Island] Initialize Success.\n");

    return Result;
}

VOID
CALLBACK
DebugExtensionNotify(
    __in ULONG Notify,
    __in ULONG64 Argument
)
{
    switch (Notify)
    {
    case DEBUG_NOTIFY_SESSION_ACTIVE:
        break;
    case DEBUG_NOTIFY_SESSION_INACTIVE:
        break;
    case DEBUG_NOTIFY_SESSION_ACCESSIBLE:
        break;
    case DEBUG_NOTIFY_SESSION_INACCESSIBLE:
        break;
    default:
        break;
    }
}

HRESULT
CALLBACK
BreakEntry(
    __in PDEBUG_CLIENT Client,
    __in PCSTR Args
)
{
    CommandExecute(".reload");

    MmLoadSystemImage = GetOffsetByName("MmLoadSystemImage");
    MmLoadSystemImageEx = GetOffsetByName("MmLoadSystemImageEx");

    if (MmLoadSystemImageEx) {
        CommandExecute("bc *");

        AddDebugBreakPoint(
            MmLoadSystemImageEx,
            DEBUG_BREAKPOINT_ENABLED | DEBUG_BREAKPOINT_ONE_SHOT);

        CommandExecute("g");
    }
    else if (MmLoadSystemImage) {
        CommandExecute("bc *");

        AddDebugBreakPoint(
            MmLoadSystemImage,
            DEBUG_BREAKPOINT_ENABLED | DEBUG_BREAKPOINT_ONE_SHOT);

        CommandExecute("g");
    }

    return S_OK;
}