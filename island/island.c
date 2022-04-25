#include "defs.h"
#include "common.h"
#include "emulator.h"
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
Entry(
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

VOID
WINAPI
CodeCallback(
    ULONG64 Address,
    ULONG Size
)
{

}

BOOLEAN
WINAPI
MemCallback(
    ULONG Type,
    ULONG64 Address,
    ULONG Size,
    ULONG64 Value
)
{
    HRESULT Result = S_OK;
    PVOID UnknownPage = NULL;
    CHAR SymbolsName[MAX_PATH] = { 0 };
    ULONG64 RspValue = 0;
    ULONG64 RetAddress = 0;

    Result = DebugSymbols->lpVtbl->GetNameByOffset(
        DebugSymbols,
        Address,
        SymbolsName,
        sizeof(SymbolsName),
        NULL,
        NULL);

    if (S_OK == Result) {
        dprintf("[Island] Type:%d Address:%I64X %s\n", Type, Address, SymbolsName);
    }
    else {
        dprintf("[Island] Type:%d Address:%I64X\n", Type, Address);
    }

    if (UC_MEM_READ_UNMAPPED == Type) {
        UnknownPage = RinHeapAlloc(USN_PAGE_SIZE);
        ReadMemory(Address & ~0xFFF, UnknownPage, USN_PAGE_SIZE, NULL);
        UcMapMemoryFromPtr(Address & ~0xFFF, UnknownPage, USN_PAGE_SIZE, UC_PROT_ALL);
    }

    if (UC_MEM_WRITE_UNMAPPED == Type) {
        UnknownPage = RinHeapAlloc(USN_PAGE_SIZE);
        ReadMemory(Address & ~0xFFF, UnknownPage, USN_PAGE_SIZE, NULL);
        UcMapMemoryFromPtr(Address & ~0xFFF, UnknownPage, USN_PAGE_SIZE, UC_PROT_ALL);
    }

    if (UC_MEM_FETCH_UNMAPPED == Type) {
        UcReadRegister(UC_X86_REG_RSP, &RspValue);
        UcReadMemory(RspValue, &RetAddress, sizeof(RetAddress));
        UcWriteRegister(UC_X86_REG_RIP, &RetAddress);
        AddDebugBreakPoint(RetAddress, DEBUG_BREAKPOINT_ENABLED | DEBUG_BREAKPOINT_ONE_SHOT);
        CommandExecute("g");
        return FALSE;
    }

    return TRUE;
}

HRESULT
CALLBACK
Trace(
    __in PDEBUG_CLIENT Client,
    __in PCSTR Args
)
{
    CONTEXT DebugContext = { 0 };
    ULONG64 GsBase = 0;
    ULONG64 ImageBase = 0;
    ULONG SizeOfImage = 0;
    PVOID ImageDumpBase = 0;
    SHORT TaskRegister = 0x40;

    InitializeEmulator();

    if (UC_ERR_OK == UcReadyEmulatorGdtr()) {
        UcWriteRegister(UC_X86_REG_TR, &TaskRegister);
        GetDebugContext(&DebugContext);
        UcLoadContext(&DebugContext);

        ReadMsr(IA32_GS_BASE, &GsBase);
        UcWriteMsr(IA32_GS_BASE, GsBase);

        ImageBase = GetImageFromContext(&DebugContext, &SizeOfImage);

        if (0 != ImageBase) {
            ImageDumpBase = RinHeapAlloc(SizeOfImage);
            ReadMemory(ImageBase, ImageDumpBase, SizeOfImage, NULL);

            UcMapMemoryFromPtr(
                ImageBase,
                ImageDumpBase,
                SizeOfImage,
                UC_PROT_ALL);

            UcSetCallback(
                NULL,
                NULL, NULL, NULL, NULL, NULL,
                MemCallback);

            UcEmulatorStart(DebugContext.Rip, 0);
        }
        else {
            dprintf("[Island] Please reload symbols\n");
        }
    }

    UninitializeEmulator();
    return S_OK;
}