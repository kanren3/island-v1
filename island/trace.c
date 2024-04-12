#include "defs.h"
#include "helper.h"
#include "debug.h"
#include "emulator.h"
#include "trace.h"

HRESULT
CALLBACK
Emulator(
    __in PDEBUG_CLIENT Client,
    __in PCSTR Args
)
{
    ULONG UcState = UC_ERR_OK;
    HRESULT Result = S_OK;
    CONTEXT Context = { 0 };
    USHORT TaskRegister = 0x40;
    ULONG64 GsBase = 0;
    PVOID ImageSnapshoot = NULL;

    if (S_OK == Result) {
        CommandExecute("bc *");
        CommandExecute(".reload");

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
                    UninitializeEmulator();
                }
            }
            else {
                dprintf("[Island] Please reload symbols\n");
            }
        }

        UninitializeEmulator();
    }
    else {
        dprintf("[Island] ThreadId acquire failed\n");
    }

    return S_OK;
}

HRESULT
CALLBACK
TraceToBefore(
    __in PDEBUG_CLIENT Client,
    __in PCSTR Args
)
{
    ULONG UcState = UC_ERR_OK;
    HRESULT Result = S_OK;
    CONTEXT Context = { 0 };
    USHORT TaskRegister = 0x40;
    ULONG64 GsBase = 0;
    PVOID ImageSnapshoot = NULL;
    CHAR SymbolsName[MAX_PATH] = { 0 };

    memset(DebugBlock.TraceFunction, 0, MAX_PATH);
    strcpy_s(DebugBlock.TraceFunction, MAX_PATH, Args);

    DebugBlock.TraceType = TraceTypeBefore;

    Result = DebugSystemObjects->lpVtbl->GetCurrentThreadId(
        DebugSystemObjects,
        &DebugBlock.TraceThreadId);

    if (S_OK == Result) {
        CommandExecute("bc *");
        CommandExecute(".reload");

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

                    UninitializeEmulator();
                    CommandExecute("g");
                }
            }
            else {
                dprintf("[Island] Please reload symbols\n");
            }
        }

        UninitializeEmulator();
    }
    else {
        dprintf("[Island] ThreadId acquire failed\n");
    }

    return S_OK;
}

HRESULT
CALLBACK
TraceToAfter(
    __in PDEBUG_CLIENT Client,
    __in PCSTR Args
)
{
    ULONG UcState = UC_ERR_OK;
    HRESULT Result = S_OK;
    CONTEXT Context = { 0 };
    USHORT TaskRegister = 0x40;
    ULONG64 GsBase = 0;
    PVOID ImageSnapshoot = NULL;
    CHAR SymbolsName[MAX_PATH] = { 0 };
    ULONG64 ReturnAddress;

    memset(DebugBlock.TraceFunction, 0, MAX_PATH);
    strcpy_s(DebugBlock.TraceFunction, MAX_PATH, Args);

    DebugBlock.TraceType = TraceTypeAfter;

    Result = DebugSystemObjects->lpVtbl->GetCurrentThreadId(
        DebugSystemObjects,
        &DebugBlock.TraceThreadId);

    if (S_OK == Result) {
        CommandExecute("bc *");
        CommandExecute(".reload");

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

                    UninitializeEmulator();
                    CommandExecute("g");
                }
            }
            else {
                dprintf("[Island] Please reload symbols\n");
            }
        }

        UninitializeEmulator();
    }
    else {
        dprintf("[Island] ThreadId acquire failed\n");
    }

    return S_OK;
}

HRESULT
CALLBACK
Trace(
    __in PDEBUG_CLIENT Client,
    __in PCSTR Args
)
{
    ULONG UcState = UC_ERR_OK;
    HRESULT Result = S_OK;
    CONTEXT Context = { 0 };
    USHORT TaskRegister = 0x40;
    ULONG64 GsBase = 0;
    PVOID ImageSnapshoot = NULL;

    Result = DebugSystemObjects->lpVtbl->GetCurrentThreadId(
        DebugSystemObjects,
        &DebugBlock.TraceThreadId);

    if (S_OK == Result) {
        CommandExecute("bc *");
        CommandExecute(".reload");

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

                    UcReadMemory(
                        Context.Rsp,
                        &DebugBlock.TraceAddress,
                        sizeof(DebugBlock.TraceAddress));

                    DebugBlock.OnceBreakpoint = AddDebugBreakPoint(DebugBlock.TraceAddress);

                    UninitializeEmulator();
                    CommandExecute("g");
                }
            }
            else {
                dprintf("[Island] Please reload symbols\n");
            }
        }

        UninitializeEmulator();
    }
    else {
        dprintf("[Island] ThreadId acquire failed\n");
    }

    return S_OK;
}