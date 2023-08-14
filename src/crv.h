/*
* CRV RISC-V Emitter
*
* MIT License
*
* Copyright (c) 2023 Dillon Beliveau
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef CRV_RV_EMITTER_CRV_H
#define CRV_RV_EMITTER_CRV_H

#include <stdint.h>
#include <stddef.h>
#include "crv_constants.h"

typedef struct CRV_CTX CRV_CTX;
typedef struct CRV_LABEL CRV_LABEL;

CRV_CTX* CRV_Init();
void CRV_Free(CRV_CTX* ctx);
size_t CRV_CodeSize(CRV_CTX* ctx);
void CRV_Encode(CRV_CTX* ctx, uint8_t* buf);

CRV_LABEL* CRV_InitLabel(CRV_CTX* ctx);
void CRV_BindLabel(CRV_CTX* ctx, CRV_LABEL* label);
void CRV_FreeLabel(CRV_LABEL* label);

void _CRV_Emit_IType(CRV_CTX* ctx, int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode);
void _CRV_Emit_BType(CRV_CTX* ctx, int16_t imm12, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t opcode);
void _CRV_Emit_BType_Incomplete(CRV_CTX* ctx, CRV_LABEL* offset, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t opcode);

// Integer Register-Immediate instructions
#define CRV_EmitAddi(ctx, rd, rs1, imm12)  _CRV_Emit_IType(ctx, imm12, _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_ADDI,  _CRV_RV_REG_TO_INT(rd), CRV_RV_OPCODE_IMM)
#define CRV_EmitSlti(ctx, rd, rs1, imm12)  _CRV_Emit_IType(ctx, imm12, _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_SLTI,  _CRV_RV_REG_TO_INT(rd), CRV_RV_OPCODE_IMM)
#define CRV_EmitSltiu(ctx, rd, rs1, imm12) _CRV_Emit_IType(ctx, imm12, _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_SLTIU, _CRV_RV_REG_TO_INT(rd), CRV_RV_OPCODE_IMM)
#define CRV_EmitAndi(ctx, rd, rs1, imm12)  _CRV_Emit_IType(ctx, imm12, _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_ANDI,  _CRV_RV_REG_TO_INT(rd), CRV_RV_OPCODE_IMM)
#define CRV_EmitOri(ctx, rd, rs1, imm12)   _CRV_Emit_IType(ctx, imm12, _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_ORI,   _CRV_RV_REG_TO_INT(rd), CRV_RV_OPCODE_IMM)
#define CRV_EmitXori(ctx, rd, rs1, imm12)  _CRV_Emit_IType(ctx, imm12, _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_XORI,  _CRV_RV_REG_TO_INT(rd), CRV_RV_OPCODE_IMM)

// Conditional branches
#define CRV_EmitBeq(ctx, rs1, rs2, offset) _Generic((offset), \
    CRV_LABEL*: _CRV_Emit_BType_Incomplete, \
    default: _CRV_Emit_BType \
    )(ctx, offset, _CRV_RV_REG_TO_INT(rs2), _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_BEQ, CRV_RV_OPCODE_BRANCH)
#define CRV_EmitBne(ctx, rs1, rs2, offset) _Generic((offset), \
    CRV_LABEL*: _CRV_Emit_BType_Incomplete, \
    default: _CRV_Emit_BType \
    )(ctx, offset, _CRV_RV_REG_TO_INT(rs2), _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_BNE, CRV_RV_OPCODE_BRANCH)
#define CRV_EmitBlt(ctx, rs1, rs2, offset) _Generic((offset), \
    CRV_LABEL*: _CRV_Emit_BType_Incomplete, \
    default: _CRV_Emit_BType \
    )(ctx, offset, _CRV_RV_REG_TO_INT(rs2), _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_BLT, CRV_RV_OPCODE_BRANCH)
#define CRV_EmitBge(ctx, rs1, rs2, offset) _Generic((offset), \
    CRV_LABEL*: _CRV_Emit_BType_Incomplete, \
    default: _CRV_Emit_BType \
    )(ctx, offset, _CRV_RV_REG_TO_INT(rs2), _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_BGE, CRV_RV_OPCODE_BRANCH)
#define CRV_EmitBltu(ctx, rs1, rs2, offset) _Generic((offset), \
    CRV_LABEL*: _CRV_Emit_BType_Incomplete, \
    default: _CRV_Emit_BType \
    )(ctx, offset, _CRV_RV_REG_TO_INT(rs2), _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_BLTU, CRV_RV_OPCODE_BRANCH)
#define CRV_EmitBgeu(ctx, rs1, rs2, offset) _Generic((offset), \
    CRV_LABEL*: _CRV_Emit_BType_Incomplete, \
    default: _CRV_Emit_BType \
    )(ctx, offset, _CRV_RV_REG_TO_INT(rs2), _CRV_RV_REG_TO_INT(rs1), CRV_RV_FUNCT3_BGEU, CRV_RV_OPCODE_BRANCH)

// TODO write a script to combine all headers and source files to make a true header only library
#ifdef CRV_IMPLEMENTATION
#include "crv.c"
#endif

#endif // CRV_RV_EMITTER_CRV_H