# CRV, a Tiny RISC-V Emitter

CRV lets you assemble and emit RISC-V machine code at runtime. This is useful for developing dynamic recompilers.

## Usage

CRV is written in pure C, and makes heavy use of macros to create a clean API.

```c
#include "crv.h"

// To emit code, first create a CRV context.
CRV_CTX* ctx = CRV_Init();

// Emit code by calling instruction emitters
CRV_EmitAddi(ctx, t0, t1, 1); // Adds 1 to t1 and stores the result in t0
// Alternate mnemonics may be used
CRV_EmitAddi(ctx, x5, x6, 1);

// Registers can be assigned dynamically by wrapping them in r(), x(), R(), or X().
// These are not global macros, and are only valid within instruction emitters.
int destination_register = 5;
int operand_register = 6;
CRV_EmitAddi(ctx, r(destination_register), x(operand_register), 1);
CRV_EmitAddi(ctx, X(destination_register), R(operand_register), 1);

// Branch and jump instructions can either have offsets set manually...
CRV_EmitBeq(ctx, t0, t1, 4); // Jump 4 bytes ahead if t0 == t1

// Or, labels can be used.
CRV_LABEL* label = CRV_InitLabel(ctx);

CRV_EmitBeq(ctx, t0, t1, label); // Supports jumping forward
CRV_EmitAddi(ctx, t0, t1, 1);
CRV_BindLabel(ctx, label); // Branches targeting this label will now jump here.
CRV_EmitAddi(ctx, t0, t1, 1);
CRV_EmitBeq(ctx, t0, t1, label); // Also supports jumping backward

// To emit the code:
size_t code_size = CRV_CodeSize(ctx);
uint8_t* code_buffer = AllocateExecutableMemory(code_size); // It is up to the user to implement this!
CRV_Encode(ctx, code_buffer);

// Free resources
CRV_FreeLabel(label);
CRV_Free(ctx);

// Done!
```

## Assembling single instructions

All CRV_Emit* macros have a corresponding CRV_Assemble* macro. This is useful for, for example, patching already generated code.

```c
#include "crv.h"
CRV_CTX* ctx = CRV_Init();
CRV_EmitAddi(ctx, t0, t1, 1);
size_t patch_offset = CRV_CurrentOffset(ctx); // Gets the offset of the next instruction to be emitted
CRV_EmitAddi(ctx, t0, t0, 1); // This will be patched later

size_t code_size = CRV_CodeSize(ctx);
uint8_t* code_buffer = AllocateExecutableMemory(code_size); // It is up to the user to implement this!
CRV_Encode(ctx, code_buffer);
CRV_Free(ctx);

// Assemble a patched version of the second ADDI. Calls to the CRV_Assemble* macros do not need the ctx parameter.
uint32_t patched_instruction = CRV_AssembleAddi(t0, t0, 2);
// And copy it in
memcpy(code_buffer + patch_offset, &patched_instruction, sizeof(uint32_t));
```

## Notes
- Labels cannot be bound twice.
- If an instruction requires an even number (i.e. the LSB is not encoded into the instruction) it will be force-aligned. The same thing happens if a given immediate value is too large, it will be cut off. There is no error handling for this (yet.)
- The emitters are automatically generated at build time from [this GitHub repository](https://github.com/riscv/riscv-opcodes).