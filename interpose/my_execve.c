#define _GNU_SOURCE
#include <stddef.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>

#define MAX_ARG_LEN 2048
#define MAX_PATH_LEN 1024
#define PRELOAD_ENV_NAME "LD_PRELOAD"
#define MYPRELOADSO "MY_PRELOAD_SO"

typedef int (*execve_fun_t)(const char *, char *const [], char *const []);
extern char** environ;
static char path_libmy_execev_so[MAX_PATH_LEN];
static size_t len_preload_env_name;
static size_t len_path_libmy_execev_so;

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

size_t count_char_vec(char *const argv[])
{
    size_t num = 1;
    while(*argv) {
        num++;
        argv++;
        if (num > 1024) {
            printf("[ERROR]: More than %zu strings are found!\n", num);
            exit(1);
        }
    }
    return num;
}

size_t print_char_vec(char *const argv[])
{
    while(*argv) {
        printf("%s---", *argv);
        ++argv;
    }
    printf("\n");
}

__attribute__((constructor)) static void init()
{
    printf("[INIT Env LD_PRELOAD]: %s\n", getenv("LD_PRELOAD"));
    if(!getenv(MYPRELOADSO)) {
        printf("Please set "MYPRELOADSO" to the path of libmy_execve.so before loading it.\n");
        exit(1);
    }
    strcpy(path_libmy_execev_so, getenv(MYPRELOADSO));
    len_preload_env_name = strlen(PRELOAD_ENV_NAME);
    len_path_libmy_execev_so = strlen(path_libmy_execev_so);
}

int my_preload_ready(char*** envpp, char** envp)
{
    char** tmp_envp = envp;
    while(*tmp_envp) {
        if (!strncmp(*tmp_envp, PRELOAD_ENV_NAME "=", len_preload_env_name+1)) {
            if (!strncmp((*tmp_envp)+len_preload_env_name+1, path_libmy_execev_so, len_path_libmy_execev_so) &&
                (
                    *((*tmp_envp)+len_preload_env_name+1+len_path_libmy_execev_so) == ' ' ||
                    *((*tmp_envp)+len_preload_env_name+1+len_path_libmy_execev_so) == '\0'
                )
               ) {
                return 1;
            }
            break;
        }
        ++tmp_envp;
    }
    *envpp = tmp_envp;
    return 0;
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
    size_t arg_num = count_char_vec(argv);
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
    char** preload_envp;
    // printf("-------------------------------------\n");
    // print_char_vec(environ);
    // print_char_vec(envp);
    // printf("-------------------------------------\n");
    if (!my_preload_ready(&preload_envp, (char**)envp)) {
        printf("xxxxxx\n");
        printf("%s\n", *preload_envp);
        if (*preload_envp) {
            char* old_preload_value = (*preload_envp) + len_preload_env_name + 1;
            // char new_preload_name_and_value[len_preload_env_name+1+len_path_libmy_execev_so+strlen(old_preload_value)+1];
            printf("%d\n", len_preload_env_name+1+len_path_libmy_execev_so+strlen(old_preload_value)+1);
            char new_preload_name_and_value[29];
            // char new_preload_name_and_value[100]="LD_PRELOAD=./libmy_execve.so";
            // char new_preload_name_and_value[100]={0};
            // sprintf(new_preload_name_and_value, "%s=%s", PRELOAD_ENV_NAME, "./libmy_execve.so");
            sprintf(new_preload_name_and_value, "%s=%s%s", PRELOAD_ENV_NAME, path_libmy_execev_so, old_preload_value);
            new_preload_name_and_value[len_preload_env_name+1+len_path_libmy_execev_so+strlen(old_preload_value)] = 0;
            new_preload_name_and_value[28] = 0;
            printf("[new_preload_name_and_value]: %s\n", new_preload_name_and_value);
            *preload_envp = new_preload_name_and_value;
            // print_char_vec(envp);
        }
    }
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
