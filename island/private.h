#ifndef _PRIVATE_H_
#define _PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

    PVOID
        NTAPI
        RtlAllocateHeap(
            IN PVOID HeapHandle,
            IN ULONG Flags,
            IN SIZE_T Size
        );

    BOOLEAN
        NTAPI
        RtlFreeHeap(
            IN PVOID HeapHandle,
            IN ULONG Flags,
            IN PVOID BaseAddress
        );

#define NtCurrentPeb() (NtCurrentTeb()->ProcessEnvironmentBlock)
#define RtlProcessHeap() (NtCurrentPeb()->Reserved4[1])

#define RinHeapAlloc(Size) RtlAllocateHeap(RtlProcessHeap(), HEAP_ZERO_MEMORY, Size)
#define RinHeapFree(Pointer) RtlFreeHeap(RtlProcessHeap(), 0, Pointer)

#ifndef _DEBUG
#define RinDbgBreak() printf("[error] %s:%d\n", __FUNCTION__, __LINE__); getchar()
#else
#define RinDbgBreak() __debugbreak()
#endif


#ifdef __cplusplus
}
#endif

#endif
