#ifndef _HANDLER_H_
#define _HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

	void uc_code_hook(uc_engine* uc, uint64_t address, uint32_t size, void* user_data);
	void uc_intr_hook(uc_engine* uc, uint32_t intno, void* user_data);
	int uc_insn_hook_cpuid(uc_engine* uc, void* user_data);
	void uc_insn_hook_syscall(uc_engine* uc, void* user_data);
	uint32_t uc_insn_hook_in(uc_engine* uc, uint32_t port, int size, void* user_data);
	uint32_t uc_insn_hook_out(uc_engine* uc, uint32_t port, int size, uint32_t value, void* user_data);
	bool uc_mem_hook(uc_engine* uc, uc_mem_type type, uint64_t address, int size, int64_t value, void* user_data);

#ifdef __cplusplus
}
#endif

#endif
