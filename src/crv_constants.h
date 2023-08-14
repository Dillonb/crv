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

#ifndef CRV_RV_EMITTER_CRV_CONSTANTS_H
#define CRV_RV_EMITTER_CRV_CONSTANTS_H

#define CRV_RV_REG_zero 0
#define CRV_RV_REG_ra   1
#define CRV_RV_REG_sp   2
#define CRV_RV_REG_gp   3
#define CRV_RV_REG_tp   4
#define CRV_RV_REG_t0   5
#define CRV_RV_REG_t1   6
#define CRV_RV_REG_t2   7
#define CRV_RV_REG_s0   8
#define CRV_RV_REG_s1   9
#define CRV_RV_REG_a0   10
#define CRV_RV_REG_a1   11
#define CRV_RV_REG_a2   12
#define CRV_RV_REG_a3   13
#define CRV_RV_REG_a4   14
#define CRV_RV_REG_a5   15
#define CRV_RV_REG_a6   16
#define CRV_RV_REG_a7   17
#define CRV_RV_REG_s2   18
#define CRV_RV_REG_s3   19
#define CRV_RV_REG_s4   20
#define CRV_RV_REG_s5   21
#define CRV_RV_REG_s6   22
#define CRV_RV_REG_s7   23
#define CRV_RV_REG_s8   24
#define CRV_RV_REG_s9   25
#define CRV_RV_REG_s10  26
#define CRV_RV_REG_s11  27
#define CRV_RV_REG_t3   28
#define CRV_RV_REG_t4   29
#define CRV_RV_REG_t5   30
#define CRV_RV_REG_t6   31

#define CRV_RV_REG_ZERO 0
#define CRV_RV_REG_RA   1
#define CRV_RV_REG_SP   2
#define CRV_RV_REG_GP   3
#define CRV_RV_REG_TP   4
#define CRV_RV_REG_T0   5
#define CRV_RV_REG_T1   6
#define CRV_RV_REG_T2   7
#define CRV_RV_REG_S0   8
#define CRV_RV_REG_S1   9
#define CRV_RV_REG_A0   10
#define CRV_RV_REG_A1   11
#define CRV_RV_REG_A2   12
#define CRV_RV_REG_A3   13
#define CRV_RV_REG_A4   14
#define CRV_RV_REG_A5   15
#define CRV_RV_REG_A6   16
#define CRV_RV_REG_A7   17
#define CRV_RV_REG_S2   18
#define CRV_RV_REG_S3   19
#define CRV_RV_REG_S4   20
#define CRV_RV_REG_S5   21
#define CRV_RV_REG_S6   22
#define CRV_RV_REG_S7   23
#define CRV_RV_REG_S8   24
#define CRV_RV_REG_S9   25
#define CRV_RV_REG_S10  26
#define CRV_RV_REG_S11  27
#define CRV_RV_REG_T3   28
#define CRV_RV_REG_T4   29
#define CRV_RV_REG_T5   30
#define CRV_RV_REG_T6   31

#define CRV_RV_REG_x0  0
#define CRV_RV_REG_x1  1
#define CRV_RV_REG_x2  2
#define CRV_RV_REG_x3  3
#define CRV_RV_REG_x4  4
#define CRV_RV_REG_x5  5
#define CRV_RV_REG_x6  6
#define CRV_RV_REG_x7  7
#define CRV_RV_REG_x8  8
#define CRV_RV_REG_x9  9
#define CRV_RV_REG_x10 10
#define CRV_RV_REG_x11 11
#define CRV_RV_REG_x12 12
#define CRV_RV_REG_x13 13
#define CRV_RV_REG_x14 14
#define CRV_RV_REG_x15 15
#define CRV_RV_REG_x16 16
#define CRV_RV_REG_x17 17
#define CRV_RV_REG_x18 18
#define CRV_RV_REG_x19 19
#define CRV_RV_REG_x20 20
#define CRV_RV_REG_x21 21
#define CRV_RV_REG_x22 22
#define CRV_RV_REG_x23 23
#define CRV_RV_REG_x24 24
#define CRV_RV_REG_x25 25
#define CRV_RV_REG_x26 26
#define CRV_RV_REG_x27 27
#define CRV_RV_REG_x28 28
#define CRV_RV_REG_x29 29
#define CRV_RV_REG_x30 30
#define CRV_RV_REG_x31 31

#define CRV_RV_REG_X0  0
#define CRV_RV_REG_X1  1
#define CRV_RV_REG_X2  2
#define CRV_RV_REG_X3  3
#define CRV_RV_REG_X4  4
#define CRV_RV_REG_X5  5
#define CRV_RV_REG_X6  6
#define CRV_RV_REG_X7  7
#define CRV_RV_REG_X8  8
#define CRV_RV_REG_X9  9
#define CRV_RV_REG_X10 10
#define CRV_RV_REG_X11 11
#define CRV_RV_REG_X12 12
#define CRV_RV_REG_X13 13
#define CRV_RV_REG_X14 14
#define CRV_RV_REG_X15 15
#define CRV_RV_REG_X16 16
#define CRV_RV_REG_X17 17
#define CRV_RV_REG_X18 18
#define CRV_RV_REG_X19 19
#define CRV_RV_REG_X20 20
#define CRV_RV_REG_X21 21
#define CRV_RV_REG_X22 22
#define CRV_RV_REG_X23 23
#define CRV_RV_REG_X24 24
#define CRV_RV_REG_X25 25
#define CRV_RV_REG_X26 26
#define CRV_RV_REG_X27 27
#define CRV_RV_REG_X28 28
#define CRV_RV_REG_X29 29
#define CRV_RV_REG_X30 30
#define CRV_RV_REG_X31 31

#define CRV_RV_REG_R(n) (n)
#define CRV_RV_REG_r(n) (n)
#define CRV_RV_REG_x(n) (n)
#define CRV_RV_REG_X(n) (n)

#define _CRV_RV_REG_TO_INT(reg) CRV_RV_REG_##reg

#define CRV_RV_OPCODE_IMM    0x13
#define CRV_RV_OPCODE_BRANCH 0x63

#define CRV_RV_FUNCT3_BEQ  0x0
#define CRV_RV_FUNCT3_BNE  0x1
#define CRV_RV_FUNCT3_BLT  0x4
#define CRV_RV_FUNCT3_BGE  0x5
#define CRV_RV_FUNCT3_BLTU 0x6
#define CRV_RV_FUNCT3_BGEU 0x7

#define CRV_RV_FUNCT3_ADDI  0x0
#define CRV_RV_FUNCT3_SLLI  0x1
#define CRV_RV_FUNCT3_SLTI  0x2
#define CRV_RV_FUNCT3_SLTIU 0x3
#define CRV_RV_FUNCT3_XORI  0x4
#define CRV_RV_FUNCT3_SRLI  0x5
#define CRV_RV_FUNCT3_SRAI  0x5
#define CRV_RV_FUNCT3_ORI   0x6
#define CRV_RV_FUNCT3_ANDI  0x7

#endif // CRV_RV_EMITTER_CRV_CONSTANTS_H