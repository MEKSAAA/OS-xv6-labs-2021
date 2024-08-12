#include "kernel/types.h"
#include "user/user.h"

int main() {
    char *addr = sbrk(4096 * 2); // 分配两个页的内存
    addr[0] = 'x'; // 访问第一个页

    unsigned int accessed = 0;
    if (pgaccess(addr, 2, &accessed) < 0) {
        printf("pgaccess failed\n");
        exit(1);
    }

    if (accessed & 1)
        printf("Page 0 accessed\n");
    if (accessed & 2)
        printf("Page 1 accessed\n");

    exit(0);
}
