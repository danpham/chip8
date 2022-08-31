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
#include <time.h>
#include "cpu.h"
#include "../import/import.h"
#include "../display/display.h"

/******************************************************************
 * 2. Define declarations (macros then function macros)
 ******************************************************************/
#define CPU_OPCODES_NUMBER                                       15U
#define CPU_IDENTIFIER_INVALID                                0xFFFF
#define CPU_IDENTIFIER_CLEAR_SCREEN                           0x00E0
#define CPU_IDENTIFIER_RETURN                                 0x00EE
#define CPU_IDENTIFIER_JUMP                                   0x1000
#define CPU_IDENTIFIER_CALL                                   0x2000
#define CPU_IDENTIFIER_SE                                     0x3000
#define CPU_IDENTIFIER_SNE                                    0x4000
#define CPU_IDENTIFIER_SE_VXVY                                0x5000
#define CPU_IDENTIFIER_SET_VX                                 0x6000
#define CPU_IDENTIFIER_ADD_TO_VX                              0x7000
#define CPU_IDENTIFIER_8XXX                                   0x8000
#define CPU_IDENTIFIER_SNE_VXVY                               0x9000
#define CPU_IDENTIFIER_SET_I                                  0xA000
#define CPU_IDENTIFIER_JUMP_V0                                0xB000
#define CPU_IDENTIFIER_RAND_VX                                0xC000
#define CPU_IDENTIFIER_DRAW                                   0xD000

#define CPU_JUMP_MASK                                         0x0FFF
#define CPU_CALL_MASK                                         0x0FFF
#define CPU_SET_VX_REGISTER_MASK                              0x0F00
#define CPU_SET_VX_VALUE_MASK                                 0x00FF
#define CPU_SET_I_MASK                                        0x0FFF
#define CPU_DRAW_VX_MASK                                      0x0F00
#define CPU_DRAW_VY_MASK                                      0x00F0
#define CPU_DRAW_N_MASK                                       0x000F
#define CPU_SE_VX_MASK                                        0x0F00
#define CPU_SE_VALUE_MASK                                     0x00FF
#define CPU_SE_VXVY_VX_MASK                                   0x0F00
#define CPU_SE_VXVY_VY_MASK                                   0x00F0
#define CPU_8XXX_VX_MASK                                      0x0F00
#define CPU_8XXX_VY_MASK                                      0x00F0
#define CPU_8XXX_IDENTIFIER_MASK                              0x000F
#define CPU_SNE_VXVY_VX_MASK                                  0x0F00
#define CPU_SNE_VXVY_VY_MASK                                  0x00F0
#define CPU_JUMP_V0_MASK                                      0x0FFF
#define CPU_RAND_VX_VX_MASK                                   0x0F00
#define CPU_RAND_VX_BYTE_MASK                                 0x00FF

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
    S8 stackLevel;
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
    {0xFFFF, CPU_IDENTIFIER_RETURN},
    {0xF000, CPU_IDENTIFIER_JUMP},
    {0xF000, CPU_IDENTIFIER_CALL},
    {0xF000, CPU_IDENTIFIER_SE},
    {0xF000, CPU_IDENTIFIER_SNE},
    {0xF00F, CPU_IDENTIFIER_SE_VXVY},
    {0xF000, CPU_IDENTIFIER_SET_VX},
    {0xF000, CPU_IDENTIFIER_ADD_TO_VX},
    {0xF000, CPU_IDENTIFIER_8XXX},
    {0xF00F, CPU_IDENTIFIER_SNE_VXVY},
    {0xF000, CPU_IDENTIFIER_SET_I},
    {0xF000, CPU_IDENTIFIER_JUMP_V0},
    {0xF000, CPU_IDENTIFIER_RAND_VX},
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
static void cpuIdentifierReturn();
static void cpuIdentifierCall(U16 opCode);
static void cpuIdentifierSE(U16 opCode);
static void cpuIdentifierSNE(U16 opCode);
static void cpuIdentifierSEVxVy(U16 opCode);
static void cpuIdentifierJump(U16 opCode);
static void cpuIdentifierSetVx(U16 opCode);
static void cpuIdentifierAddToVx(U16 opCode);
static void cpuIdentifier8xxx(U16 opCode);
static void cpuIdentifierSNEVxVy(U16 opCode);
static void cpuIdentifierSetI(U16 opCode);
static void cpuIdentifierJumpV0(U16 opCode);
static void cpuIdentifierRandVx(U16 opCode);
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
        /* Set PC to start address */
        s_cpu.pc = CPU_START_ADDRESS;

        /* Set stack level to -1 */
        s_cpu.stackLevel = -1;

        /* Load ROM */
        returnValue = ImportRom(s_cpu.memory);

        /* Random seed initialization */
        srand(time(NULL));
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
    case CPU_IDENTIFIER_RETURN:
        cpuIdentifierReturn();
        break;
    case CPU_IDENTIFIER_CALL:
        cpuIdentifierCall(currentOpCode);
        break;
    case CPU_IDENTIFIER_SE:
        cpuIdentifierSE(currentOpCode);
        break;
    case CPU_IDENTIFIER_SNE:
        cpuIdentifierSNE(currentOpCode);
        break;
    case CPU_IDENTIFIER_SE_VXVY:
        cpuIdentifierSEVxVy(currentOpCode);
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
    case CPU_IDENTIFIER_8XXX:
        cpuIdentifier8xxx(currentOpCode);
        break;
    case CPU_IDENTIFIER_SNE_VXVY:
        cpuIdentifierSNEVxVy(currentOpCode);
        break;
    case CPU_IDENTIFIER_SET_I:
        cpuIdentifierSetI(currentOpCode);
        break;
    case CPU_IDENTIFIER_JUMP_V0:
        cpuIdentifierJumpV0(currentOpCode);
        break;
    case CPU_IDENTIFIER_RAND_VX:
        cpuIdentifierRandVx(currentOpCode);
        break;
    case CPU_IDENTIFIER_DRAW:
        cpuIdentifierDraw(currentOpCode);
        break;
    case CPU_IDENTIFIER_INVALID:
    default:
        break;
    }
}

/******************************************************************
 * FUNCTION : cpuIdentifierClearScreen()
 *    Description: Clear screen
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierClearScreen()
{
    /* Clear screen routine */
    DisplayClearScreen();

    /* Increment program counter */
    s_cpu.pc += 2U;
}

/******************************************************************
 * FUNCTION : cpuIdentifierReturn()
 *    Description: Return
 *    Parameters:  None
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierReturn()
{
    /* Set pc to stack value */
    s_cpu.pc = s_cpu.stack[s_cpu.stackLevel];

    /* Decrement stack level */
    s_cpu.stackLevel--;
}

/******************************************************************
 * FUNCTION : cpuIdentifierCall()
 *    Description: Call desired routine
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierCall(U16 opCode)
{
    U16 routineAddress = opCode & CPU_CALL_MASK;

    /* Increment stack pointer */
    s_cpu.stackLevel++; 

    /* Store pc on stack */
    s_cpu.stack[s_cpu.stackLevel] = s_cpu.pc;

    /* Set program counter */
    s_cpu.pc = routineAddress;
}

/******************************************************************
 * FUNCTION : cpuIdentifierSE()
 *    Description: Skip next instruction if Vx = kk.
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierSE(U16 opCode)
{
    U8 vx = (U8)((opCode & CPU_SE_VX_MASK) >> 8U);
    U8 value = (U8)(opCode & CPU_SE_VALUE_MASK);

    if (s_cpu.vx[vx] == value)
    {
        /* Skip next instruction */
        s_cpu.pc += 4U;
    }
    else
    {
        /* Go to next instruction */
        s_cpu.pc += 2U;
    }
}

/******************************************************************
 * FUNCTION : cpuIdentifierSNE()
 *    Description: Skip next instruction if Vx != kk.
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierSNE(U16 opCode)
{
    U8 vx = (U8)((opCode & CPU_SE_VX_MASK) >> 8U);
    U8 value = (U8)(opCode & CPU_SE_VALUE_MASK);

    if (s_cpu.vx[vx] != value)
    {
        /* Skip next instruction */
        s_cpu.pc += 4U;
    }
    else
    {
        /* Go to next instruction */
        s_cpu.pc += 2U;
    }
}

/******************************************************************
 * FUNCTION : cpuIdentifierSEVxVy()
 *    Description: Skip next instruction if Vx = Vy.
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierSEVxVy(U16 opCode)
{
    U8 vx = (U8)((opCode & CPU_SE_VXVY_VX_MASK) >> 8U);
    U8 vy = (U8)((opCode & CPU_SE_VXVY_VY_MASK) >> 4U);

    if (s_cpu.vx[vx] == s_cpu.vx[vy])
    {
        /* Skip next instruction */
        s_cpu.pc += 4U;
    }
    else
    {
        /* Go to next instruction */
        s_cpu.pc += 2U;
    }
}

/******************************************************************
 * FUNCTION : cpuIdentifier8xxx()
 *    Description: Handle all similar opcode starting with 8xxx
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifier8xxx(U16 opCode)
{
    U8 vx = (U8)((opCode & CPU_8XXX_VX_MASK) >> 8U);
    U8 vy = (U8)((opCode & CPU_8XXX_VY_MASK) >> 4U);
    U8 identifier = (U8)(opCode & CPU_8XXX_IDENTIFIER_MASK);
    U16 add;

    switch (identifier)
    {
    case 0x0:
        /* Load Vx register with Vy */
        s_cpu.vx[vx] = s_cpu.vx[vy];
        break;
    case 0x1:
        /* Bitwise OR */
        s_cpu.vx[vx] |= s_cpu.vx[vy];
        break;
    case 0x2:
        /* Bitwise AND */
        s_cpu.vx[vx] &= s_cpu.vx[vy];
        break;
    case 0x3:
        /* Bitwise XOR */
        s_cpu.vx[vx] ^= s_cpu.vx[vy];
        break;
    case 0x4:
        add = s_cpu.vx[vx] + s_cpu.vx[vy];

        /* 8-bit ADD */
        s_cpu.vx[vx] += s_cpu.vx[vy];

        if (add > 255U)
        {
            /* If addition is overflowing set VF to 1 */
            s_cpu.vx[0xF] = 1U;
        }
        else
        {
            s_cpu.vx[0xF] = 0U;
        }
        break;
    case 0x5:
        if (s_cpu.vx[vx] > s_cpu.vx[vy])
        {
            /* Not borrow */
            s_cpu.vx[0xF] = 1U;
        }
        else
        {
            s_cpu.vx[0xF] = 0U;
        }

        /* 8-bit SUB */
        s_cpu.vx[vx] -= s_cpu.vx[vy];
        break;  
    case 0x6:
        /* If the least-significant bit of Vx is 1 */
        if ((s_cpu.vx[vx] & 0x01) == 1U)
        {
            s_cpu.vx[0xF] = 1U;
        }
        else
        {
            s_cpu.vx[0xF] = 0U;
        }

        /* Vx is divided by 2 */
        s_cpu.vx[vx] = s_cpu.vx[vx] / 2U;
        break;
    case 0x7:
        if (s_cpu.vx[vy] > s_cpu.vx[vx])
        {
            /* Not borrow */
            s_cpu.vx[0xF] = 1U;
        }
        else
        {
            s_cpu.vx[0xF] = 0U;
        }

        /* 8-bit SUBN */
        s_cpu.vx[vx] = s_cpu.vx[vy] - s_cpu.vx[vx];
        break;  
    case 0xE:
        /* If the most-significant bit of Vx is 1 */
        if ((s_cpu.vx[vx] & 0x80) == 1U)
        {
            s_cpu.vx[0xF] = 1U;
        }
        else
        {
            s_cpu.vx[0xF] = 0U;
        }

        /* Vx is multiplied by 2 */
        s_cpu.vx[vx] = s_cpu.vx[vx] * 2U;
        break;
    default:
        break;
    }

    /* Go to next instruction */
    s_cpu.pc += 2U;
}

/******************************************************************
 * FUNCTION : cpuIdentifierSNEVxVy()
 *    Description: Skip next instruction if Vx != Vy.
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierSNEVxVy(U16 opCode)
{
    U8 vx = (U8)((opCode & CPU_SNE_VXVY_VX_MASK) >> 8U);
    U8 vy = (U8)((opCode & CPU_SNE_VXVY_VY_MASK) >> 4U);

    if (s_cpu.vx[vx] != s_cpu.vx[vy])
    {
        /* Skip next instruction */
        s_cpu.pc += 4U;
    }
    else
    {
        /* Go to next instruction */
        s_cpu.pc += 2U;
    }
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
 * FUNCTION : cpuIdentifierJumpV0()
 *    Description: Jump to location nnn + V0.
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierJumpV0(U16 opCode)
{
    U16 jumpAddress = opCode & CPU_JUMP_V0_MASK;

    /* Set program counter */
    s_cpu.pc = s_cpu.vx[0U] + jumpAddress;
}


/******************************************************************
 * FUNCTION : cpuIdentifierRandVx()
 *    Description: Set Vx = random byte AND kk.
 *    Parameters:  opCode: jump opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierRandVx(U16 opCode)
{
    U8 vx = (U8)((opCode & CPU_RAND_VX_VX_MASK) >> 8U);
    U8 byte = (U8)(opCode & CPU_RAND_VX_BYTE_MASK);

    /* Perform a modulus 255 to be on 1 byte */
    U8 random = (U8)(rand() % 255U);

    /* AND with the input byte */
    random = random && byte;

    /* Set result to vx */
    s_cpu.vx[vx] = random;

    /* Increment program counter */
    s_cpu.pc += 2U;
}

/******************************************************************
 * FUNCTION : cpuIdentifierDraw()
 *    Description: Draw value
 *    Parameters:  opCode: draw opcode
 *    Return:      None
 ******************************************************************/
static void cpuIdentifierDraw(U16 opCode)
{
    U8 vx = (U8)((opCode & CPU_DRAW_VX_MASK) >> 8U);
    U8 x = s_cpu.vx[vx];
    U8 vy = (U8)((opCode & CPU_DRAW_VY_MASK) >> 4U);
    U8 y = s_cpu.vx[vy];
    U8 n = opCode & CPU_DRAW_N_MASK; /* number of bytes to display */

    DisplayDraw(&s_cpu.memory[s_cpu.i], x, y, n);

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