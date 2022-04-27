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

#ifdef __cplusplus
}
#endif

#endif
