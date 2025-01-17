#include <map>

#include <Windows.h>
#include <TrampHook/TrampHook.h>

//
// [SECTION] Variables & constants
//

#if _WIN64
constexpr std::uint8_t minimum_hook_size{12};
#else
constexpr std::uint8_t minimum_hook_size{5};
#endif

std::map<std::uintptr_t, TrampHook::Hook> hooked_functions;

//
// [SECTION] Functions
//

void *TrampHook::hook(void *p_target, void *p_detour)
{
    if (p_target == nullptr || p_detour == nullptr)
        return nullptr;

    /* Check that the function isn't already hooked */
    if (hooked_functions.find(reinterpret_cast<std::uintptr_t>(p_target)) != hooked_functions.end())
        return nullptr;

    Hook hook;
    uint8_t total_size_to_overwrite{};
    uint8_t *p_opcode{static_cast<uint8_t *>(p_target)};

    /* Iterate over instructions until required size is found */
    while (total_size_to_overwrite < minimum_hook_size)
    {
        uint8_t instruction_size{getInstructionSize(p_opcode)};

        if (!instruction_size)
            return nullptr;

        total_size_to_overwrite += instruction_size;
        p_opcode += instruction_size;
    }

    hook.size_of_overwritten_code = total_size_to_overwrite;
    hook.p_trampoline = VirtualAlloc(nullptr, total_size_to_overwrite + minimum_hook_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (!hook.p_trampoline)
        return nullptr;

    memcpy(hook.p_trampoline, p_target, total_size_to_overwrite);

    uint8_t *p_jump_back{(uint8_t *)(hook.p_trampoline) + total_size_to_overwrite};
    uint8_t *p_normal_code{(uint8_t *)p_target + total_size_to_overwrite};

#if _WIN64
    // MOV RAX, p_normal_code
    *(uint16_t *)p_jump_back = 0x48B8;
    *(uint64_t *)(p_jump_back + 2) = (uint64_t)p_normal_code;

    // JMP RAX
    *(uint16_t *)(p_jump_back + 10) = 0xFFE0;
#else
    uint32_t relative_jmp_address{(uintptr_t)p_normal_code - (uint32_t)(p_jump_back + minimum_hook_size)};

    // JMP relative_jmp_address
    *p_jump_back = 0xE9;
    *(uint32_t *)(p_jump_back + 1) = relative_jmp_address;
#endif

    DWORD old_protect;
    VirtualProtect(p_target, total_size_to_overwrite, PAGE_EXECUTE_READWRITE, &old_protect);
    memset(p_target, '\x90', total_size_to_overwrite);

#if _WIN64
    // MOV RAX, p_detour
    *(uint16_t *)p_target = 0x48B8;
    *(uint64_t *)((uint8_t *)p_target + 2) = (uint64_t)p_detour;

    // JMP RAX
    *(uint16_t *)(p_jump_back + 10) = 0xFFE0;
#else
    *(uint8_t *)(p_target) = 0xE9;
    *(int32_t *)((uint8_t *)p_target + 1) = (int32_t)((uint8_t *)p_detour - ((uint8_t *)p_target + 5));
#endif

    VirtualProtect(p_target, total_size_to_overwrite, old_protect, &old_protect);

    hooked_functions[(uintptr_t)p_target] = hook;

    return hook.p_trampoline;
}

void TrampHook::unhook(void *p_target)
{
    if (!p_target)
        return;

    uintptr_t target_addr = (uintptr_t)p_target;
    auto it = hooked_functions.find(target_addr);

    if (it == hooked_functions.end())
        return;

    Hook &hook = it->second;

    DWORD old_protect;
    VirtualProtect(p_target, hook.size_of_overwritten_code, PAGE_EXECUTE_READWRITE, &old_protect);

    memcpy(p_target, hook.p_trampoline, hook.size_of_overwritten_code);

    VirtualProtect(p_target, hook.size_of_overwritten_code, old_protect, &old_protect);

    VirtualFree(hook.p_trampoline, 0, MEM_RELEASE);

    hooked_functions.erase(it);
}

void TrampHook::unhookAll()
{
    if (hooked_functions.empty())
        return;

    for (const auto &hook_pair : hooked_functions)
    {
        uintptr_t address = hook_pair.first;
        const Hook &hook = hook_pair.second;

        DWORD old_protect;
        VirtualProtect((void *)address, hook.size_of_overwritten_code, PAGE_EXECUTE_READWRITE, &old_protect);

        if (hook.p_trampoline)
            memcpy((void *)address, hook.p_trampoline, hook.size_of_overwritten_code);

        VirtualProtect((void *)address, hook.size_of_overwritten_code, old_protect, &old_protect);

        if (hook.p_trampoline)
            VirtualFree(hook.p_trampoline, 0, MEM_RELEASE);
    }

    hooked_functions.clear();
}

std::uint8_t TrampHook::getInstructionSize(std::uint8_t *p_opcode)
{
    return 0;
}
