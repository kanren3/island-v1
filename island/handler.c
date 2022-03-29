#include "defs.h"
#include "emulator.h"
#include "handler.h"

void uc_code_hook(uc_engine* uc, uint64_t address, uint32_t size, void* user_data)
{
	UC_CALLBACK_CODE Callback = user_data;
	Callback(address, size);
}

void uc_intr_hook(uc_engine* uc, uint32_t intno, void* user_data)
{
	UC_CALLBACK_INTR Callback = user_data;
	Callback(intno);
}

int uc_insn_hook_cpuid(uc_engine* uc, void* user_data)
{
	UC_CALLBACK_CPUID Callback = user_data;
	Callback();
	return 1;
}

void uc_insn_hook_syscall(uc_engine* uc, void* user_data)
{
	UC_CALLBACK_SYSCALL Callback = user_data;
	Callback();
}

uint32_t uc_insn_hook_in(uc_engine* uc, uint32_t port, int size, void* user_data)
{
	UC_CALLBACK_IN Callback = user_data;
	Callback(port, size);
	return 0;
}

uint32_t uc_insn_hook_out(uc_engine* uc, uint32_t port, int size, uint32_t value, void* user_data)
{
	UC_CALLBACK_OUT Callback = user_data;
	Callback(port, size, value);
	return 0;
}

bool uc_mem_hook(uc_engine* uc, uc_mem_type type, uint64_t address, int size, int64_t value, void* user_data)
{
	UC_CALLBACK_MEM Callback = user_data;
	return Callback(type, address, size, value);
}