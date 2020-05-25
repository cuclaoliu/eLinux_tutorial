测试设备驱动开发实例
1. 文件说明
	drvdemo.c		设备驱动代码
	drvdemo_read.c		读设备测试程序代码
	drvdemo_write.c		写设备测试程序代码
	Makefile		编译脚本文件
2. 编译准备工作
	编译驱动，需要内核头文件
	步骤A
		$ sudo apt-get install cmake gcc clang gdb build-essential libelf-dev
		$ sudo apt update && sudo apt upgrade #保持系统更新
		
	步骤B，以下任选其一，主要是保证头文件的版本与内核版本一致
		$ sudo apt install linux-headers-$(uname -r) #安装内核头文件，“uname -r”可返回内核版本字符串
	或
		$ sudo apt install linux-headers-generic
3. 编译
	这里需要编译内核模块，编译两个测试程序，所以使用Makefile更方便
		$ make
		$ ll
	# Makefile 默认编译了drvdemo模块和两个测试程序
	# 文件列表，显示了有用的：drvdemo.ko, drvdemo_read, drvdemo_write
4. 运行测试
	需要insmod和mknod，这里也写到 Makefile 中了，所以只需
		$ sudo make install
		$ ll drvdemo
	# 多了设备文件 drvdemo
		$ ./drvdemo_write "This is my driver demo. Thanks."
		$ dmesg | tail -n 10
		$ ./drvdemo_read

		$ sudo make uninstall
		$ ll drvdemo # 会提示没有这个文件，因已经删除了该节点
