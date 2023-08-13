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
#include <string.h>

// 1KiB initial buffer size
#define CRV_BUF_INITIAL_SIZE 0x1000

typedef struct CRV_CTX {
    uint8_t* buf;
    size_t buf_size;
    size_t buf_used;
} CRV_CTX;

CRV_CTX* CRV_Init() {
    CRV_CTX* ctx = malloc(sizeof(CRV_CTX));

    memset(ctx, 0, sizeof(CRV_CTX));
    ctx->buf_size = CRV_BUF_INITIAL_SIZE;
    ctx->buf_used = 0;
    ctx->buf = malloc(ctx->buf_size);

    return ctx;
}

void CRV_Free(CRV_CTX* ctx) {
    free(ctx->buf);
    free(ctx);
}

size_t CRV_BufSize(CRV_CTX* ctx) {
    return ctx->buf_used;
}

void CRV_Encode(CRV_CTX* ctx, uint8_t* buf) {
    memcpy(buf, ctx->buf, ctx->buf_used);
}

void _CRV_Emit_Data(CRV_CTX* ctx, uint8_t* data, size_t size) {
    if (ctx->buf_used + size > ctx->buf_size) {
        size_t new_size = ctx->buf_size << 1;
        ctx->buf = realloc(ctx->buf, new_size);
        ctx->buf_size = new_size;
    }

    memcpy(ctx->buf + ctx->buf_used, data, size);
    ctx->buf_used += size;
}

void _CRV_Emit_IType(CRV_CTX* ctx, int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode) {
    uint32_t instr = ((imm12 & 0xFFF) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12) | ((rd & 0x1F) << 7) | (opcode & 0x7F);
    _CRV_Emit_Data(ctx, (uint8_t*)&instr, sizeof(uint32_t));
}