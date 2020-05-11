// 实验： address_space.c
// 定义多种变量形式，观察其地址，来了解进程空间
// gcc address_space.c -o address_space
// 运行中，可通过另一个终端，执行
// ps aux | grep address_space #查询其pid
// cat /proc/<pid>/maps   #查看各段区间
// 或者1个命令 cat /proc/`pidof address_space`/maps
// 或者1个命令 pmap `pidof address_space`
// 程序最后，通过按键<enter>退出
#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
int global_uninit_var;              //未初始化的全局变量
int global_init_var = 1;            //初始化的全局变量
static int global_static_init_var = 2;  //初始化的全局静态变量
static int global_static_uninit_var;    //未初始化的全局静态变量
const int global_const = 3;         //全局常量
int main(int argc, char **argv) {
  char *literal_const = "Welcome.";     //局部字符串常量
  static int local_static_init_var = 4; //静态局部初始化变量，编译时分配
  static int local_static_uninit_var; //静态局部未初始化变量，编译时分配
  char *malloc_ptr = (char*)malloc(16);//动态内存指针
  int local_var_init = 2;		//局部初始化变量
  int local_var_uninit;		//局部未初始化变量
  const int local_const = 3;	//局部常量
  printf("The following are addresses of types of process's mem\n");
  printf("Text location:\n");
  printf("\tmain\t\t\t(Code Segment):\t%p\n", main);
  printf("\tglobal_const\t(Code Segment):\t%p\n", &global_const);
  printf("\tconst string\t\t(Code Segment):\t%p\n", literal_const);
  printf("------------------------------\n");
  printf("Data Location:\n");
  printf("\tglobal_init_var\t\t(Data Segment):\t%p\n", &global_init_var);
  printf("\tglobal_static_init_var\t(Data Segment):\t%p\n", &global_static_init_var);
  printf("\tlocal_static_init_var\t(Data Segment):\t%p\n", &local_static_init_var);
  printf("------------------------------\n");
  printf("BSS Location:\n");
  printf("\tglobal_static_uninit_var(BSS Segment):\t%p\n", &global_static_uninit_var);
  printf("\tlocal_static_uninit_var\t(BSS Segment):\t%p\n", &local_static_uninit_var);
  printf("\tglobal_uninit_var\t(BSS Segment):\t%p\n", &global_uninit_var);
  printf("------------------------------\n");
  printf("Heap Location:\n");
  printf("\tmalloc_ptr\t\t(Heap Segment):\t%p\n", malloc_ptr);
  free(malloc_ptr);		//释放动态内存
  malloc_ptr = (char*)malloc(64);	//重新申请动态内存
  printf("\tnew malloc_ptr\t\t(Heap Segment):\t%p\n", malloc_ptr);
  free(malloc_ptr);
  printf("------------------------------\n");
  printf("Stack Location:\n");
  printf("\tlocal_var_init\t\t(Stack Segment):%p\n", &local_var_init);
  printf("\tlocal_var_uninit\t(Stack Segment):%p\n", &local_var_uninit);
  printf("\tlocal_const\t\t(Stack Segment):%p\n", &local_const);
  printf("------------------------------\n");
  printf("Press any key to quit.");
  getchar();
  return 0;
}
