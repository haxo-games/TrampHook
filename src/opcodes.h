#pragma once

#include <cstdint>

//
// [SECTION] Types & defines
//

enum InstructionType : std::uint8_t
{
    INSTR_INVALID = 0,
    INSTR_SIMPLE,
    INSTR_CONTROL,
    INSTR_ARITHMETIC,
    INSTR_DATA_MOV,
    INSTR_STACK,
    INSTR_2BYTE,
    INSTR_SYSTEM
};

enum ImmediateSize : std::uint8_t
{
    IMM_NONE = 0,
    IMM_8,
    IMM_16,
    IMM_32,
    IMM_64,
    IMM_16_8
};

#define SIMPLE(type) {type, 0, IMM_NONE, 0, 0}
#define WITH_MODRM(type) {type, 1, IMM_NONE, 0, 0}
#define WITH_IMM8(type) {type, 0, IMM_8, 0, 0}
#define WITH_IMM32(type) {type, 0, IMM_32, 0, 0}
#define WITH_MODRM_IMM8(type) {type, 1, IMM_8, 0, 0}
#define WITH_MODRM_IMM32(type) {type, 1, IMM_32, 0, 0}
#define INVALID {INSTR_INVALID, 0, IMM_NONE, 0, 0}
#define WITH_IMM16(type) {type, 0, IMM_16, 0, 0}
#define WITH_IMM16_IMM8(type) {type, 0, IMM_16_8, 0, 0}

struct InstructionInfo
{
    std::uint8_t type;
    std::uint8_t has_modrm;
    std::uint8_t imm_size;
    std::uint8_t prefix;
    std::uint8_t rex_w;
};

//
// [SECTION] Variables
//

const InstructionInfo opcodes[256] =
    {
        // 0x00-0x0F: Arithmetic and two-byte instructions
        WITH_MODRM(INSTR_ARITHMETIC),     // 00 - ADD r/m8, r8
        WITH_MODRM(INSTR_ARITHMETIC),     // 01 - ADD r/m32, r32
        WITH_MODRM(INSTR_ARITHMETIC),     // 02 - ADD r8, r/m8
        WITH_MODRM(INSTR_ARITHMETIC),     // 03 - ADD r32, r/m32
        WITH_IMM8(INSTR_ARITHMETIC),      // 04 - ADD AL, imm8
        WITH_IMM32(INSTR_ARITHMETIC),     // 05 - ADD EAX, imm32
        INVALID,                          // 06
        INVALID,                          // 07
        WITH_MODRM(INSTR_ARITHMETIC),     // 08 - OR r/m8, r8
        WITH_MODRM(INSTR_ARITHMETIC),     // 09 - OR r/m32, r32
        WITH_MODRM(INSTR_ARITHMETIC),     // 0A - OR r8, r/m8
        WITH_MODRM(INSTR_ARITHMETIC),     // 0B - OR r32, r/m32
        WITH_IMM8(INSTR_ARITHMETIC),      // 0C - OR AL, imm8
        WITH_IMM32(INSTR_ARITHMETIC),     // 0D - OR EAX, imm32
        INVALID,                          // 0E
        {INSTR_2BYTE, 0, IMM_NONE, 0, 0}, // 0F - Two-byte instruction prefix

        // 0x10-0x1F: Arithmetic (ADC) instructions
        WITH_MODRM(INSTR_ARITHMETIC), // 10 - ADC r/m8, r8
        WITH_MODRM(INSTR_ARITHMETIC), // 11 - ADC r/m32, r32
        WITH_MODRM(INSTR_ARITHMETIC), // 12 - ADC r8, r/m8
        WITH_MODRM(INSTR_ARITHMETIC), // 13 - ADC r32, r/m32
        WITH_IMM8(INSTR_ARITHMETIC),  // 14 - ADC AL, imm8
        WITH_IMM32(INSTR_ARITHMETIC), // 15 - ADC EAX, imm32
        INVALID,                      // 16
        INVALID,                      // 17
        WITH_MODRM(INSTR_ARITHMETIC), // 18 - SBB r/m8, r8
        WITH_MODRM(INSTR_ARITHMETIC), // 19 - SBB r/m32, r32
        WITH_MODRM(INSTR_ARITHMETIC), // 1A - SBB r8, r/m8
        WITH_MODRM(INSTR_ARITHMETIC), // 1B - SBB r32, r/m32
        WITH_IMM8(INSTR_ARITHMETIC),  // 1C - SBB AL, imm8
        WITH_IMM32(INSTR_ARITHMETIC), // 1D - SBB EAX, imm32
        INVALID,                      // 1E
        INVALID,                      // 1F

        // 0x20-0x2F: Arithmetic (AND) instructions
        WITH_MODRM(INSTR_ARITHMETIC), // 20 - AND r/m8, r8
        WITH_MODRM(INSTR_ARITHMETIC), // 21 - AND r/m32, r32
        WITH_MODRM(INSTR_ARITHMETIC), // 22 - AND r8, r/m8
        WITH_MODRM(INSTR_ARITHMETIC), // 23 - AND r32, r/m32
        WITH_IMM8(INSTR_ARITHMETIC),  // 24 - AND AL, imm8
        WITH_IMM32(INSTR_ARITHMETIC), // 25 - AND EAX, imm32
        INVALID,                      // 26 - ES segment override (invalid in 64-bit)
        INVALID,                      // 27
        WITH_MODRM(INSTR_ARITHMETIC), // 28 - SUB r/m8, r8
        WITH_MODRM(INSTR_ARITHMETIC), // 29 - SUB r/m32, r32
        WITH_MODRM(INSTR_ARITHMETIC), // 2A - SUB r8, r/m8
        WITH_MODRM(INSTR_ARITHMETIC), // 2B - SUB r32, r/m32
        WITH_IMM8(INSTR_ARITHMETIC),  // 2C - SUB AL, imm8
        WITH_IMM32(INSTR_ARITHMETIC), // 2D - SUB EAX, imm32
        INVALID,                      // 2E - CS segment override (invalid in 64-bit)
        INVALID,                      // 2F

        // 0x30-0x3F: Arithmetic (XOR) instructions
        WITH_MODRM(INSTR_ARITHMETIC), // 30 - XOR r/m8, r8
        WITH_MODRM(INSTR_ARITHMETIC), // 31 - XOR r/m32, r32
        WITH_MODRM(INSTR_ARITHMETIC), // 32 - XOR r8, r/m8
        WITH_MODRM(INSTR_ARITHMETIC), // 33 - XOR r32, r/m32
        WITH_IMM8(INSTR_ARITHMETIC),  // 34 - XOR AL, imm8
        WITH_IMM32(INSTR_ARITHMETIC), // 35 - XOR EAX, imm32
        INVALID,                      // 36 - SS segment override (invalid in 64-bit)
        INVALID,                      // 37
        WITH_MODRM(INSTR_ARITHMETIC), // 38 - CMP r/m8, r8
        WITH_MODRM(INSTR_ARITHMETIC), // 39 - CMP r/m32, r32
        WITH_MODRM(INSTR_ARITHMETIC), // 3A - CMP r8, r/m8
        WITH_MODRM(INSTR_ARITHMETIC), // 3B - CMP r32, r/m32
        WITH_IMM8(INSTR_ARITHMETIC),  // 3C - CMP AL, imm8
        WITH_IMM32(INSTR_ARITHMETIC), // 3D - CMP EAX, imm32
        INVALID,                      // 3E - DS segment override (invalid in 64-bit)
        INVALID,                      // 3F

        // 0x40-0x4F: REX prefixes in x64 mode, INC/DEC in x86 mode
        SIMPLE(INSTR_ARITHMETIC), // 40 - INC eax / REX prefix
        SIMPLE(INSTR_ARITHMETIC), // 41 - INC ecx / REX.B prefix
        SIMPLE(INSTR_ARITHMETIC), // 42 - INC edx / REX.X prefix
        SIMPLE(INSTR_ARITHMETIC), // 43 - INC ebx / REX.XB prefix
        SIMPLE(INSTR_ARITHMETIC), // 44 - INC esp / REX.R prefix
        SIMPLE(INSTR_ARITHMETIC), // 45 - INC ebp / REX.RB prefix
        SIMPLE(INSTR_ARITHMETIC), // 46 - INC esi / REX.RX prefix
        SIMPLE(INSTR_ARITHMETIC), // 47 - INC edi / REX.RXB prefix
        SIMPLE(INSTR_ARITHMETIC), // 48 - DEC eax / REX.W prefix
        SIMPLE(INSTR_ARITHMETIC), // 49 - DEC ecx / REX.WB prefix
        SIMPLE(INSTR_ARITHMETIC), // 4A - DEC edx / REX.WX prefix
        SIMPLE(INSTR_ARITHMETIC), // 4B - DEC ebx / REX.WXB prefix
        SIMPLE(INSTR_ARITHMETIC), // 4C - DEC esp / REX.WR prefix
        SIMPLE(INSTR_ARITHMETIC), // 4D - DEC ebp / REX.WRB prefix
        SIMPLE(INSTR_ARITHMETIC), // 4E - DEC esi / REX.WRX prefix
        SIMPLE(INSTR_ARITHMETIC), // 4F - DEC edi / REX.WRXB prefix

        // 0x50-0x5F: Stack operations
        SIMPLE(INSTR_STACK), // 50 - PUSH eax/rax
        SIMPLE(INSTR_STACK), // 51 - PUSH ecx/rcx
        SIMPLE(INSTR_STACK), // 52 - PUSH edx/rdx
        SIMPLE(INSTR_STACK), // 53 - PUSH ebx/rbx
        SIMPLE(INSTR_STACK), // 54 - PUSH esp/rsp
        SIMPLE(INSTR_STACK), // 55 - PUSH ebp/rbp
        SIMPLE(INSTR_STACK), // 56 - PUSH esi/rsi
        SIMPLE(INSTR_STACK), // 57 - PUSH edi/rdi
        SIMPLE(INSTR_STACK), // 58 - POP eax/rax
        SIMPLE(INSTR_STACK), // 59 - POP ecx/rcx
        SIMPLE(INSTR_STACK), // 5A - POP edx/rdx
        SIMPLE(INSTR_STACK), // 5B - POP ebx/rbx
        SIMPLE(INSTR_STACK), // 5C - POP esp/rsp
        SIMPLE(INSTR_STACK), // 5D - POP ebp/rbp
        SIMPLE(INSTR_STACK), // 5E - POP esi/rsi
        SIMPLE(INSTR_STACK), // 5F - POP edi/rdi

        // 0x60-0x6F: Invalid in 64-bit mode
        INVALID,                            // 60 - PUSHA/PUSHAD (invalid in 64-bit)
        INVALID,                            // 61 - POPA/POPAD (invalid in 64-bit)
        INVALID,                            // 62 - BOUND (invalid in 64-bit)
        INVALID,                            // 63 - ARPL/MOVSXD
        INVALID,                            // 64 - FS segment override
        INVALID,                            // 65 - GS segment override
        INVALID,                            // 66 - Operand size override
        INVALID,                            // 67 - Address size override
        WITH_IMM32(INSTR_ARITHMETIC),       // 68 - PUSH imm32
        WITH_MODRM_IMM32(INSTR_ARITHMETIC), // 69 - IMUL r32, r/m32, imm32
        WITH_IMM8(INSTR_ARITHMETIC),        // 6A - PUSH imm8
        WITH_MODRM_IMM8(INSTR_ARITHMETIC),  // 6B - IMUL r32, r/m32, imm8
        SIMPLE(INSTR_SIMPLE),               // 6C - INSB
        SIMPLE(INSTR_SIMPLE),               // 6D - INSD
        SIMPLE(INSTR_SIMPLE),               // 6E - OUTSB
        SIMPLE(INSTR_SIMPLE),               // 6F - OUTSD

        // 0x70-0x7F: Conditional jumps (short)
        WITH_IMM8(INSTR_CONTROL), // 70 - JO rel8
        WITH_IMM8(INSTR_CONTROL), // 71 - JNO rel8
        WITH_IMM8(INSTR_CONTROL), // 72 - JB/JNAE/JC rel8
        WITH_IMM8(INSTR_CONTROL), // 73 - JNB/JAE/JNC rel8
        WITH_IMM8(INSTR_CONTROL), // 74 - JE/JZ rel8
        WITH_IMM8(INSTR_CONTROL), // 75 - JNE/JNZ rel8
        WITH_IMM8(INSTR_CONTROL), // 76 - JBE/JNA rel8
        WITH_IMM8(INSTR_CONTROL), // 77 - JNBE/JA rel8
        WITH_IMM8(INSTR_CONTROL), // 78 - JS rel8
        WITH_IMM8(INSTR_CONTROL), // 79 - JNS rel8
        WITH_IMM8(INSTR_CONTROL), // 7A - JP/JPE rel8
        WITH_IMM8(INSTR_CONTROL), // 7B - JNP/JPO rel8
        WITH_IMM8(INSTR_CONTROL), // 7C - JL/JNGE rel8
        WITH_IMM8(INSTR_CONTROL), // 7D - JNL/JGE rel8
        WITH_IMM8(INSTR_CONTROL), // 7E - JLE/JNG rel8
        WITH_IMM8(INSTR_CONTROL), // 7F - JNLE/JG rel8

        // 0x80-0x8F: Immediate arithmetic and MOV instructions
        WITH_MODRM_IMM8(INSTR_ARITHMETIC),  // 80 - ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m8, imm8
        WITH_MODRM_IMM32(INSTR_ARITHMETIC), // 81 - ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m32, imm32
        WITH_MODRM_IMM8(INSTR_ARITHMETIC),  // 82 - ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m8, imm8 (duplicate)
        WITH_MODRM_IMM8(INSTR_ARITHMETIC),  // 83 - ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m32, imm8
        WITH_MODRM(INSTR_ARITHMETIC),       // 84 - TEST r/m8, r8
        WITH_MODRM(INSTR_ARITHMETIC),       // 85 - TEST r/m32, r32
        WITH_MODRM(INSTR_DATA_MOV),         // 86 - XCHG r8, r/m
        WITH_MODRM(INSTR_DATA_MOV),         // 87 - XCHG r32, r/m32
        WITH_MODRM(INSTR_DATA_MOV),         // 88 - MOV r/m8, r8
        WITH_MODRM(INSTR_DATA_MOV),         // 89 - MOV r/m32, r32
        WITH_MODRM(INSTR_DATA_MOV),         // 8A - MOV r8, r/m8
        WITH_MODRM(INSTR_DATA_MOV),         // 8B - MOV r32, r/m32
        WITH_MODRM(INSTR_DATA_MOV),         // 8C - MOV r/m16, Sreg
        WITH_MODRM(INSTR_DATA_MOV),         // 8D - LEA r32, m
        WITH_MODRM(INSTR_DATA_MOV),         // 8E - MOV Sreg, r/m16
        WITH_MODRM(INSTR_DATA_MOV),         // 8F - POP r/m32

        // 0x90-0x9F: XCHG and string operations
        SIMPLE(INSTR_DATA_MOV),    // 90 - NOP / XCHG eax, eax
        SIMPLE(INSTR_DATA_MOV),    // 91 - XCHG eax, ecx
        SIMPLE(INSTR_DATA_MOV),    // 92 - XCHG eax, edx
        SIMPLE(INSTR_DATA_MOV),    // 93 - XCHG eax, ebx
        SIMPLE(INSTR_DATA_MOV),    // 94 - XCHG eax, esp
        SIMPLE(INSTR_DATA_MOV),    // 95 - XCHG eax, ebp
        SIMPLE(INSTR_DATA_MOV),    // 96 - XCHG eax, esi
        SIMPLE(INSTR_DATA_MOV),    // 97 - XCHG eax, edi
        SIMPLE(INSTR_DATA_MOV),    // 98 - CWDE/CBW
        SIMPLE(INSTR_DATA_MOV),    // 99 - CDQ/CWD
        WITH_IMM32(INSTR_CONTROL), // 9A - CALLF ptr16:32 (invalid in 64-bit)
        SIMPLE(INSTR_SYSTEM),      // 9B - WAIT/FWAIT
        SIMPLE(INSTR_STACK),       // 9C - PUSHF/PUSHFD
        SIMPLE(INSTR_STACK),       // 9D - POPF/POPFD
        SIMPLE(INSTR_DATA_MOV),    // 9E - SAHF
        SIMPLE(INSTR_DATA_MOV),    // 9F - LAHF

        // 0xA0-0xAF: Memory operations
        WITH_IMM32(INSTR_DATA_MOV), // A0 - MOV AL, moffs8
        WITH_IMM32(INSTR_DATA_MOV), // A1 - MOV EAX, moffs32
        WITH_IMM32(INSTR_DATA_MOV), // A2 - MOV moffs8, AL
        WITH_IMM32(INSTR_DATA_MOV), // A3 - MOV moffs32, EAX
        SIMPLE(INSTR_DATA_MOV),     // A4 - MOVSB
        SIMPLE(INSTR_DATA_MOV),     // A5 - MOVSD
        SIMPLE(INSTR_DATA_MOV),     // A6 - CMPSB
        SIMPLE(INSTR_DATA_MOV),     // A7 - CMPSD
        WITH_IMM8(INSTR_DATA_MOV),  // A8 - TEST AL, imm8
        WITH_IMM32(INSTR_DATA_MOV), // A9 - TEST EAX, imm32
        SIMPLE(INSTR_DATA_MOV),     // AA - STOSB
        SIMPLE(INSTR_DATA_MOV),     // AB - STOSD
        SIMPLE(INSTR_DATA_MOV),     // AC - LODSB
        SIMPLE(INSTR_DATA_MOV),     // AD - LODSD
        SIMPLE(INSTR_DATA_MOV),     // AE - SCASB
        SIMPLE(INSTR_DATA_MOV),     // AF - SCASD

        // 0xB0-0xBF: Immediate MOV instructions
        WITH_IMM8(INSTR_DATA_MOV),  // B0 - MOV AL, imm8
        WITH_IMM8(INSTR_DATA_MOV),  // B1 - MOV CL, imm8
        WITH_IMM8(INSTR_DATA_MOV),  // B2 - MOV DL, imm8
        WITH_IMM8(INSTR_DATA_MOV),  // B3 - MOV BL, imm8
        WITH_IMM8(INSTR_DATA_MOV),  // B4 - MOV AH, imm8
        WITH_IMM8(INSTR_DATA_MOV),  // B5 - MOV CH, imm8
        WITH_IMM8(INSTR_DATA_MOV),  // B6 - MOV DH, imm8
        WITH_IMM8(INSTR_DATA_MOV),  // B7 - MOV BH, imm8
        WITH_IMM32(INSTR_DATA_MOV), // B8 - MOV EAX, imm32
        WITH_IMM32(INSTR_DATA_MOV), // B9 - MOV ECX, imm32
        WITH_IMM32(INSTR_DATA_MOV), // BA - MOV EDX, imm32
        WITH_IMM32(INSTR_DATA_MOV), // BB - MOV EBX, imm32
        WITH_IMM32(INSTR_DATA_MOV), // BC - MOV ESP, imm32
        WITH_IMM32(INSTR_DATA_MOV), // BD - MOV EBP, imm32
        WITH_IMM32(INSTR_DATA_MOV), // BE - MOV ESI, imm32
        WITH_IMM32(INSTR_DATA_MOV), // BF - MOV EDI, imm32

        // 0xC0-0xCF: Shift, return and interrupt instructions
        WITH_MODRM_IMM8(INSTR_ARITHMETIC), // C0 - ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m8, imm8
        WITH_MODRM_IMM8(INSTR_ARITHMETIC), // C1 - ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m32, imm8
        WITH_IMM16(INSTR_CONTROL),         // C2 - RET imm16
        SIMPLE(INSTR_CONTROL),             // C3 - RET
        WITH_MODRM(INSTR_DATA_MOV),        // C4 - LES r32, m16:32 (invalid in 64-bit)
        WITH_MODRM(INSTR_DATA_MOV),        // C5 - LDS r32, m16:32 (invalid in 64-bit)
        WITH_MODRM_IMM8(INSTR_DATA_MOV),   // C6 - MOV r/m8, imm8
        WITH_MODRM_IMM32(INSTR_DATA_MOV),  // C7 - MOV r/m32, imm32
        WITH_IMM16_IMM8(INSTR_CONTROL),    // C8 - ENTER imm16, imm8
        SIMPLE(INSTR_CONTROL),             // C9 - LEAVE
        WITH_IMM16(INSTR_CONTROL),         // CA - RETF imm16
        SIMPLE(INSTR_CONTROL),             // CB - RETF
        SIMPLE(INSTR_SYSTEM),              // CC - INT 3
        WITH_IMM8(INSTR_SYSTEM),           // CD - INT imm8
        SIMPLE(INSTR_SYSTEM),              // CE - INTO
        SIMPLE(INSTR_CONTROL),             // CF - IRET

        // 0xD0-0xDF: Shift and floating-point instructions
        WITH_MODRM(INSTR_ARITHMETIC), // D0 - ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m8, 1
        WITH_MODRM(INSTR_ARITHMETIC), // D1 - ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m32, 1
        WITH_MODRM(INSTR_ARITHMETIC), // D2 - ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m8, CL
        WITH_MODRM(INSTR_ARITHMETIC), // D3 - ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m32, CL
        INVALID,                      // D4 - AAM (invalid in 64-bit)
        INVALID,                      // D5 - AAD (invalid in 64-bit)
        INVALID,                      // D6
        SIMPLE(INSTR_DATA_MOV),       // D7 - XLAT/XLATB
        WITH_MODRM(INSTR_ARITHMETIC), // D8 - Floating-point operation
        WITH_MODRM(INSTR_ARITHMETIC), // D9 - Floating-point operation
        WITH_MODRM(INSTR_ARITHMETIC), // DA - Floating-point operation
        WITH_MODRM(INSTR_ARITHMETIC), // DB - Floating-point operation
        WITH_MODRM(INSTR_ARITHMETIC), // DC - Floating-point operation
        WITH_MODRM(INSTR_ARITHMETIC), // DD - Floating-point operation
        WITH_MODRM(INSTR_ARITHMETIC), // DE - Floating-point operation
        WITH_MODRM(INSTR_ARITHMETIC), // DF - Floating-point operation

        // 0xE0-0xEF: Loop and control transfer
        WITH_IMM8(INSTR_CONTROL),  // E0 - LOOPNE/LOOPNZ rel8
        WITH_IMM8(INSTR_CONTROL),  // E1 - LOOPE/LOOPZ rel8
        WITH_IMM8(INSTR_CONTROL),  // E2 - LOOP rel8
        WITH_IMM8(INSTR_CONTROL),  // E3 - JCXZ/JECXZ rel8
        WITH_IMM8(INSTR_SIMPLE),   // E4 - IN AL, imm8
        WITH_IMM8(INSTR_SIMPLE),   // E5 - IN EAX, imm8
        WITH_IMM8(INSTR_SIMPLE),   // E6 - OUT imm8, AL
        WITH_IMM8(INSTR_SIMPLE),   // E7 - OUT imm8, EAX
        WITH_IMM32(INSTR_CONTROL), // E8 - CALL rel32
        WITH_IMM32(INSTR_CONTROL), // E9 - JMP rel32
        WITH_IMM32(INSTR_CONTROL), // EA - JMPF ptr16:32 (invalid in 64-bit)
        WITH_IMM8(INSTR_CONTROL),  // EB - JMP rel8
        SIMPLE(INSTR_SIMPLE),      // EC - IN AL, DX
        SIMPLE(INSTR_SIMPLE),      // ED - IN EAX, DX
        SIMPLE(INSTR_SIMPLE),      // EE - OUT DX, AL
        SIMPLE(INSTR_SIMPLE),      // EF - OUT DX, EAX

        // 0xF0-0xFF: Lock, string, and misc instructions
        INVALID,                      // F0 - LOCK prefix
        SIMPLE(INSTR_SYSTEM),         // F1 - INT1/ICEBP
        INVALID,                      // F2 - REPNE/REPNZ prefix
        INVALID,                      // F3 - REP/REPE/REPZ prefix
        SIMPLE(INSTR_SYSTEM),         // F4 - HLT
        SIMPLE(INSTR_SYSTEM),         // F5 - CMC
        WITH_MODRM(INSTR_ARITHMETIC), // F6 - TEST/NOT/NEG/MUL/IMUL/DIV/IDIV r/m8
        WITH_MODRM(INSTR_ARITHMETIC), // F7 - TEST/NOT/NEG/MUL/IMUL/DIV/IDIV r/m32
        SIMPLE(INSTR_SYSTEM),         // F8 - CLC
        SIMPLE(INSTR_SYSTEM),         // F9 - STC
        SIMPLE(INSTR_SYSTEM),         // FA - CLI
        SIMPLE(INSTR_SYSTEM),         // FB - STI
        SIMPLE(INSTR_SYSTEM),         // FC - CLD
        SIMPLE(INSTR_SYSTEM),         // FD - STD
        WITH_MODRM(INSTR_ARITHMETIC), // FE - INC/DEC r/m8
        WITH_MODRM(INSTR_ARITHMETIC)  // FF - INC/DEC/CALL/JMP/PUSH r/m32
};