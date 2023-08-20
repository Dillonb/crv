#include <crv.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef WIN32
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#else
#include <windows.h>
#include <memoryapi.h>
#endif

void mprotect_error() {
#ifdef WIN32
    LPVOID lpMsgBuf;
    DWORD error = GetLastError();

    DWORD bufLen = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL );
    LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;

    if (bufLen) {
        printf("VirtualProtect failed! Code: dec %lu hex %lX Message: %s\n", error, error, lpMsgStr);
        exit(1);
    } else {
        printf("VirtualProtect failed! Code: %lu\n", error);
        exit(1);
    }
#else
    printf("mprotect failed! %s\n", strerror(errno));
    exit(1);
#endif
}


void mprotect_rwx(uint8_t* cache, size_t size) {
#ifdef WIN32
    DWORD oldProtect = 0;
    if (!VirtualProtect(cache, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        mprotect_error();
    }
#else
    if (mprotect(cache, size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
        mprotect_error();
    }
#endif
}

#ifdef WIN32
#define crv_aligned_malloc _aligned_malloc
#define crv_aligned_free _aligned_free
#else
#define crv_aligned_malloc aligned_alloc
#define crv_aligned_free free
#endif

size_t align_buf_size(int buf_size, int page_size) {
    size_t remainder = buf_size % page_size;
    if (remainder != 0) {
        return buf_size + (page_size - remainder);
    }
    return buf_size;
}

int main(int argc, char** argv) {
#ifdef WIN32
    int page_size = 4096;
#else
    int page_size = sysconf(_SC_PAGE_SIZE);
#endif
    CRV_CTX* ctx = CRV_Init();

    CRV_EmitAddi(ctx, a0, a0, 1);
    CRV_EmitJalr(ctx, zero, ra, 0);

    size_t buf_size = align_buf_size(CRV_CodeSize(ctx), page_size);
    uint8_t* buf = crv_aligned_malloc(buf_size, page_size);
    mprotect_rwx(buf, page_size);
    CRV_Encode(ctx, buf);
    CRV_Free(ctx);

#ifdef __riscv
    int (*add_one)(int x) = (void*)buf;
    printf("1 + 1 = %d\n", add_one(1));
#else
    printf("Not running on a RISC-V host, not executing code.\n");
#endif
    crv_aligned_free(buf);
}