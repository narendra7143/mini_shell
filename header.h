
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
/* --------- job structure -------*/
typedef struct job
{
    pid_t pid;
    char cmd[100];
    struct job *next;
} job_t;

/* -------- global variables (extern ONLY) -------- */
extern job_t *head;
extern pid_t fg_pid;
extern int status;


#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

void copy_change(char *prompt, char *input_string);
void echo(char *input_string, int status);
void execute_internal_commands(char *);
void execute_external_commands(char *);
void execute_n_pipe(char *argv[], int argc);
void signal_handler(int sig_num);
void extract_external_commands(char external_commands[152][15]);
int check_command_type(char *cmd,char external_commands[152][15]);
void trim_string(char *);

void add_job(pid_t pid, char *cmd);
void remove_job(pid_t pid);
void print_jobs(void);
void delete_first_job(void);
#endif