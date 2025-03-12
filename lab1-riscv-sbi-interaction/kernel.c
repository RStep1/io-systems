#include "sbi.h"
#include "common.h"

extern char __bss[], __bss_end[], __stack_top[];

void print_menu(void) {
    putstring("1. SBI version\n");
    putstring("2. Counters count\n");
    putstring("3. Counter info\n");
    putstring("4. Shutdown\n");
}

int read_int(void) {
    char buffer[32];
    int index = 0;
    while (1) {
        int c;
        while ((c = getchar()) == -1)
            ;
        putchar(c);
        if (c == '\r') {
            buffer[index] = '\0';
            putchar('\n');
            break;
        } else if (c >= '0' && c <= '9') {
            buffer[index++] = c;
        }
    }

    int number = 0;
    for (int i = 0; i < index; i++) {
        number = number * 10 + (buffer[i] - '0');
    }

    return number;
}

void kernel_main(void) {
    while (1) {
        print_menu();
        putstring("> ");

        int choice;
        while ((choice = getchar()) == -1)
            ;
        putchar(choice);
        putchar('\n');

        switch (choice) {
            case '1': {
                struct sbiret version = sbi_get_spec_version();
                putstring("SBI version: ");
                int minor = version.value & VERSION_MINOR_MASK;
                int major = (version.value >> 24) & VERSION_MAJOR_MASK;
                printf("%d.%d", major, minor);
                putchar('\n');
                break;
            }
            case '2': {
                struct sbiret num_counters = sbi_get_num_counters();
                putstring("Number of counters: ");
                printf("%d", num_counters.value);
                putchar('\n');
                break;
            }
            case '3': {
                putstring("Enter counter ID: ");
                int id = read_int();
                printf("%d\n", id);
                struct sbiret counter_info = sbi_get_counter_info(id);
                
                if (counter_info.error == SBI_SUCCESS) {
                    int counter_type = (counter_info.value >> (XLEN - 1)) & 0x1;
                    
                    if (counter_type == 0) {
                        int csr = counter_info.value & COUNTER_CSR_MASK;
                        int width = ((counter_info.value >> 12) & 0x3f) + 1;
                        putstring("Counter type: Hardware\n");
                        printf("CSR: %d \n", csr);
                        printf("Width: %d \n", width);
                    } else {
                        putstring("Counter type: Firmware\n");
                    }
                } else {
                    printf("The counter with id = %d do not exists\n", id);
                }
                
                break;
            }
            case '4': {
                sbi_shutdown();
                break;
            }
            default: {
                putstring("Invalid choice\n");
                break;
            }
        }
    }
}

__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void) {
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" // Установка указателя стека
        "j kernel_main\n"       // Переход к функции kernel_main
        :
        : [stack_top] "r" (__stack_top) // Передача верхнего адреса стека
    );
}