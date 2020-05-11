#include <stdio.h>
#include <stdlib.h>

//const int const_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
//int data_array[1024] = {10};

typedef struct
{
    char c;
    __int64_t d;
    int e;
    short f;
    char g;
    short h;
} T;

typedef struct
{
    char c;
    int d;
    short e;
} A;

typedef struct
{
    char c;
    __int64_t d;
    int e;
    short f;
    A g;

    char h;
    int i;
} B;

int main(int argc, char* argv[])
{
    T t;
    A a;
    B b;
    printf("size of struct T is: %ld\n", sizeof(T));
    printf("\tt.c address: %p, sizeof(T.c): %d\n", &t.c, sizeof(t.c));
    printf("\tt.d address: %p\n", &t.d);
    printf("\tt.e address: %p\n", &t.e);
    printf("\tt.f address: %p\n", &t.f);
    printf("\tt.g address: %p\n", &t.g);
    printf("\tt.h address: %p\n", &t.h);
    printf("size of struct A is: %ld\n", sizeof(A));
    printf("size of struct B is: %ld\n", sizeof(B));
    return 0;
}
