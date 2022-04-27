#include "defs.h"
#include "common.h"

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
)
{
    KGDT_BASE GdtBase = { 0 };
    KGDT_LIMIT GdtLimit = { 0 };
    PKGDTENTRY64 GdtEntry = NULL;

    GdtEntry = (PKGDTENTRY64)((PUCHAR)Gdt + (Selector & ~7));

    GdtBase.Base = Base;
    GdtEntry->BaseLow = GdtBase.BaseLow;
    GdtEntry->Bits.BaseMiddle = GdtBase.BaseMiddle;
    GdtEntry->Bits.BaseHigh = GdtBase.BaseHigh;

    if (FALSE != LongMode) {
        GdtEntry->MustBeZero = 0;
        GdtEntry->BaseUpper = GdtBase.BaseUpper;
        GdtEntry->Bits.LongMode = 1;
    }
    else {
        GdtEntry->Bits.LongMode = 0;
    }

    if (Limit > (1 << 20)) {
        GdtLimit.Limit = (ULONG)(Limit / 0x1000);
        GdtEntry->Bits.Granularity = 1;
    }
    else {
        GdtLimit.Limit = (ULONG)Limit;
        GdtEntry->Bits.Granularity = 0;
    }

    GdtEntry->LimitLow = GdtLimit.LimitLow;
    GdtEntry->Bits.LimitHigh = GdtLimit.LimitHigh;

    if (Type == 0) {
        GdtEntry->Bits.Present = 0;
    }
    else {
        GdtEntry->Bits.Present = 1;
    }

    GdtEntry->Bits.Type = Type;
    GdtEntry->Bits.Dpl = Dpl;
    GdtEntry->Bits.DefaultBig = DefaultBig;
    GdtEntry->Bits.System = 0;
}