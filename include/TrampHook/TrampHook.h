#pragma once

#include <cstdint>

namespace TrampHook
{
    //
    // [SECTION] Types
    //

    struct Hook
    {
        void *p_trampoline;
        size_t size_of_overwritten_code;
    };

    //
    // [SECTION] Functions
    //

    void *hook(void *p_target, void *p_detour);
    void unhook(void *p_target);
    void unhookAll();

    std::uint8_t getInstructionSize(std::uint8_t *p_opcode);
}