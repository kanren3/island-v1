#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdio.h>
#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <capstone/capstone.h>
#include <unicorn/unicorn.h>
#include <winternl.h>

#ifndef NOTHING
#define NOTHING
#endif

#ifndef _WIN64
#define KDEXT_32BIT
#else
#define KDEXT_64BIT
#endif
#include <initguid.h>
#include <wdbgexts.h>
#include <dbgeng.h>

#include "private.h"

#ifdef __cplusplus
extern "C" {
#endif

    enum {
        IA32_P5_MC_ADDR = 0x0,
        IA32_P5_MC_TYPE = 0x1,
        IA32_TSC = 0x10,
        IA32_PLATFORM_ID = 0x17,
        IA32_APIC_BASE = 0x1b,
        IA32_EBC_HARD_POWERON = 0x2a,
        IA32_EBC_SOFT_POWERON = 0x2b,
        IA32_EBC_FREQUENCY_ID = 0x2c,
        IA32_FEATURE_CONTROL = 0x3a,
        IA32_THERM_DIODE_OFFSET = 0x3f,
        IA32_BIOS_UPDT_TRIG = 0x79,
        IA32_BIOS_SIGN_ID = 0x8b,
        IA32_SMM_MONITOR_CTL = 0x9b,
        IA32_PMC0 = 0xc1,
        IA32_PMC1 = 0xc2,
        IA32_PMC2 = 0xc3,
        IA32_PMC3 = 0xc4,
        IA32_FSB_FREQ = 0xcd,
        IA32_MPERF = 0xe7,
        IA32_APERF = 0xe8,
        IA32_TEMP_TARGET = 0xee,
        IA32_MTRRCAP = 0xfe,
        IA32_BBL_CR_CTL3 = 0x11e,
        IA32_SYSENTER_CS = 0x174,
        IA32_SYSENTER_ESP = 0x175,
        IA32_SYSENTER_EIP = 0x176,
        IA32_MCG_CAP = 0x179,
        IA32_MCG_STATUS = 0x17a,
        IA32_MCG_CTL = 0x17b,
        IA32_PERFEVTSEL0 = 0x186,
        IA32_PERFEVTSEL1 = 0x187,
        IA32_PERFEVTSEL2 = 0x188,
        IA32_PERFEVTSEL3 = 0x189,
        IA32_PERF_CTL = 0x199,
        IA32_MISC_ENABLE = 0x1a0,
        IA32_DEBUGCTL = 0x1d9,
        IA32_MTRR_PHYSBASE0 = 0x200,
        IA32_MTRR_PHYSMASK0 = 0x201,
        IA32_MTRR_PHYSBASE1 = 0x202,
        IA32_MTRR_PHYSMASK1 = 0x203,
        IA32_MTRR_PHYSBASE2 = 0x204,
        IA32_MTRR_PHYSMASK2 = 0x205,
        IA32_MTRR_PHYSBASE3 = 0x206,
        IA32_MTRR_PHYSMASK3 = 0x207,
        IA32_MTRR_PHYSBASE4 = 0x208,
        IA32_MTRR_PHYSMASK4 = 0x209,
        IA32_MTRR_PHYSBASE5 = 0x20a,
        IA32_MTRR_PHYSMASK5 = 0x20b,
        IA32_MTRR_PHYSBASE6 = 0x20c,
        IA32_MTRR_PHYSMASK6 = 0x20d,
        IA32_MTRR_PHYSBASE7 = 0x20e,
        IA32_MTRR_PHYSMASK7 = 0x20f,
        IA32_MTRR_PHYSBASE8 = 0x210,
        IA32_MTRR_PHYSMASK8 = 0x211,
        IA32_MTRR_PHYSBASE9 = 0x212,
        IA32_MTRR_PHYSMASK9 = 0x213,
        MTRRFIX64K_00000 = 0x250,
        MTRRFIX16K_80000 = 0x258,
        MTRRFIX16K_A0000 = 0x259,
        MTRRFIX4K_C0000 = 0x268,
        MTRRFIX4K_F8000 = 0x26f,
        IA32_CR_PAT = 0x277,
        IA32_MC0_CTL2 = 0x280,
        IA32_MC1_CTL2 = 0x281,
        IA32_MC2_CTL2 = 0x282,
        IA32_MC3_CTL2 = 0x283,
        IA32_MC4_CTL2 = 0x284,
        IA32_MC5_CTL2 = 0x285,
        IA32_MC6_CTL2 = 0x286,
        IA32_MC7_CTL2 = 0x287,
        IA32_MC8_CTL2 = 0x288,
        IA32_MTRR_DEF_TYPE = 0x2ff,
        MSR_BPU_COUNTER0 = 0x300,
        IA32_FIXED_CTR0 = 0x309,
        IA32_FIXED_CTR1 = 0x30a,
        IA32_FIXED_CTR2 = 0x30b,
        IA32_PERF_CAPABILITIES = 0x345,
        MSR_PEBS_MATRIX_VERT = 0x3f2,
        IA32_FIXED_CTR_CTRL = 0x38d,
        IA32_PERF_GLOBAL_STATUS = 0x38e,
        IA32_PERF_GLOBAL_CTRL = 0x38f,
        IA32_PERF_GLOBAL_OVF_CTRL = 0x390,
        IA32_MC0_CTL = 0x400,
        IA32_MC0_STATUS = 0x401,
        IA32_MC0_ADDR = 0x402,
        IA32_MC0_MISC = 0x403,
        IA32_CPUID_FEATURE_MASK = 0x478,
        IA32_VMX_BASIC = 0x480,
        IA32_VMX_PINBASED_CTLS = 0x481,
        IA32_VMX_PROCBASED_CTLS = 0x482,
        IA32_VMX_EXIT_CTLS = 0x483,
        IA32_VMX_ENTRY_CTLS = 0x484,
        IA32_VMX_MISC = 0x485,
        IA32_VMX_CR0_FIXED0 = 0x486,
        IA32_VMX_CR0_FIXED1 = 0x487,
        IA32_VMX_CR4_FIXED0 = 0x488,
        IA32_VMX_CR4_FIXED1 = 0x489,
        IA32_VMX_VMCS_ENUM = 0x48a,
        IA32_VMX_SECONDARY_CTLS = 0x48b,
        IA32_VMX_EPT_VPID_CAP = 0x48c,
        IA32_VMX_TRUE_PINBASED_CTLS = 0x48d,
        IA32_VMX_TRUE_PROCBASED_CTLS = 0x48e,
        IA32_VMX_TRUE_EXIT_CTLS = 0x48f,
        IA32_VMX_TRUE_ENTRY_CTLS = 0x490,
        IA32_EFER = 0xc0000080,
        IA32_STAR = 0xc0000081,
        IA32_LSTAR = 0xc0000082,
        IA32_CSTAR = 0xc0000083,
        IA32_SF_MASK = 0xc0000084,
        IA32_FS_BASE = 0xc0000100,
        IA32_GS_BASE = 0xc0000101,
        IA32_KERNEL_GS_BASE = 0xc0000102,
        IA32_TSC_AUX = 0xc0000103
    };

	extern PDEBUG_CLIENT DebugClient;
	extern PDEBUG_CONTROL DebugControl;
	extern PDEBUG_REGISTERS DebugRegisters;
	extern PDEBUG_SYMBOLS DebugSymbols;
	extern PDEBUG_SYSTEM_OBJECTS DebugSystemObjects;
	extern PDEBUG_DATA_SPACES DebugDataSpaces;

	typedef struct _DEBUG_BLOCK {
		ULONG64 MmLoadSystemImage;
		ULONG64 MmLoadSystemImageEx;
		ULONG64 LoadImageRetAddress;
        ULONG64 LoadImageStack;

        ULONG64 TraceImageBase;
        ULONG TraceSizeOfImage;
        ULONG TraceThreadId;
        ULONG64 TraceAddress;
	}DEBUG_BLOCK, * PDEBUG_BLOCK;

    typedef struct _CPU_STATE {
        CONTEXT Context;
    }CPU_STATE, * PCPU_STATE;

	extern DEBUG_BLOCK DebugBlock;

#ifdef __cplusplus
}
#endif

#endif
