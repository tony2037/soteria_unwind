#include <stdio.h>
#include <unistd.h>

int funcC(int a, int b, int c) {
    return a + a*b + a*b*c;
}
int funcB(int a, int b) {
    int r = funcC(a, b, a*b);
    return a * b * r;
} 

int funcA(int a) {
    int r = funcB(a, a*a);
    return r * a;
}
int main(int c, char **v) {
    pid_t pid = getpid();
    printf("Hello! I m an infinite loop\nMy PID: %d\n", pid);
    for (;;)
        funcA(20);
}

