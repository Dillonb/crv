#include <crv.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_CAPSTONE
#include <capstone/capstone.h>
#endif

int main(int argc, char** argv) {
    CRV_CTX* ctx = CRV_Init();
    CRV_LABEL* l = CRV_InitLabel(ctx);
    CRV_LABEL* l_forward = CRV_InitLabel(ctx);

    CRV_EmitAddi(ctx, t0, t1, -1);
    CRV_EmitBeq(ctx, t0, t1, l_forward);
    CRV_EmitBne(ctx, t0, t1, l_forward);
    CRV_EmitBlt(ctx, t0, t1, l_forward);
    CRV_EmitBge(ctx, t0, t1, l_forward);
    CRV_EmitBltu(ctx, t0, r(0), l_forward);
    CRV_EmitBgeu(ctx, t0, t1, l_forward);
    CRV_EmitSlti(ctx, t0, t1, 1);
    CRV_BindLabel(ctx, l_forward);
    CRV_EmitSltiu(ctx, t0, t1, 5);
    CRV_EmitAndi(ctx, t0, t1, 1);
    CRV_EmitOri(ctx, t0, t1, 1);
    CRV_BindLabel(ctx, l);
    CRV_EmitXori(ctx, t0, t1, 1);
    CRV_EmitBeq(ctx, t0, t1, -1234);
    CRV_EmitBeq(ctx, t0, t1, l);
    CRV_EmitLui(ctx, t0, 0xffffb3);
    CRV_EmitAuipc(ctx, t0, 0xffffb3);
    CRV_EmitJal(ctx, t0, 0x12346);
    CRV_EmitJal(ctx, t0, l);
    CRV_EmitJalr(ctx, t0, t1, -1);
    CRV_EmitLb(ctx, t0, t1, -2);
    CRV_EmitLh(ctx, t0, t1, -3);
    CRV_EmitLw(ctx, t0, t1, -4);
    CRV_EmitLbu(ctx, t0, t1, -5);
    CRV_EmitLhu(ctx, t0, t1, -6);
    CRV_EmitSb(ctx, t1, t2, -7);
    CRV_EmitSh(ctx, t1, t2, -8);
    CRV_EmitSw(ctx, t1, t2, -9);
    CRV_EmitAdd(ctx, t0, t1, t2);
    CRV_EmitSll(ctx, t0, t1, t2);
    CRV_EmitSlt(ctx, t0, t1, t2);
    CRV_EmitSltu(ctx, t0, t1, t2);
    CRV_EmitXor(ctx, t0, t1, t2);
    CRV_EmitSrl(ctx, t0, t1, t2);
    CRV_EmitSra(ctx, t0, t1, t2);
    CRV_EmitOr(ctx, t0, t1, t2);
    CRV_EmitAnd(ctx, t0, t1, t2);
    //CRV_EmitFence(ctx, fm, pred, succ, t1, t0);
    CRV_EmitEcall(ctx);
    CRV_EmitEbreak(ctx);

    // From RV64
    CRV_EmitLwu(ctx, t0, t1, -1);
    CRV_EmitLd(ctx, t0, t1, -2);
    CRV_EmitSd(ctx, t0, t1, -3);
    CRV_EmitSlli(ctx, t0, t1, 4);
    CRV_EmitSrli(ctx, t0, t1, 5);
    CRV_EmitSrai(ctx, t0, t1, 6);
    CRV_EmitAddiw(ctx, t0, t1, 7);
    CRV_EmitSlliw(ctx, t0, t1, 8);
    CRV_EmitSrliw(ctx, t0, t1, 9);
    CRV_EmitSraiw(ctx, t0, t1, 10);

    CRV_EmitAddw(ctx, t0, t1, t2);
    CRV_EmitSubw(ctx, t0, t1, t2);
    CRV_EmitSllw(ctx, t0, t1, t2);
    CRV_EmitSrlw(ctx, t0, t1, t2);
    CRV_EmitSraw(ctx, t0, t1, t2);

    size_t buf_size = CRV_CodeSize(ctx);
    uint8_t* buf = malloc(buf_size);
    CRV_Encode(ctx, buf);

    CRV_FreeLabel(l);
    CRV_FreeLabel(l_forward);
    CRV_Free(ctx);

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
#else
    printf("Raw code:\n");
    for (size_t i = 0; i < buf_size; i++) {
        printf("%02X", buf[i]);
    }
    printf("\n");
#endif
    free(buf);
}