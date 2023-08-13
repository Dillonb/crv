#include "test.h"

#include <crv.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_CAPSTONE
#include <capstone/capstone.h>
#endif

void run_test() {
    CRV_CTX* ctx = CRV_Init();
    CRV_EmitAddi(ctx, t0, t1, -1);
    CRV_EmitSlti(ctx, t0, t1, 1);
    CRV_EmitSltiu(ctx, t0, t1, 5);
    CRV_EmitAndi(ctx, t0, t1, 1);
    CRV_EmitOri(ctx, t0, t1, 1);
    CRV_EmitXori(ctx, t0, t1, 1);
    size_t buf_size = CRV_BufSize(ctx);
    uint8_t* buf = malloc(buf_size);
    CRV_Encode(ctx, buf);
    CRV_Free(ctx);

    printf("Raw code:\n");
    for (size_t i = 0; i < buf_size; i++) {
        printf("%02X", buf[i]);
    }
    printf("\n");
#ifdef HAVE_CAPSTONE
    printf("Disassembling with Capstone\n");
    csh cs_handle;
    if (cs_open(CS_ARCH_RISCV, CS_MODE_RISCV64, &cs_handle)) {
        printf("Failed to initialize capstone");
        exit(1);
    }
    cs_option(cs_handle, CS_OPT_DETAIL, CS_OPT_ON);
    cs_insn* insn;
    size_t count = cs_disasm(cs_handle, buf, buf_size, 0, 0, &insn);

    for (size_t i = 0; i < count; i++) {
        printf("%08" PRIX64 " %s %s\n", insn[i].address, insn[i].mnemonic, insn[i].op_str);
    }
    cs_free(insn, count);
#endif
}