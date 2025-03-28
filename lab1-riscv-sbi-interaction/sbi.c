#include "sbi.h"

#include <stdint.h>

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid) {
    register long a0 __asm__("a0") = arg0;
    register long a1 __asm__("a1") = arg1;
    register long a2 __asm__("a2") = arg2;
    register long a3 __asm__("a3") = arg3;
    register long a4 __asm__("a4") = arg4;
    register long a5 __asm__("a5") = arg5;
    register long a6 __asm__("a6") = fid;
    register long a7 __asm__("a7") = eid;

    __asm__ volatile("ecall"
                     : "=r"(a0), "=r"(a1)
                     : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                       "r"(a6), "r"(a7)
                     : "memory");
    return (struct sbiret){.error = a0, .value = a1};
}

void putchar(char ch) {
    sbi_call(ch, 0, 0, 0, 0, 0, 0, 1 /* Console Putchar */);
}

void putstring(const char *str) {
    while (*str) {
        putchar(*str++);
    }
}

int getchar(void) {
    return sbi_call(0, 0, 0, 0, 0, 0, 0, 2).error;
}

struct sbiret sbi_get_spec_version(void) {
    return sbi_call(0, 0, 0, 0, 0, 0, 0, 0x10);
}

struct sbiret sbi_get_num_counters(void) {
    return sbi_call(0, 0, 0, 0, 0, 0, 0, 0x504D55);
}

struct sbiret sbi_get_counter_info(unsigned long counter_id) {
    return sbi_call(counter_id, 0, 0, 0, 0, 0, 1, 0x504D55);
}

void sbi_shutdown(void) {
    sbi_call(0, 0, 0, 0, 0, 0, 0, 8);
}