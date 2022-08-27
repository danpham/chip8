/******************************************************************
 *
 *
 * FILE        : cpu.c
 * PROJECT     : chip8
 * AUTHOR      : danpham
 * DESCRIPTION : Cpu emulator
 *
 ******************************************************************/

/******************************************************************
 * 1. Included files (microcontroller ones then user defined ones)
 ******************************************************************/
#include <string.h>
#include "cpu.h"
#include "../import/import.h"

/******************************************************************
 * 2. Define declarations (macros then function macros)
 ******************************************************************/
#define CPU_OPCODES_NUMBER                                        6U
#define CPU_IDENTIFIER_INVALID                                0xFFFF
#define CPU_IDENTIFIER_CLEAR_SCREEN                           0x00E0
#define CPU_IDENTIFIER_JUMP                                   0x1000
#define CPU_IDENTIFIER_SET_VX                                 0x6000
#define CPU_IDENTIFIER_ADD_TO_VX                              0x7000
#define CPU_IDENTIFIER_SET_I                                  0xA000
#define CPU_IDENTIFIER_DRAW                                   0xD000

#define CPU_JUMP_MASK                                         0x0FFF
#define CPU_SET_VX_REGISTER_MASK                              0x0F00
#define CPU_SET_VX_VALUE_MASK                                 0x00FF
#define CPU_SET_I_MASK                                        0x0FFF

/******************************************************************
 * 3. Typedef definitions (simple typedef, then enum and structs)
 ******************************************************************/
typedef struct
{
    U8 memory[CPU_MEMORY_SIZE];
    U16 i;
    U8 vx[CPU_NUMBER_OF_VX_REGISTER];
    U16 pc;
    U16 stack[CPU_STACK_DEPTH_LEVEL];
    U8 stackLevel;
    U8 sysCounter;
    U8 soundCounter;
} cpuType;

typedef struct
{
    U16 mask;
    U16 identifier;
} opCodeType;

opCodeType opCodeReference[CPU_OPCODES_NUMBER] =
{
    {0xFFFF, CPU_IDENTIFIER_CLEAR_SCREEN},
    {0xF000, CPU_IDENTIFIER_JUMP},
    {0xF000, CPU_IDENTIFIER_SET_VX},
    {0xF000, CPU_IDENTIFIER_ADD_TO_VX},
    {0xF000, CPU_IDENTIFIER_SET_I},
    {0xF000, CPU_IDENTIFIER_DRAW},
};

/******************************************************************
 * 4. Variable definitions (static then global)
 ******************************************************************/
static cpuType s_cpu;

/******************************************************************
 * 5. Functions prototypes (static only)
 ******************************************************************/
static void cpuCounters(void);
static void cpuExecute(U16 identifier);
static U16 cpuParseOpcode(void);
static void cpuIdentifierClearScreen();
static void cpuIdentifierJump(U16 opCode);
static void cpuIdentifierSetVx(U16 opCode);
static void cpuIdentifierAddToVx(U16 opCode);
static void cpuIdentifierSetI(U16 opCode);
static void cpuIdentifierDraw(U16 opCode);

/******************************************************************
 * FUNCTION : CpuInit()
 *    Description: Initialize cpu
 *    Parameters:  None
 *    Return:      E_OK if initialization succeed, E_NOT_OK
 *                 otherwise.
 ******************************************************************/
Std_ReturnType CpuInit(void)
{
    cpuType *returnPtr = NULL;
    Std_ReturnType returnValue = E_NOT_OK;

    returnPtr = (cpuType *)memset((void *)&s_cpu, 0U, sizeof(cpuType));

    if (&s_cpu == returnPtr)
    {
        s_cpu.pc = CPU_START_ADDRESS;

        /* Load ROM */
        returnValue = ImportRom(s_cpu.memory);
    }

    return returnValue;
}

/******************************************************************
 * FUNCTION : cpuCounters()
 *    Description: Handle time counters
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
static void cpuCounters(void)
{
    if (s_cpu.sysCounter > 0)
    {
        s_cpu.sysCounter--;
    }

    if (s_cpu.soundCounter > 0)
    {
        s_cpu.soundCounter--;
    }

    return;
}

/******************************************************************
 * FUNCTION : cpuParseOpcode()
 *    Description: 
 *    Parameters:  None
 *    Return:      Found identifier
 ******************************************************************/
static U16 cpuParseOpcode(void)
{
    U16 currentOpCode = (s_cpu.memory[s_cpu.pc] << 8U) + s_cpu.memory[s_cpu.pc + 1U];
    U16 identifier = CPU_IDENTIFIER_INVALID;
    U8 i;

    for (i = 0U; i < CPU_OPCODES_NUMBER; i++)
    {
        if ((currentOpCode & opCodeReference[i].mask) == opCodeReference[i].identifier)
        {
            identifier = opCodeReference[i].identifier;
            break;
        }
    }

    return identifier;
}

/******************************************************************
 * FUNCTION : cpuExecute()
 *    Description: Route on the correct identifier to execute
 *    Parameters:  identifier
 *    Return:      None
 ******************************************************************/
static void cpuExecute(U16 identifier)
{
    U16 currentOpCode = (s_cpu.memory[s_cpu.pc] << 8U) + s_cpu.memory[s_cpu.pc + 1U];

    switch (identifier)
    {
    case CPU_IDENTIFIER_CLEAR_SCREEN:
        cpuIdentifierClearScreen();
        break;
    case CPU_IDENTIFIER_JUMP:
        cpuIdentifierJump(currentOpCode);
        break;
    case CPU_IDENTIFIER_SET_VX:
        cpuIdentifierSetVx(currentOpCode);
        break;
    case CPU_IDENTIFIER_ADD_TO_VX:
        cpuIdentifierAddToVx(currentOpCode);
        break;
    case CPU_IDENTIFIER_SET_I:
        cpuIdentifierSetI(currentOpCode);
        break;
    case CPU_IDENTIFIER_DRAW:
        cpuIdentifierDraw(currentOpCode);
        break;
    case CPU_IDENTIFIER_INVALID:
    default:
        break;
    }

    return;
}

/******************************************************************
 * FUNCTION : cpuIdentifierClearScreen()
 *    Description: Clear screen
 *    Parameters:  None
 *    Return:      E_OK if loop succeed, E_NOT_OK
 *                 otherwise.
 ******************************************************************/
static void cpuIdentifierClearScreen()
{

    /* Increment program counter */
    s_cpu.pc += 2U;
}

/******************************************************************
 * FUNCTION : cpuIdentifierJump()
 *    Description: Jump to desired address
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierJump(U16 opCode)
{
    U16 jumpAddress = opCode & CPU_JUMP_MASK;

    /* Set program counter */
    s_cpu.pc = jumpAddress;
}

/******************************************************************
 * FUNCTION : cpuIdentifierSetVx()
 *    Description: Set value to specified register Vx
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierSetVx(U16 opCode)
{
    U8 reg = (U8)((opCode & CPU_SET_VX_REGISTER_MASK) >> 8U);
    U8 value = (U8)(opCode & CPU_SET_VX_VALUE_MASK);

    /* Set value to specified register */
    s_cpu.vx[reg] = value;

    /* Increment program counter */
    s_cpu.pc += 2U;
}

/******************************************************************
 * FUNCTION : cpuIdentifierAddToVx()
 *    Description: Add value to specified register Vx
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierAddToVx(U16 opCode)
{
    U8 reg = (U8)((opCode & CPU_SET_VX_REGISTER_MASK) >> 8U);
    U8 value = (U8)(opCode & CPU_SET_VX_VALUE_MASK);

    /* Set value to specified register */
    s_cpu.vx[reg] += value;

    /* Increment program counter */
    s_cpu.pc += 2U;
}

/******************************************************************
 * FUNCTION : cpuIdentifierSetI()
 *    Description: Set value to specified register I
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierSetI(U16 opCode)
{
    U16 value = opCode & CPU_SET_I_MASK;

    /* Set value to specified register */
    s_cpu.i = value;

    /* Increment program counter */
    s_cpu.pc += 2U;
}

/******************************************************************
 * FUNCTION : cpuIdentifierDraw()
 *    Description: Set value to specified register I
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierDraw(U16 opCode)
{
    /* Increment program counter */
    s_cpu.pc += 2U;
}

/******************************************************************
 * FUNCTION : CpuMain()
 *    Description: Main cpu loop
 *    Parameters:  None
 *    Return:      E_OK if loop succeed, E_NOT_OK
 *                 otherwise.
 ******************************************************************/
Std_ReturnType CpuMain(void)
{
    U16 identifier = CPU_IDENTIFIER_INVALID;

    /* Update cpu counters */
    cpuCounters();

    /* Parse opcode and get identifier */
    identifier = cpuParseOpcode();

    /* Execute one instruction */
    cpuExecute(identifier);

    return E_OK;
}