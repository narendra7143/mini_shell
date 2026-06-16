

/******************************************************************************
 * Project Name : MINI SHELL
 * Author       : DAINDE.NARENDRA KUMAR
   DATE         : 03-02-2026
 * Description  :
 * ----------------------------------------------------------------------------
 * Mini Shell is a simplified implementation of a UNIX/Linux command-line shell,
 * developed using C and Linux system programming concepts. This project mimics
 * the core behavior of standard shells like bash, providing an interactive
 * command prompt to execute user commands.
 *
 * The shell supports both internal (built-in) and external commands. Internal
 * commands such as cd, pwd, exit, echo $$, echo $?, echo $SHELL, jobs, fg, and bg
 * are handled within the shell process itself, while external commands are
 * executed using child processes created with fork() and execvp().
 *
 * Advanced features such as multiple command pipelines (n-pipe), signal
 * handling, job control, and process synchronization are implemented to closely
 * resemble real shell behavior. The shell correctly handles signals like
 * SIGINT (Ctrl+C), SIGTSTP (Ctrl+Z), and SIGCHLD to manage foreground and
 * background processes without terminating the shell itself.
 *
 * The project also maintains a linked list of background and stopped jobs,
 * allowing users to list jobs, move jobs to the foreground, or resume them in
 * the background. Proper trimming and parsing of input strings ensures reliable
 * execution of commands, especially in piped operations.
 *
 * Key System Calls and APIs Used:
 *   - fork(), execvp(), wait(), waitpid()
 *   - pipe(), dup2()
 *   - signal(), kill()
 *   - getcwd(), chdir(), getenv()
 *
 * This project demonstrates a strong understanding of Linux process management,
 * inter-process communication, signal handling, and shell design fundamentals.
 *
 * Platform     : Linux
 * Language     : C
 *
 *****************************************************************************/
*/
#include "header.h"

int main()
{
    system("clear");
   char  prompt[25] = "minishell$";
    char input_string[100];

    /* Start shell */
    scan_input(prompt, input_string);

    return 0;
}
