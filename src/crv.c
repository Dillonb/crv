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

#include "crv.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// 1KiB initial buffer size
#define CRV_BUF_INITIAL_SIZE 0x1000

typedef struct _CRV_INSTR {
    size_t code_offset;
    enum {
        _CRV_32B_COMPLETE_INSTR,
        _CRV_32B_INCOMPLETE_BTYPE
    } type;
    union {
        uint32_t _32b_complete_instr;
        struct {
          CRV_LABEL* offset;
          uint8_t rs2;
          uint8_t rs1;
          uint8_t funct3;
          uint8_t opcode;
        } _32b_incomplete_btype;
    };
} _CRV_INSTR;

size_t _CRV_InstrSize(_CRV_INSTR* instr) {
    switch (instr->type) {
        case _CRV_32B_COMPLETE_INSTR:
        case _CRV_32B_INCOMPLETE_BTYPE:
            return sizeof(uint32_t);
    }
}

typedef struct CRV_CTX {
    _CRV_INSTR* instr_buf;
    size_t instr_buf_size;
    size_t instr_buf_used;
    
    size_t running_code_size;
} CRV_CTX;

typedef struct CRV_LABEL {
    bool is_bound;
    size_t offset;
    CRV_CTX* ctx;
} CRV_LABEL;

CRV_CTX* CRV_Init() {
    CRV_CTX* ctx = malloc(sizeof(CRV_CTX));

    memset(ctx, 0, sizeof(CRV_CTX));
    ctx->instr_buf_size = CRV_BUF_INITIAL_SIZE;
    ctx->instr_buf_used = 0;
    ctx->instr_buf = malloc(ctx->instr_buf_size * sizeof(_CRV_INSTR));

    return ctx;
}

void CRV_Free(CRV_CTX* ctx) {
    free(ctx->instr_buf);
    free(ctx);
}

size_t CRV_CodeSize(CRV_CTX* ctx) {
    return ctx->running_code_size;
}

CRV_LABEL* CRV_InitLabel(CRV_CTX* ctx) {
    CRV_LABEL* l = malloc(sizeof(CRV_LABEL));
    l->is_bound = false;
    l->offset = 0;
    l->ctx = ctx;
    return l;
}

void CRV_BindLabel(CRV_CTX* ctx, CRV_LABEL* label) {
    if (!label->is_bound) {
        label->offset = ctx->running_code_size;
        label->is_bound = true;
    } else {
        printf("WARNING: tried to re-bind a label!\n");
    }
}

void CRV_FreeLabel(CRV_LABEL* label) {
    free(label);
}

int16_t _CRV_LabelToOffset(CRV_LABEL* label, size_t use_offset) {
    if (!label->is_bound) {
        printf("WARNING: Converting an unbound label to an offset!\n");
    }
    return label->offset - use_offset;
}

void _CRV_AppendInstruction(CRV_CTX* ctx, _CRV_INSTR instr) {
    if (ctx->instr_buf_used + 1 >= ctx->instr_buf_size) {
        size_t new_size = ctx->instr_buf_size + sizeof(_CRV_INSTR);
        ctx->instr_buf = realloc(ctx->instr_buf, new_size);
        ctx->instr_buf_size = new_size;
    }

    instr.code_offset = ctx->running_code_size;
    ctx->instr_buf[ctx->instr_buf_used++] = instr;
    ctx->running_code_size += _CRV_InstrSize(&instr);
}

void _CRV_Emit_IType(CRV_CTX* ctx, int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode) {
    _CRV_INSTR instr = { 0 };
    instr.type = _CRV_32B_COMPLETE_INSTR;

    instr._32b_complete_instr |= (imm12  & 0xFFF) << 20;
    instr._32b_complete_instr |= (rs1    & 0x1F)  << 15;
    instr._32b_complete_instr |= (funct3 & 0x7)   << 12;
    instr._32b_complete_instr |= (rd     & 0x1F)  << 7;
    instr._32b_complete_instr |= opcode  & 0x7F   << 0;
    _CRV_AppendInstruction(ctx, instr);
}

uint32_t _CRV_Assemble_BType(int16_t imm12, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t opcode) {
    uint32_t instr = 0;
    instr |= (imm12  & 0x1000) << 19; // imm[12]
    instr |= (imm12  & 0x7E0)  << 20; // imm[10:5]
    instr |= (rs2    & 0x1F)   << 20;
    instr |= (rs1    & 0x1F)   << 15;
    instr |= (funct3 & 0x7)    << 12;
    instr |= (imm12  & 0x1E)   << 7;  // imm[4:1]
    instr |= (imm12  & 0x800)  >> 4;  // imm[11]
    instr |= (opcode & 0x7F)   << 0;

    return instr;
}

void _CRV_Emit_BType(CRV_CTX* ctx, int16_t imm12, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t opcode) {
    _CRV_INSTR instr = { 0 };
    instr.type = _CRV_32B_COMPLETE_INSTR;
    instr._32b_complete_instr = _CRV_Assemble_BType(imm12, rs2, rs1, funct3, opcode);
    _CRV_AppendInstruction(ctx, instr);
}

void _CRV_Emit_BType_Incomplete(CRV_CTX* ctx, CRV_LABEL* offset, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t opcode) {
    _CRV_INSTR instr = { 0 };
    instr.type = _CRV_32B_INCOMPLETE_BTYPE;
    instr._32b_incomplete_btype.offset = offset;
    instr._32b_incomplete_btype.rs2 = rs2;
    instr._32b_incomplete_btype.rs1 = rs1;
    instr._32b_incomplete_btype.funct3 = funct3;
    instr._32b_incomplete_btype.opcode = opcode;
    _CRV_AppendInstruction(ctx, instr);
}

void CRV_Encode(CRV_CTX* ctx, uint8_t* buf) {
    size_t buf_index = 0;
    for (size_t i = 0; i < ctx->instr_buf_used; i++) {
        _CRV_INSTR* instr = &ctx->instr_buf[i];
        size_t instr_size = _CRV_InstrSize(instr);
        uint8_t* instr_loc = buf + buf_index;

        switch (instr->type) {
            case _CRV_32B_COMPLETE_INSTR:
                memcpy(instr_loc, &instr->_32b_complete_instr, sizeof(uint32_t));
                break;
            case _CRV_32B_INCOMPLETE_BTYPE: {
                uint32_t assembled = _CRV_Assemble_BType(
                    _CRV_LabelToOffset(instr->_32b_incomplete_btype.offset, instr->code_offset),
                    instr->_32b_incomplete_btype.rs2,
                    instr->_32b_incomplete_btype.rs1,
                    instr->_32b_incomplete_btype.funct3,
                    instr->_32b_incomplete_btype.opcode);
                memcpy(instr_loc, &assembled, sizeof(uint32_t));
                break;
            }
        }

        buf_index += instr_size;
    }
}