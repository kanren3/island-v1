#include "defs.h"
#include "common.h"
#include "handler.h"
#include "emulator.h"

EMULATOR_REGION EmulatorRegion = { 0 };

ULONG
WINAPI
UcLoadGdtr(
    __in PVOID Gdt
)
{
    uc_err uc_state = UC_ERR_OK;
    uc_x86_mmr virtual_gdtr = { 0 };

#ifndef _WIN64
    virtual_gdtr.base = 0xC0000000;
    virtual_gdtr.limit = KGDT64_LAST - 1;
#else
    virtual_gdtr.base = 0xFFFFF00000000000;
    virtual_gdtr.limit = KGDT64_LAST - 1;
#endif

    uc_state = uc_mem_map(
        EmulatorRegion.uc_handle,
        virtual_gdtr.base,
        0x1000,
        UC_PROT_WRITE | UC_PROT_READ);

    uc_state = uc_mem_write(
        EmulatorRegion.uc_handle,
        virtual_gdtr.base,
        Gdt,
        KGDT64_LAST);

    uc_state = uc_reg_write(
        EmulatorRegion.uc_handle,
        UC_X86_REG_GDTR,
        &virtual_gdtr);

    return uc_state;
}

ULONG
WINAPI
UcReadMsr(
    __in LONG RegisterId,
    __out PULONG64 Value
)
{
    uc_err uc_state = UC_ERR_OK;
    uc_x86_msr virtual_msr = { 0 };

    virtual_msr.rid = RegisterId;

    uc_state = uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_MSR, &virtual_msr);

    *Value = virtual_msr.value;

    return uc_state;
}

ULONG
WINAPI
UcWriteMsr(
    __in LONG RegisterId,
    __in ULONG64 Value
)
{
    uc_err uc_state = UC_ERR_OK;
    uc_x86_msr virtual_msr = { 0 };

    virtual_msr.rid = RegisterId;
    virtual_msr.value = Value;

    uc_state = uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_MSR, &virtual_msr);

    return uc_state;
}

ULONG
WINAPI
UcReadRegister(
    __in LONG RegisterId,
    __out PVOID Value
)
{
    uc_err uc_state = UC_ERR_OK;

    uc_state = uc_reg_read(EmulatorRegion.uc_handle, RegisterId, Value);

    return uc_state;
}

ULONG
WINAPI
UcWriteRegister(
    __in LONG RegisterId,
    __in PVOID Value
)
{
    uc_err uc_state = UC_ERR_OK;

    uc_state = uc_reg_write(EmulatorRegion.uc_handle, RegisterId, Value);

    return uc_state;
}

ULONG
WINAPI
UcMapMemory(
    __in ULONG_PTR Address,
    __in SIZE_T Size,
    __in ULONG Protect
)
{
    uc_err uc_state = UC_ERR_OK;

    uc_state = uc_mem_map(
        EmulatorRegion.uc_handle,
        Address,
        Size,
        Protect);

    return uc_state;
}

ULONG
WINAPI
UcMapMemoryFromPtr(
    __in ULONG_PTR Address,
    __in PVOID Buffer,
    __in SIZE_T Size,
    __in ULONG Protect
)
{
    size_t len = 0;
    uc_err uc_state = UC_ERR_OK;

    if (Size < USN_PAGE_SIZE) {
        len = USN_PAGE_SIZE;
    }
    else {
        len = Size / USN_PAGE_SIZE * USN_PAGE_SIZE;
        len += Size % USN_PAGE_SIZE ? USN_PAGE_SIZE : 0;
    }

    uc_state = uc_mem_map_ptr(
        EmulatorRegion.uc_handle,
        Address,
        len,
        Protect,
        Buffer);

    return uc_state;
}

ULONG
WINAPI
UcUnmapMemory(
    __in ULONG_PTR Address,
    __in SIZE_T Size
)
{
    uc_err uc_state = UC_ERR_OK;

    uc_state = uc_mem_unmap(
        EmulatorRegion.uc_handle,
        Address,
        Size);

    return uc_state;
}

ULONG
WINAPI
UcReadMemory(
    __in ULONG_PTR Address,
    __out PVOID Buffer,
    __in SIZE_T Size
)
{
    uc_err uc_state = UC_ERR_OK;

    uc_state = uc_mem_read(
        EmulatorRegion.uc_handle,
        Address,
        Buffer,
        Size);

    return uc_state;
}

ULONG
WINAPI
UcWriteMemory(
    __in ULONG_PTR Address,
    __in PVOID Buffer,
    __in SIZE_T Size
)
{
    uc_err uc_state = UC_ERR_OK;

    uc_state = uc_mem_write(
        EmulatorRegion.uc_handle,
        Address,
        Buffer,
        Size);

    return uc_state;
}

ULONG
WINAPI
UcReadyEmulatorGdtr(
    VOID
)
{
    ULONG UcState = UC_ERR_OK;
    PVOID EmulatorGdt = NULL;
    ULONG EmulatorSelector = 0;

    EmulatorGdt = RinHeapAlloc(KGDT64_LAST);

    BuildGdtEntry(EmulatorGdt, KGDT64_NULL, 0, 0, 0, 0, FALSE, FALSE);
    BuildGdtEntry(EmulatorGdt, KGDT64_R0_CODE, 0, 0, TYPE_CODE, DPL_SYSTEM, TRUE, FALSE);
    BuildGdtEntry(EmulatorGdt, KGDT64_R0_DATA, 0, 0, TYPE_DATA, DPL_SYSTEM, FALSE, TRUE);
    BuildGdtEntry(EmulatorGdt, KGDT64_R3_CODE, 0, 0, TYPE_CODE, DPL_USER, TRUE, FALSE);
    BuildGdtEntry(EmulatorGdt, KGDT64_R3_CMCODE, 0, 0xFFFFFFFF, TYPE_CODE, DPL_USER, FALSE, TRUE);
    BuildGdtEntry(EmulatorGdt, KGDT64_R3_DATA, 0, 0xFFFFFFFF, TYPE_DATA, DPL_USER, FALSE, TRUE);
    BuildGdtEntry(EmulatorGdt, KGDT64_R3_CMTEB, 0, 0xFFF, TYPE_DATA, DPL_USER, FALSE, TRUE);

    if (UC_ERR_OK == UcLoadGdtr(EmulatorGdt)) {
#ifndef _WIN64
        EmulatorSelector = KGDT64_R3_CMCODE | DPL_USER;
        UcState = UcWriteRegister(UC_X86_REG_CS, &EmulatorSelector);
#else
        EmulatorSelector = KGDT64_R3_CODE | DPL_USER;
        UcState = UcWriteRegister(UC_X86_REG_CS, &EmulatorSelector);
#endif
        EmulatorSelector = KGDT64_R3_DATA | DPL_USER;
        UcState = UcWriteRegister(UC_X86_REG_SS, &EmulatorSelector);
        UcState = UcWriteRegister(UC_X86_REG_DS, &EmulatorSelector);
        UcState = UcWriteRegister(UC_X86_REG_ES, &EmulatorSelector);
        UcState = UcWriteRegister(UC_X86_REG_GS, &EmulatorSelector);

        EmulatorSelector = KGDT64_R3_CMTEB | DPL_USER;
        UcState = UcWriteRegister(UC_X86_REG_FS, &EmulatorSelector);
    }

    RinHeapFree(EmulatorGdt);
    return UcState;
}

VOID
WINAPI
UcSaveContext(
    __out PCONTEXT Context
)
{
#ifndef _WIN64
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_EAX, &Context->Eax);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_EBX, &Context->Ebx);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_ECX, &Context->Ecx);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_EDX, &Context->Edx);

    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_ESI, &Context->Esi);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_EDI, &Context->Edi);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_ESP, &Context->Esp);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_EBP, &Context->Ebp);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_EFLAGS, &Context->EFlags);
#else
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_RAX, &Context->Rax);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_RBX, &Context->Rbx);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_RCX, &Context->Rcx);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_RDX, &Context->Rdx);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_R8, &Context->R8);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_R9, &Context->R9);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_R10, &Context->R10);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_R11, &Context->R11);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_R12, &Context->R12);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_R13, &Context->R13);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_R14, &Context->R14);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_R15, &Context->R15);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_RSI, &Context->Rsi);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_RDI, &Context->Rdi);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_RSP, &Context->Rsp);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_RBP, &Context->Rbp);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_EFLAGS, &Context->EFlags);

    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM0, &Context->Xmm0);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM1, &Context->Xmm1);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM2, &Context->Xmm2);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM3, &Context->Xmm3);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM4, &Context->Xmm4);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM5, &Context->Xmm5);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM6, &Context->Xmm6);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM7, &Context->Xmm7);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM8, &Context->Xmm8);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM9, &Context->Xmm9);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM10, &Context->Xmm10);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM11, &Context->Xmm11);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM12, &Context->Xmm12);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM13, &Context->Xmm13);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM14, &Context->Xmm14);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_XMM15, &Context->Xmm15);
    uc_reg_read(EmulatorRegion.uc_handle, UC_X86_REG_MXCSR, &Context->MxCsr);
#endif
}

VOID
WINAPI
UcLoadContext(
    __in PCONTEXT Context
)
{
#ifndef _WIN64
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_EAX, &Context->Eax);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_EBX, &Context->Ebx);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_ECX, &Context->Ecx);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_EDX, &Context->Edx);

    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_ESI, &Context->Esi);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_EDI, &Context->Edi);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_ESP, &Context->Esp);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_EBP, &Context->Ebp);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_EFLAGS, &Context->EFlags);
#else
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_RAX, &Context->Rax);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_RBX, &Context->Rbx);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_RCX, &Context->Rcx);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_RDX, &Context->Rdx);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_R8, &Context->R8);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_R9, &Context->R9);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_R10, &Context->R10);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_R11, &Context->R11);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_R12, &Context->R12);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_R13, &Context->R13);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_R14, &Context->R14);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_R15, &Context->R15);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_RSI, &Context->Rsi);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_RDI, &Context->Rdi);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_RSP, &Context->Rsp);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_RBP, &Context->Rbp);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_EFLAGS, &Context->EFlags);

    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM0, &Context->Xmm0);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM1, &Context->Xmm1);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM2, &Context->Xmm2);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM3, &Context->Xmm3);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM4, &Context->Xmm4);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM5, &Context->Xmm5);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM6, &Context->Xmm6);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM7, &Context->Xmm7);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM8, &Context->Xmm8);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM9, &Context->Xmm9);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM10, &Context->Xmm10);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM11, &Context->Xmm11);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM12, &Context->Xmm12);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM13, &Context->Xmm13);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM14, &Context->Xmm14);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_XMM15, &Context->Xmm15);
    uc_reg_write(EmulatorRegion.uc_handle, UC_X86_REG_MXCSR, &Context->MxCsr);
#endif
}

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
)
{
    uc_err uc_state = UC_ERR_OK;

    if (NULL != CodeCallback) {
        uc_state = uc_hook_add(EmulatorRegion.uc_handle,
            &EmulatorRegion.code_hook, UC_HOOK_CODE, uc_code_hook, CodeCallback, 1, 0);
    }
    else {
        uc_hook_del(EmulatorRegion.uc_handle, EmulatorRegion.code_hook);
    }

    if (NULL != IntrCallback) {
        uc_state = uc_hook_add(EmulatorRegion.uc_handle,
            &EmulatorRegion.intr_hook, UC_HOOK_INTR, uc_intr_hook, IntrCallback, 1, 0);
    }
    else {
        uc_hook_del(EmulatorRegion.uc_handle, EmulatorRegion.intr_hook);
    }

    if (NULL != CpuidCallback) {
        uc_state = uc_hook_add(EmulatorRegion.uc_handle,
            &EmulatorRegion.cpuid_hook, UC_HOOK_INSN, uc_insn_hook_cpuid, CpuidCallback, 1, 0, UC_X86_INS_CPUID);
    }
    else {
        uc_hook_del(EmulatorRegion.uc_handle, EmulatorRegion.cpuid_hook);
    }

    if (NULL != SyscallCallback) {
        uc_state = uc_hook_add(EmulatorRegion.uc_handle,
            &EmulatorRegion.syscall_hook, UC_HOOK_INSN, uc_insn_hook_syscall, SyscallCallback, 1, 0, UC_X86_INS_SYSCALL);
    }
    else {
        uc_hook_del(EmulatorRegion.uc_handle, EmulatorRegion.syscall_hook);
    }

    if (NULL != InCallback) {
        uc_state = uc_hook_add(EmulatorRegion.uc_handle,
            &EmulatorRegion.in_hook, UC_HOOK_INSN, uc_insn_hook_in, InCallback, 1, 0, UC_X86_INS_IN);
    }
    else {
        uc_hook_del(EmulatorRegion.uc_handle, EmulatorRegion.in_hook);
    }

    if (NULL != OutCallback) {
        uc_state = uc_hook_add(EmulatorRegion.uc_handle,
            &EmulatorRegion.out_hook, UC_HOOK_INSN, uc_insn_hook_out, OutCallback, 1, 0, UC_X86_INS_OUT);
    }
    else {
        uc_hook_del(EmulatorRegion.uc_handle, EmulatorRegion.out_hook);
    }

    if (NULL != MemCallback) {
        uc_state = uc_hook_add(EmulatorRegion.uc_handle,
            &EmulatorRegion.mem_hook,
            UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED | UC_HOOK_MEM_FETCH_UNMAPPED,
            uc_mem_hook, MemCallback, 1, 0);
    }
    else {
        uc_hook_del(EmulatorRegion.uc_handle, EmulatorRegion.mem_hook);
    }

    return uc_state;
}

ULONG
WINAPI
UcEmulatorStart(
    __in ULONG64 Begin,
    __in_opt ULONG Count
)
{
    uc_err uc_state = UC_ERR_OK;

    uc_state = uc_emu_start(EmulatorRegion.uc_handle, Begin, ~1, 0, Count);

    return uc_state;
}

ULONG
WINAPI
UcEmulatorStop(
    VOID
)
{
    return uc_emu_stop(EmulatorRegion.uc_handle);
}


VOID
WINAPI
UninitializeEmulator(
    VOID
)
{
    cs_err CsState = CS_ERR_OK;
    uc_err UcState = UC_ERR_OK;

    if (0 != EmulatorRegion.cs_handle) {
        CsState = cs_close(&EmulatorRegion.cs_handle);
        EmulatorRegion.cs_handle = 0;
    }

    if (NULL != EmulatorRegion.uc_handle) {
        UcState = uc_close(EmulatorRegion.uc_handle);
        EmulatorRegion.uc_handle = NULL;
    }

    if (CS_ERR_OK != CsState || UC_ERR_OK != UcState) {
        RinDbgBreak();
    }
}

VOID
WINAPI
InitializeEmulator(
    VOID
)
{
    cs_err CsState = CS_ERR_OK;
    uc_err UcState = UC_ERR_OK;

    if (0 == EmulatorRegion.cs_handle) {
#ifndef _WIN64
        CsState = cs_open(CS_ARCH_X86, CS_MODE_32, &EmulatorRegion.cs_handle);
#else
        CsState = cs_open(CS_ARCH_X86, CS_MODE_64, &EmulatorRegion.cs_handle);
#endif
    }
    
    if (NULL == EmulatorRegion.uc_handle) {
#ifndef _WIN64
        UcState = uc_open(UC_ARCH_X86, UC_MODE_32, &EmulatorRegion.uc_handle);
#else
        UcState = uc_open(UC_ARCH_X86, UC_MODE_64, &EmulatorRegion.uc_handle);
#endif
    }

    if (CS_ERR_OK != CsState || UC_ERR_OK != UcState) {
        RinDbgBreak();
    }
}

VOID
WINAPI
DisasmPrint(
    IN ULONG64 Address
)
{
    cs_insn insn = { 0 };
    uint8_t* code;
    size_t size;
    uint64_t address;

    UCHAR Code[0x10] = { 0 };

    uc_mem_read(EmulatorRegion.uc_handle, Address, Code, sizeof(Code));

    code = Code;
    size = sizeof(Code);
    address = Address;

    cs_disasm_iter(EmulatorRegion.cs_handle, &code, &size, &address, &insn);

    if (strstr(insn.mnemonic, "cpuid") ||
        strstr(insn.mnemonic, "str") ||
        strstr(insn.mnemonic, "idt") ||
        strstr(insn.mnemonic, "gdt") ||
        strstr(insn.mnemonic, "msr")) {
        dprintf("address:%I64X\t\t\t%s\t\t%s\n", insn.address, insn.mnemonic, insn.op_str);
    }
}