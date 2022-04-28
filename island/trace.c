#include "defs.h"
#include "helper.h"
#include "debug.h"
#include "emulator.h"
#include "trace.h"

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

                    TraceNotify(Context.Rip);

                    UcReadMemory(
                        Context.Rsp,
                        &DebugBlock.TraceAddress,
                        sizeof(DebugBlock.TraceAddress));

                    AddDebugBreakPoint(
                        DebugBlock.TraceAddress,
                        DEBUG_BREAKPOINT_ENABLED | DEBUG_BREAKPOINT_ONE_SHOT,
                        DebugBlock.TraceThreadId);

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