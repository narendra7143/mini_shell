#include"header.h"
char external_commands[152][15];
char prompt[25] = "minishell$";
pid_t pid=0; 
int status=0;
volatile sig_atomic_t sigint_received  = 0;
volatile sig_atomic_t sigtstp_received = 0;
volatile sig_atomic_t sigchld_received = 0;
void scan_input(char *prompt, char *input_string)
{
    extract_external_commands(external_commands);
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGCHLD, signal_handler);
    while (1)
    {
        if(sigint_received)
        {
            sigint_received = 0;
            if (fg_pid > 0)
                kill(fg_pid, SIGINT);
            write(1, "\n", 1);
        }

        if(sigtstp_received)
        {
            sigtstp_received = 0;
            if (fg_pid > 0)
            {
                kill(fg_pid, SIGTSTP);
                add_job(fg_pid, input_string);
                fg_pid = -1;
            }
            write(1, "\n", 1);
        }

        if(sigchld_received)
        {
            sigchld_received = 0;
            pid_t pid;
            while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
                remove_job(pid);
        }
        printf("\n%s ", prompt);

        if (fgets(input_string, 100, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        input_string[strcspn(input_string, "\n")] = '\0';
        trim_string(input_string);
        if (strlen(input_string) == 0)
            continue;
     
        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            if (strchr(input_string + 4, ' '))
                printf("Error: space not allowed in prompt\n");
            else
                strcpy(prompt, input_string + 4);
            continue;
        }

        char *cmd = get_command(input_string);
        int type=check_command_type(cmd,external_commands);
        if (type== BUILTIN)
        {
            printf("%s is a builtin command\n",cmd);
            execute_internal_commands(input_string);
        }
        else if(type==EXTERNAL)
        {
            pid = fork();
            if (pid == 0)
            {
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                printf("%s is a external command\n",cmd);
                execute_external_commands(input_string);
            }
            else if(pid > 0)
            {
                int status;
                waitpid(pid, &status, WUNTRACED);
                char cmd_copy[100];
                strcpy(cmd_copy, input_string);
                if(WIFSTOPPED(status))
                {
                    add_job(pid, cmd_copy);
                }

            }
        }
        else
            printf("command %s not found\n",cmd);
    }
}

void signal_handler(int signum)
{
    
    if(signum==SIGINT)
    {
        if(pid==0)
        {
            printf("\n%s ",prompt);
            fflush(stdout);
        }
    }
     if(signum==SIGTSTP)
    {
        if(pid==0)
        {
            printf("\n%s ",prompt);
            fflush(stdout);
        }
    }
    if(signum==SIGCHLD)
    {
        fflush(stdout);
    }
}


