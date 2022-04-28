#ifndef _EMULATOR_H_
#define _EMULATOR_H_

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _EMULATOR_REGION {
        csh cs_handle;
        uc_engine* uc_handle;
        uc_hook code_hook;
        uc_hook intr_hook;
        uc_hook cpuid_hook;
        uc_hook syscall_hook;
        uc_hook in_hook;
        uc_hook out_hook;
        uc_hook mem_hook;
    }EMULATOR_REGION, * PEMULATOR_REGION;

    ULONG
        WINAPI
        UcReadMsr(
            __in LONG RegisterId,
            __out PULONG64 Value
        );

    ULONG
        WINAPI
        UcWriteMsr(
            __in LONG RegisterId,
            __in ULONG64 Value
        );

    ULONG
        WINAPI
        UcReadRegister(
            __in LONG RegisterId,
            __out PVOID Value
        );

    ULONG
        WINAPI
        UcWriteRegister(
            __in LONG RegisterId,
            __in PVOID Value
        );

    ULONG
        WINAPI
        UcMapMemory(
            __in ULONG64 Address,
            __in SIZE_T Size,
            __in ULONG Protect
        );

    ULONG
        WINAPI
        UcMapMemoryFromPtr(
            __in ULONG64 Address,
            __in PVOID Buffer,
            __in SIZE_T Size,
            __in ULONG Protect
        );

    ULONG
        WINAPI
        UcUnmapMemory(
            __in ULONG64 Address,
            __in SIZE_T Size
        );

    ULONG
        WINAPI
        UcReadMemory(
            __in ULONG64 Address,
            __out PVOID Buffer,
            __in SIZE_T Size
        );

    ULONG
        WINAPI
        UcWriteMemory(
            __in ULONG64 Address,
            __in PVOID Buffer,
            __in SIZE_T Size
        );

    VOID
        WINAPI
        UcSaveContext(
            __out PCONTEXT Context
        );

    VOID
        WINAPI
        UcLoadContext(
            __in PCONTEXT Context
        );

    ULONG
        WINAPI
        UcReadyEmulatorGdtr(
            VOID
        );

    typedef VOID(WINAPI* UC_CALLBACK_CODE)(ULONG64 Address, ULONG Size);
    typedef VOID(WINAPI* UC_CALLBACK_INTR)(ULONG IntrNumber);
    typedef VOID(WINAPI* UC_CALLBACK_CPUID)();
    typedef VOID(WINAPI* UC_CALLBACK_SYSCALL)();
    typedef VOID(WINAPI* UC_CALLBACK_IN)(ULONG Port, ULONG Size);
    typedef VOID(WINAPI* UC_CALLBACK_OUT)(ULONG Port, ULONG Size, ULONG Value);
    typedef BOOLEAN(WINAPI* UC_CALLBACK_MEM)(ULONG Type, ULONG64 Address, ULONG Size, ULONG64 Value);

    ULONG
        WINAPI
        UcSetCallback(
            __in_opt UC_CALLBACK_CODE CodeCallback,
            __in_opt UC_CALLBACK_INTR IntrCallback,
            __in_opt UC_CALLBACK_CPUID CpuidCallback,
            __in_opt UC_CALLBACK_SYSCALL SyscallCallback,
            __in_opt UC_CALLBACK_IN InCallback,
            __in_opt UC_CALLBACK_OUT OutCallback,
            __in_opt UC_CALLBACK_MEM MemCallback
        );

    ULONG
        WINAPI
        UcEmulatorStart(
            __in ULONG64 Begin,
            __in_opt ULONG Count
        );

    ULONG
        WINAPI
        UcEmulatorStop(
            VOID
        );

    VOID
        WINAPI
        UninitializeEmulator(
            VOID
        );

    VOID
        WINAPI
        InitializeEmulator(
            VOID
        );

    VOID
        WINAPI
        DisasmPrint(
            IN ULONG64 Address
        );

    BOOLEAN
        WINAPI
        EmulatorMemoryNotify(
            ULONG Type,
            ULONG64 Address,
            ULONG Size,
            ULONG64 Value
        );

#ifdef __cplusplus
}
#endif

#endif
