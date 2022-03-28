#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

	HRESULT
		WINAPI
		InitializeDebugEvent(
			VOID
		);

	PDEBUG_BREAKPOINT
		WINAPI
		AddDebugBreakPoint(
			__in ULONG64 Address,
			__in ULONG Flags
		);

	HRESULT
		WINAPI
		RemoveDebugBreakPoint(
			__in PDEBUG_BREAKPOINT Breakpoint
		);

#ifdef __cplusplus
}
#endif

#endif
