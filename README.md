# csapp
linux> ./prog
如果prog不是shell的内置命令，shell会认为prog是可执行目标文件，调用顺序为：
    shell->execve->loader
                  |
                  -> prog
