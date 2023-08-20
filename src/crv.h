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

typedef enum _crv_label_field {
    _CRV_LABEL_FIELD_bimm12,
    _CRV_LABEL_FIELD_jimm20
} _crv_label_field_t;

CRV_CTX* CRV_Init();
void CRV_Free(CRV_CTX* ctx);
size_t CRV_CodeSize(CRV_CTX* ctx);
void CRV_Encode(CRV_CTX* ctx, uint8_t* buf);

CRV_LABEL* CRV_InitLabel(CRV_CTX* ctx);
void CRV_BindLabel(CRV_CTX* ctx, CRV_LABEL* label);
void CRV_FreeLabel(CRV_LABEL* label);

void _CRV_Emit_32b(CRV_CTX* ctx, uint32_t instruction);
void _CRV_Emit_32b_Label(CRV_CTX* ctx, uint32_t instruction, CRV_LABEL* label, _crv_label_field_t field);
// Hack: work around all _Generic associations needing to be valid
void _CRV_Emit_32b_Label_Ignore(CRV_CTX* ctx, uint32_t instruction, int label, _crv_label_field_t field);

#define CRV_RV_ARG_rs1(rs1) (_CRV_RV_REG_TO_INT(rs1) << 15)
#define CRV_RV_ARG_rs2(rs2) (_CRV_RV_REG_TO_INT(rs2) << 20)
#define CRV_RV_ARG_rd(rd) (_CRV_RV_REG_TO_INT(rd) << 7)
#define CRV_RV_ARG_imm12(imm12) (((imm12) & 0xFFF) << 20)
#define _CRV_LabelToZero(x) _Generic((x), CRV_LABEL*: 0, default: (x))
#define CRV_RV_ARG_bimm12(bimm12) _Generic((bimm12), CRV_LABEL*: 0, default: ( \
    (((uint32_t)_CRV_LabelToZero(bimm12)  & 0x1000) << 19) | \
    (((uint32_t)_CRV_LabelToZero(bimm12)  & 0x7E0)  << 20) | \
    (((uint32_t)_CRV_LabelToZero(bimm12)  & 0x1E)   << 7)  | \
    (((uint32_t)_CRV_LabelToZero(bimm12)  & 0x800)  >> 4)))
#define CRV_RV_ARG_imm20(imm20) (((imm20) & 0xFFFFF) << 12)
#define CRV_RV_ARG_imm12_split(imm12) ((((imm12) & 0x1F) << 7) | (((imm12) & 0xFE0) << 20))
#define CRV_RV_ARG_jimm20(jimm20) _Generic((jimm20), CRV_LABEL*: 0, default: ( \
    (((uint32_t)_CRV_LabelToZero(jimm20) & 0x100000) << 11) | \
    (((uint32_t)_CRV_LabelToZero(jimm20) & 0x7FE)    << 20) | \
    (((uint32_t)_CRV_LabelToZero(jimm20) & 0x800)    << 9) | \
    (((uint32_t)_CRV_LabelToZero(jimm20) & 0xFF000)  << 0)))

#include "crv_emitters.h"

#endif // CRV_RV_EMITTER_CRV_H