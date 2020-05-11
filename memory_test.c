// 实验： memory_test.c
// 定义多种变量形式，观察其地址，来了解进程空间
// gcc memory_test.c -o memory_test
// 运行中，可通过另一个终端，执行
// ps aux | grep memory_test #查询其pid
// cat /proc/<pid>/maps   #查看各段区间
// 程序最后，通过按键<enter>退出
#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
int global_uninit_var;              //未初始化的全局变量
int global_init_var = 1;            //初始化的全局变量
static int global_static_init_var = 2;  //初始化的全局静态变量
static int global_static_uninit_var;    //未初始化的全局静态变量
const int global_const_var = 3;         //全局只读变量
int main(int argc, char **argv) {
  printf("below are addresses of types of process's mem\n");
  printf("Text location:\n");
  printf("\tmain\t\t\t(Code Segment):\t%p\n", main);
  char *literal_const = "Welcome.";     //字符串常量
  printf("\tglobal_const_var\t(Code Segment):\t%p\n", &global_const_var);
  printf("\tconst string\t\t(Code Segment):\t%p\n", literal_const);
  printf("------------------------------\n");
  printf("Data Location:\n");
  printf("\tglobal_init_var\t\t(Data Segment):\t%p\n", &global_init_var);
  printf("\tglobal_static_init_var\t(Data Segment):\t%p\n", &global_static_init_var);
  static int local_static_init_var = 4; //静态变量，编译时分配
  static int local_static_uninit_var; //静态变量，编译时分配
  printf("\tlocal_static_init_var\t(Data Segment):\t%p\n", &local_static_init_var);
  printf("\tlocal_static_uninit_var\t(Data Segment):\t%p\n", &local_static_uninit_var);
  printf("____________________________\n");
  printf("BSS Location:\n");
  printf("\tglobal_uninit_var\t(BSS Segment):\t%p\n", &global_uninit_var);
  printf("\tglobal_static_uninit_var(BSS Segment):\t%p\n", &global_static_uninit_var);
  getchar();
  printf("____________________________\n");
  char *heep_location = (char *)sbrk((ptrdiff_t)0);
  printf("Heap Location:\n");
  printf("\tInitial end of heap:\t%p\n", heep_location);
  brk(heep_location + 4);
  heep_location = (char *)sbrk((ptrdiff_t)0);
  printf("\tNew end of heap:%p\n", heep_location);
  char *malloc_ptr = (char*)malloc(16);
  printf("\tmalloc_ptr\t\t(Heap Segment):\t%p\n", malloc_ptr);
  malloc_ptr = (char *)realloc(malloc_ptr, 64);
  printf("____________________________\n");
  int local_var = 2;
  printf("Stack Location:\n");
  printf("\tlocal_var\t\t(Stack Segment):%p\n", &local_var);
  const int local_const_var = 3;
  printf("\tlocal_const_var\t\t(Stack Segment):\t%p\n", &local_const_var);
  printf("____________________________\n");
  printf("Press any key to quit.");
  getchar();
  free(malloc_ptr);
  return 0;
}
