#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#define KGDT64_NULL (0 * 16)            // NULL descriptor
#define KGDT64_R0_CODE (1 * 16)         // kernel mode 64-bit code
#define KGDT64_R0_DATA (1 * 16) + 8     // kernel mode 64-bit data (stack)
#define KGDT64_R3_CMCODE (2 * 16)       // user mode 32-bit code
#define KGDT64_R3_DATA (2 * 16) + 8     // user mode 32-bit data
#define KGDT64_R3_CODE (3 * 16)         // user mode 64-bit code
#define KGDT64_SYS_TSS (4 * 16)         // kernel mode system task state
#define KGDT64_R3_CMTEB (5 * 16)        // user mode 32-bit TEB
#define KGDT64_R0_CMCODE (6 * 16)       // kernel mode 32-bit code
#define KGDT64_LAST (7 * 16)            // last entry

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

#define TYPE_CODE 0x1A
#define TYPE_DATA 0x12
#define TYPE_TSS64 0x09

#define DPL_USER 3
#define DPL_SYSTEM 0

#define GRANULARITY_BYTE 0
#define GRANULARITY_PAGE 1

    typedef union _KGDT_BASE {
        struct {
            USHORT BaseLow;
            UCHAR BaseMiddle;
            UCHAR BaseHigh;
            ULONG BaseUpper;
        };

        ULONG64 Base;
    } KGDT_BASE, * PKGDT_BASE;

    typedef union _KGDT_LIMIT {
        struct {
            USHORT LimitLow;
            USHORT LimitHigh : 4;
            USHORT MustBeZero : 12;
        };

        ULONG Limit;
    } KGDT_LIMIT, * PKGDT_LIMIT;

    typedef union _KGDTENTRY64 {
        struct {
            USHORT LimitLow;
            USHORT BaseLow;
            union {
                struct {
                    UCHAR BaseMiddle;
                    UCHAR Flags1;
                    UCHAR Flags2;
                    UCHAR BaseHigh;
                } Bytes;

                struct {
                    ULONG BaseMiddle : 8;
                    ULONG Type : 5;
                    ULONG Dpl : 2;
                    ULONG Present : 1;
                    ULONG LimitHigh : 4;
                    ULONG System : 1;
                    ULONG LongMode : 1;
                    ULONG DefaultBig : 1;
                    ULONG Granularity : 1;
                    ULONG BaseHigh : 8;
                } Bits;
            };

            ULONG BaseUpper;
            ULONG MustBeZero;
        };

        ULONG64 Alignment;
    } KGDTENTRY64, * PKGDTENTRY64;

    typedef struct _KDESCRIPTOR {
        USHORT Pad[3];
        USHORT Limit;
        PVOID Base;
    } KDESCRIPTOR, * PKDESCRIPTOR;

    VOID
        WINAPI
        BuildGdtEntry(
            __out PVOID Gdt,
            __in ULONG Selector,
            __in ULONGLONG Base,
            __in ULONGLONG Limit,
            __in ULONG Type,
            __in ULONG Dpl,
            __in BOOLEAN LongMode,
            __in BOOLEAN DefaultBig
        );

#define GetValueFormPointer(Value,Pointer) \
    RtlCopyMemory(&Value, Pointer, sizeof(Value))

    typedef enum _DEBUG_REG_TYPE {
        DEBUG_REG_RAX,
        DEBUG_REG_RCX,
        DEBUG_REG_RDX,
        DEBUG_REG_RBX,
        DEBUG_REG_RSP,
        DEBUG_REG_RBP,
        DEBUG_REG_RSI,
        DEBUG_REG_RDI,
        DEBUG_REG_R8,
        DEBUG_REG_R9,
        DEBUG_REG_R10,
        DEBUG_REG_R11,
        DEBUG_REG_R12,
        DEBUG_REG_R13,
        DEBUG_REG_R14,
        DEBUG_REG_R15,
        DEBUG_REG_RIP,
        DEBUG_REG_EFL,
        DEBUG_REG_CS,
        DEBUG_REG_DS,
        DEBUG_REG_ES,
        DEBUG_REG_FS,
        DEBUG_REG_GS,
        DEBUG_REG_SS,
        DEBUG_REG_DR0,
        DEBUG_REG_DR1,
        DEBUG_REG_DR2,
        DEBUG_REG_DR3,
        DEBUG_REG_DR6,
        DEBUG_REG_DR7,
        DEBUG_REG_FPCW,
        DEBUG_REG_FPSW,
        DEBUG_REG_FPTW,
        DEBUG_REG_ST0,
        DEBUG_REG_ST1,
        DEBUG_REG_ST2,
        DEBUG_REG_ST3,
        DEBUG_REG_ST4,
        DEBUG_REG_ST5,
        DEBUG_REG_ST6,
        DEBUG_REG_ST7,
        DEBUG_REG_MM0,
        DEBUG_REG_MM1,
        DEBUG_REG_MM2,
        DEBUG_REG_MM3,
        DEBUG_REG_MM4,
        DEBUG_REG_MM5,
        DEBUG_REG_MM6,
        DEBUG_REG_MM7,
        DEBUG_REG_MXCSR,
        DEBUG_REG_XMM0,
        DEBUG_REG_XMM1,
        DEBUG_REG_XMM2,
        DEBUG_REG_XMM3,
        DEBUG_REG_XMM4,
        DEBUG_REG_XMM5,
        DEBUG_REG_XMM6,
        DEBUG_REG_XMM7,
        DEBUG_REG_XMM8,
        DEBUG_REG_XMM9,
        DEBUG_REG_XMM10,
        DEBUG_REG_XMM11,
        DEBUG_REG_XMM12,
        DEBUG_REG_XMM13,
        DEBUG_REG_XMM14,
        DEBUG_REG_XMM15,
        DEBUG_REG_XMM0_0,
        DEBUG_REG_XMM0_1,
        DEBUG_REG_XMM0_2,
        DEBUG_REG_XMM0_3,
        DEBUG_REG_XMM1_0,
        DEBUG_REG_XMM1_1,
        DEBUG_REG_XMM1_2,
        DEBUG_REG_XMM1_3,
        DEBUG_REG_XMM2_0,
        DEBUG_REG_XMM2_1,
        DEBUG_REG_XMM2_2,
        DEBUG_REG_XMM2_3,
        DEBUG_REG_XMM3_0,
        DEBUG_REG_XMM3_1,
        DEBUG_REG_XMM3_2,
        DEBUG_REG_XMM3_3,
        DEBUG_REG_XMM4_0,
        DEBUG_REG_XMM4_1,
        DEBUG_REG_XMM4_2,
        DEBUG_REG_XMM4_3,
        DEBUG_REG_XMM5_0,
        DEBUG_REG_XMM5_1,
        DEBUG_REG_XMM5_2,
        DEBUG_REG_XMM5_3,
        DEBUG_REG_XMM6_0,
        DEBUG_REG_XMM6_1,
        DEBUG_REG_XMM6_2,
        DEBUG_REG_XMM6_3,
        DEBUG_REG_XMM7_0,
        DEBUG_REG_XMM7_1,
        DEBUG_REG_XMM7_2,
        DEBUG_REG_XMM7_3,
        DEBUG_REG_XMM8_0,
        DEBUG_REG_XMM8_1,
        DEBUG_REG_XMM8_2,
        DEBUG_REG_XMM8_3,
        DEBUG_REG_XMM9_0,
        DEBUG_REG_XMM9_1,
        DEBUG_REG_XMM9_2,
        DEBUG_REG_XMM9_3,
        DEBUG_REG_XMM10_0,
        DEBUG_REG_XMM10_1,
        DEBUG_REG_XMM10_2,
        DEBUG_REG_XMM10_3,
        DEBUG_REG_XMM11_0,
        DEBUG_REG_XMM11_1,
        DEBUG_REG_XMM11_2,
        DEBUG_REG_XMM11_3,
        DEBUG_REG_XMM12_0,
        DEBUG_REG_XMM12_1,
        DEBUG_REG_XMM12_2,
        DEBUG_REG_XMM12_3,
        DEBUG_REG_XMM13_0,
        DEBUG_REG_XMM13_1,
        DEBUG_REG_XMM13_2,
        DEBUG_REG_XMM13_3,
        DEBUG_REG_XMM14_0,
        DEBUG_REG_XMM14_1,
        DEBUG_REG_XMM14_2,
        DEBUG_REG_XMM14_3,
        DEBUG_REG_XMM15_0,
        DEBUG_REG_XMM15_1,
        DEBUG_REG_XMM15_2,
        DEBUG_REG_XMM15_3,
        DEBUG_REG_XMM0L,
        DEBUG_REG_XMM1L,
        DEBUG_REG_XMM2L,
        DEBUG_REG_XMM3L,
        DEBUG_REG_XMM4L,
        DEBUG_REG_XMM5L,
        DEBUG_REG_XMM6L,
        DEBUG_REG_XMM7L,
        DEBUG_REG_XMM8L,
        DEBUG_REG_XMM9L,
        DEBUG_REG_XMM10L,
        DEBUG_REG_XMM11L,
        DEBUG_REG_XMM12L,
        DEBUG_REG_XMM13L,
        DEBUG_REG_XMM14L,
        DEBUG_REG_XMM15L,
        DEBUG_REG_XMM0H,
        DEBUG_REG_XMM1H,
        DEBUG_REG_XMM2H,
        DEBUG_REG_XMM3H,
        DEBUG_REG_XMM4H,
        DEBUG_REG_XMM5H,
        DEBUG_REG_XMM6H,
        DEBUG_REG_XMM7H,
        DEBUG_REG_XMM8H,
        DEBUG_REG_XMM9H,
        DEBUG_REG_XMM10H,
        DEBUG_REG_XMM11H,
        DEBUG_REG_XMM12H,
        DEBUG_REG_XMM13H,
        DEBUG_REG_XMM14H,
        DEBUG_REG_XMM15H,
        DEBUG_REG_YMM0,
        DEBUG_REG_YMM1,
        DEBUG_REG_YMM2,
        DEBUG_REG_YMM3,
        DEBUG_REG_YMM4,
        DEBUG_REG_YMM5,
        DEBUG_REG_YMM6,
        DEBUG_REG_YMM7,
        DEBUG_REG_YMM8,
        DEBUG_REG_YMM9,
        DEBUG_REG_YMM10,
        DEBUG_REG_YMM11,
        DEBUG_REG_YMM12,
        DEBUG_REG_YMM13,
        DEBUG_REG_YMM14,
        DEBUG_REG_YMM15,
        DEBUG_REG_YMM0_0,
        DEBUG_REG_YMM0_1,
        DEBUG_REG_YMM0_2,
        DEBUG_REG_YMM0_3,
        DEBUG_REG_YMM1_0,
        DEBUG_REG_YMM1_1,
        DEBUG_REG_YMM1_2,
        DEBUG_REG_YMM1_3,
        DEBUG_REG_YMM2_0,
        DEBUG_REG_YMM2_1,
        DEBUG_REG_YMM2_2,
        DEBUG_REG_YMM2_3,
        DEBUG_REG_YMM3_0,
        DEBUG_REG_YMM3_1,
        DEBUG_REG_YMM3_2,
        DEBUG_REG_YMM3_3,
        DEBUG_REG_YMM4_0,
        DEBUG_REG_YMM4_1,
        DEBUG_REG_YMM4_2,
        DEBUG_REG_YMM4_3,
        DEBUG_REG_YMM5_0,
        DEBUG_REG_YMM5_1,
        DEBUG_REG_YMM5_2,
        DEBUG_REG_YMM5_3,
        DEBUG_REG_YMM6_0,
        DEBUG_REG_YMM6_1,
        DEBUG_REG_YMM6_2,
        DEBUG_REG_YMM6_3,
        DEBUG_REG_YMM7_0,
        DEBUG_REG_YMM7_1,
        DEBUG_REG_YMM7_2,
        DEBUG_REG_YMM7_3,
        DEBUG_REG_YMM8_0,
        DEBUG_REG_YMM8_1,
        DEBUG_REG_YMM8_2,
        DEBUG_REG_YMM8_3,
        DEBUG_REG_YMM9_0,
        DEBUG_REG_YMM9_1,
        DEBUG_REG_YMM9_2,
        DEBUG_REG_YMM9_3,
        DEBUG_REG_YMM10_0,
        DEBUG_REG_YMM10_1,
        DEBUG_REG_YMM10_2,
        DEBUG_REG_YMM10_3,
        DEBUG_REG_YMM11_0,
        DEBUG_REG_YMM11_1,
        DEBUG_REG_YMM11_2,
        DEBUG_REG_YMM11_3,
        DEBUG_REG_YMM12_0,
        DEBUG_REG_YMM12_1,
        DEBUG_REG_YMM12_2,
        DEBUG_REG_YMM12_3,
        DEBUG_REG_YMM13_0,
        DEBUG_REG_YMM13_1,
        DEBUG_REG_YMM13_2,
        DEBUG_REG_YMM13_3,
        DEBUG_REG_YMM14_0,
        DEBUG_REG_YMM14_1,
        DEBUG_REG_YMM14_2,
        DEBUG_REG_YMM14_3,
        DEBUG_REG_YMM15_0,
        DEBUG_REG_YMM15_1,
        DEBUG_REG_YMM15_2,
        DEBUG_REG_YMM15_3,
        DEBUG_REG_YMM0L,
        DEBUG_REG_YMM1L,
        DEBUG_REG_YMM2L,
        DEBUG_REG_YMM3L,
        DEBUG_REG_YMM4L,
        DEBUG_REG_YMM5L,
        DEBUG_REG_YMM6L,
        DEBUG_REG_YMM7L,
        DEBUG_REG_YMM8L,
        DEBUG_REG_YMM9L,
        DEBUG_REG_YMM10L,
        DEBUG_REG_YMM11L,
        DEBUG_REG_YMM12L,
        DEBUG_REG_YMM13L,
        DEBUG_REG_YMM14L,
        DEBUG_REG_YMM15L,
        DEBUG_REG_YMM0H,
        DEBUG_REG_YMM1H,
        DEBUG_REG_YMM2H,
        DEBUG_REG_YMM3H,
        DEBUG_REG_YMM4H,
        DEBUG_REG_YMM5H,
        DEBUG_REG_YMM6H,
        DEBUG_REG_YMM7H,
        DEBUG_REG_YMM8H,
        DEBUG_REG_YMM9H,
        DEBUG_REG_YMM10H,
        DEBUG_REG_YMM11H,
        DEBUG_REG_YMM12H,
        DEBUG_REG_YMM13H,
        DEBUG_REG_YMM14H,
        DEBUG_REG_YMM15H,
        DEBUG_REG_ZMM0,
        DEBUG_REG_ZMM1,
        DEBUG_REG_ZMM2,
        DEBUG_REG_ZMM3,
        DEBUG_REG_ZMM4,
        DEBUG_REG_ZMM5,
        DEBUG_REG_ZMM6,
        DEBUG_REG_ZMM7,
        DEBUG_REG_ZMM8,
        DEBUG_REG_ZMM9,
        DEBUG_REG_ZMM10,
        DEBUG_REG_ZMM11,
        DEBUG_REG_ZMM12,
        DEBUG_REG_ZMM13,
        DEBUG_REG_ZMM14,
        DEBUG_REG_ZMM15,
        DEBUG_REG_ZMM16,
        DEBUG_REG_ZMM17,
        DEBUG_REG_ZMM18,
        DEBUG_REG_ZMM19,
        DEBUG_REG_ZMM20,
        DEBUG_REG_ZMM21,
        DEBUG_REG_ZMM22,
        DEBUG_REG_ZMM23,
        DEBUG_REG_ZMM24,
        DEBUG_REG_ZMM25,
        DEBUG_REG_ZMM26,
        DEBUG_REG_ZMM27,
        DEBUG_REG_ZMM28,
        DEBUG_REG_ZMM29,
        DEBUG_REG_ZMM30,
        DEBUG_REG_ZMM31,
        DEBUG_REG_K0,
        DEBUG_REG_K1,
        DEBUG_REG_K2,
        DEBUG_REG_K3,
        DEBUG_REG_K4,
        DEBUG_REG_K5,
        DEBUG_REG_K6,
        DEBUG_REG_K7,
        DEBUG_REG_ZMM0H,
        DEBUG_REG_ZMM1H,
        DEBUG_REG_ZMM2H,
        DEBUG_REG_ZMM3H,
        DEBUG_REG_ZMM4H,
        DEBUG_REG_ZMM5H,
        DEBUG_REG_ZMM6H,
        DEBUG_REG_ZMM7H,
        DEBUG_REG_ZMM8H,
        DEBUG_REG_ZMM9H,
        DEBUG_REG_ZMM10H,
        DEBUG_REG_ZMM11H,
        DEBUG_REG_ZMM12H,
        DEBUG_REG_ZMM13H,
        DEBUG_REG_ZMM14H,
        DEBUG_REG_ZMM15H,
        DEBUG_REG_EXFROM,
        DEBUG_REG_EXTO,
        DEBUG_REG_BRFROM,
        DEBUG_REG_BRTO,
        DEBUG_REG_EAX,
        DEBUG_REG_ECX,
        DEBUG_REG_EDX,
        DEBUG_REG_EBX,
        DEBUG_REG_ESP,
        DEBUG_REG_EBP,
        DEBUG_REG_ESI,
        DEBUG_REG_EDI,
        DEBUG_REG_R8D,
        DEBUG_REG_R9D,
        DEBUG_REG_R10D,
        DEBUG_REG_R11D,
        DEBUG_REG_R12D,
        DEBUG_REG_R13D,
        DEBUG_REG_R14D,
        DEBUG_REG_R15D,
        DEBUG_REG_EIP,
        DEBUG_REG_AX,
        DEBUG_REG_CX,
        DEBUG_REG_DX,
        DEBUG_REG_BX,
        DEBUG_REG_SP,
        DEBUG_REG_BP,
        DEBUG_REG_SI,
        DEBUG_REG_DI,
        DEBUG_REG_R8W,
        DEBUG_REG_R9W,
        DEBUG_REG_R10W,
        DEBUG_REG_R11W,
        DEBUG_REG_R12W,
        DEBUG_REG_R13W,
        DEBUG_REG_R14W,
        DEBUG_REG_R15W,
        DEBUG_REG_IP,
        DEBUG_REG_FL,
        DEBUG_REG_AL,
        DEBUG_REG_CL,
        DEBUG_REG_DL,
        DEBUG_REG_BL,
        DEBUG_REG_SPL,
        DEBUG_REG_BPL,
        DEBUG_REG_SIL,
        DEBUG_REG_DIL,
        DEBUG_REG_R8B,
        DEBUG_REG_R9B,
        DEBUG_REG_R10B,
        DEBUG_REG_R11B,
        DEBUG_REG_R12B,
        DEBUG_REG_R13B,
        DEBUG_REG_R14B,
        DEBUG_REG_R15B,
        DEBUG_REG_AH,
        DEBUG_REG_CH,
        DEBUG_REG_DH,
        DEBUG_REG_BH,
        DEBUG_REG_IOPL,
        DEBUG_REG_OF,
        DEBUG_REG_DF,
        DEBUG_REG_IF,
        DEBUG_REG_TF,
        DEBUG_REG_SF,
        DEBUG_REG_ZF,
        DEBUG_REG_AF,
        DEBUG_REG_PF,
        DEBUG_REG_CF,
        DEBUG_REG_VIP,
        DEBUG_REG_VIF,
        DEBUG_REG_SSP,
        DEBUG_REG_CETUMSR,
        DEBUG_REG_CR0,
        DEBUG_REG_CR2,
        DEBUG_REG_CR3,
        DEBUG_REG_CR4,
        DEBUG_REG_CR8,
        DEBUG_REG_GDTR,
        DEBUG_REG_GDTL,
        DEBUG_REG_IDTR,
        DEBUG_REG_IDTL,
        DEBUG_REG_TR,
        DEBUG_REG_LDTR,
        DEBUG_REG_KMXCSR,
        DEBUG_REG_KDR0,
        DEBUG_REG_KDR1,
        DEBUG_REG_KDR2,
        DEBUG_REG_KDR3,
        DEBUG_REG_KDR6,
        DEBUG_REG_KDR7,
        DEBUG_REG_XCR0
    }DEBUG_REG_TYPE;

    VOID
        WINAPI
        CommandExecute(
            __in PCSTR Command
        );

    ULONG64
        WINAPI
        GetOffsetByName(
            __in PCSTR Symbol
        );

    HRESULT
        WINAPI
        GetRegisterValue(
            __in DEBUG_REG_TYPE Register,
            __in PDEBUG_VALUE Value
        );

    VOID
        WINAPI
        GetDebugContext(
            __out PCONTEXT Context
        );

    ULONG64
        WINAPI
        GetImageFromContext(
            __in PCONTEXT Context,
            __out PULONG SizeOfImage
        );

#ifdef __cplusplus
}
#endif

#endif
