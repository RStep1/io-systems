#pragma once

#define SBI_SUCCESS 0
#define XLEN 32
#define VERSION_MAJOR_MASK 0x7f
#define VERSION_MINOR_MASK 0xffffff
#define COUNTER_CSR_MASK 0xfff

struct sbiret {
    long error;
    long value;
};

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid);

void putchar(char ch);
void putstring(const char *str);
int getchar(void);

struct sbiret sbi_get_spec_version(void);
struct sbiret sbi_get_num_counters(void);
struct sbiret sbi_get_counter_info(unsigned long counter_idx);
void sbi_shutdown(void);

void print_menu(void);
