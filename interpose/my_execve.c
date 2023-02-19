#define _GNU_SOURCE
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#define MAX_ARG_LEN 2048

typedef int (*execve_fun_t)(const char *, char *const [], char *const []);

void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

void cat_char_vec(char* dest, char* const srcv[])
{
    char* const * current_src = srcv;
    int len_remain = MAX_ARG_LEN;
    while (current_src && *current_src && len_remain > 0) {
        // printf("here: %s\n", *current_src);
        // printf("%s\n", dest);
        strncat(dest, " ", len_remain);
        len_remain -= strlen(" ");
        strncat(dest, *current_src, len_remain);
        len_remain -= strlen(*current_src);
        ++current_src;
    }
}

void cat_char_valist(char* dest, const char *arg0, va_list args)
{
    const char* current_arg = arg0;
    while(current_arg) {
        strcat(dest, current_arg);
        current_arg = va_arg(args, char*);
    }
}

int count_char_vec(char *const argv[])
{
    int num = 1;
    while(*argv) {
        num++;
        argv++;
    }
    return num;
}

int execl(const char *path, const char *arg0, ... /*, (char *)0 */)
{
    printf("[execl]: %s %s\n", path, arg0);
}

int execle(const char *path, const char *arg0, ... /*,(char *)0, char *const envp[]*/)
{
    printf("[execle]: %s %s\n", path, arg0);
}

int execlp(const char *file, const char *arg0, ... /*, (char *)0 */)
{
    printf("[execlp]: %s %s\n", file, arg0);
}

int execv(const char *path, char *const argv[])
{
    char *error;
    int (*origin_execv_ptr)(const char *pathname, char *const argv[]);
    origin_execv_ptr = dlsym(RTLD_NEXT, "execv");
    if ((error = dlerror()) != NULL) {
        write(2, error, strlen(error));
        write(2, "\n", strlen("\n"));
        exit(1);
    }
    char cmdline_buf[MAX_ARG_LEN] = {0};
    cat_char_vec(cmdline_buf, argv);
    printf("[Executing(execv)]: %s\n", cmdline_buf);
    printf("[Env LD_PRELOAD]: %s\n", getenv("LD_PRELOAD"));
    int status = origin_execv_ptr(path, argv);
    if (status < 0) {
        unix_error("origin_execv_ptr error");
        return status;
    }
}

int execve_binsh(char *const argv[], char *const envp[], execve_fun_t origin_execve_ptr)
{
    int arg_num = count_char_vec(argv);
    char * new_argv[arg_num+1];
    new_argv[0] = "/bin/sh";
    int i = 0;
    for (int i = 0; i < arg_num; ++i) {
        new_argv[i+1] = argv[i];
    }
    return origin_execve_ptr("/bin/sh", new_argv, envp);
}

int execve(const char *pathname, char *const argv[], char *const envp[])
{
    char *error;
    execve_fun_t origin_execve_ptr;
    origin_execve_ptr = dlsym(RTLD_NEXT, "execve");
    if ((error = dlerror()) != NULL) {
        write(2, error, strlen(error));
        write(2, "\n", strlen("\n"));
        exit(1);
    }
    char cmdline_buf[MAX_ARG_LEN] = {0};
    cat_char_vec(cmdline_buf, argv);
    printf("[Executing(execve)]: %s\n", cmdline_buf);
    printf("[Env LD_PRELOAD]: %s\n", getenv("LD_PRELOAD"));
    // unsetenv("LD_PRELOAD");
    // printf("[Env LD_PRELOAD]: %s\n", getenv("LD_PRELOAD"));
    int status = origin_execve_ptr(pathname, argv, envp);
    if (status == -1 && errno == ENOEXEC && strcmp("/bin/sh", pathname)) {
        status = execve_binsh(argv, envp, origin_execve_ptr);
    }
    if (status < 0) {
        unix_error("origin_execve_ptr error");
        return status;
    }
}

int execvp(const char *file, char *const argv[])
{
    char *error;
    int (*origin_execvp_ptr)(const char *file, char *const argv[]);
    origin_execvp_ptr = dlsym(RTLD_NEXT, "execvp");
    if ((error = dlerror()) != NULL) {
        write(2, error, strlen(error));
        write(2, "\n", strlen("\n"));
        exit(1);
    }
    char cmdline_buf[MAX_ARG_LEN] = {0};
    cat_char_vec(cmdline_buf, argv);
    printf("[Executing(execvp)]: %s\n", cmdline_buf);
    printf("[Env LD_PRELOAD]: %s\n", getenv("LD_PRELOAD"));
    int status = origin_execvp_ptr(file, argv);
    if (status < 0) {
        unix_error("origin_execvp_ptr error");
        return status;
    }
}

int execvpe(const char *pathname, char *const argv[], char *const envp[])
{
    char *error;
    int (*origin_execvpe_ptr)(const char *pathname, char *const argv[], char *const envp[]);
    origin_execvpe_ptr = dlsym(RTLD_NEXT, "execvpe");
    if ((error = dlerror()) != NULL) {
        write(2, error, strlen(error));
        write(2, "\n", strlen("\n"));
        exit(1);
    }
    char cmdline_buf[MAX_ARG_LEN] = {0};
    cat_char_vec(cmdline_buf, argv);
    printf("[Executing(execvpe)]: %s\n", cmdline_buf);
    printf("[Env LD_PRELOAD]: %s\n", getenv("LD_PRELOAD"));
    int status = origin_execvpe_ptr(pathname, argv, envp);
    if (status < 0) {
        unix_error("origin_execvpe_ptr error");
        return status;
    }
}
