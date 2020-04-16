//  实验2：Linux 程序接口
//  取进程标识及用户信息，了解Linux部分系统调用的用法。
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    struct passwd *my_info = getpwuid(getuid());
    printf("Process ID: %d\n", getpid());
    printf("Parent ID: %d\n", getppid());
    printf("User ID: %d\n", getuid());
    printf("Effective User ID: %d\n", geteuid());
    printf("Group ID: %d\n", getgid());
    printf("Effective Group ID: %d\n", getegid());
    if(my_info)
    {
        printf("My Login Name: %s\n", my_info->pw_name);
        printf("My Password: %s\n", my_info->pw_passwd);
        printf("My User ID: %d\n", my_info->pw_uid);
        printf("My Group ID: %d\n", my_info->pw_gid);
        printf("My Real Name: %s\n", my_info->pw_gecos);
        printf("My Home Directory: %s\n", my_info->pw_dir);
        printf("My Work Shell: %s\n", my_info->pw_shell);
    }
    return 0;
}
