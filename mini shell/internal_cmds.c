#include"header.h"
job_t *head = NULL;
pid_t fg_pid = -1;

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","jobs","fg","bg", NULL};
void execute_internal_commands(char *input)
{
    char buf[50];
	job_t *s;
    int status;
    if (strncmp(input, "exit", 4) == 0)
        exit(0);

    if (strncmp(input, "pwd", 3) == 0)
    {
        getcwd(buf, sizeof(buf));
        printf("%s\n", buf);
        return;
    }

	if (strncmp(input, "cd", 2) == 0)
	{
		char *path = input + 2;
		trim_string(path);

		if (strlen(path) == 0)
			chdir(getenv("HOME"));
		else
			chdir(path);

		getcwd(buf,50);
		printf("%s\n",buf);
		return;
	}


    if(strcmp(input,"echo $$")==0)
    {
        printf("%d\n",getpid());
        return;
    }

    if(strcmp(input,"echo $?")==0)
    {
        if(WIFEXITED(status))
        printf("%d\n",WEXITSTATUS(status));
        else
        printf("1\n");
        return;
    }
    
    if(strcmp(input,"echo $shell")==0)
    {
        printf("%s\n",getenv("SHELL"));
        return;
    }
	
	if(strcmp(input,"jobs")==0)
	{
		print_jobs();
	}

	if (strcmp(input, "fg") == 0)
    {
        s = head;
        if (s)
        {
            fg_pid = s->pid;
            kill(s->pid, SIGCONT);
            waitpid(s->pid, &status, WUNTRACED);
            delete_first_job();
            fg_pid = -1;
        }
    }

    if (strcmp(input, "bg") == 0)
    {
        s = head;
        if (s)
        {
            kill(s->pid, SIGCONT);
            delete_first_job();
        }
    }
}						

void add_job(pid_t pid, char *cmd)
{
    job_t *new = malloc(sizeof(job_t));
    new->pid = pid;
    strcpy(new->cmd, cmd);
    new->next = head;
    head = new;
}
void remove_job(pid_t pid)
{
    job_t *temp = head, *prev = NULL;

    while(temp)
    {
        if(temp->pid == pid)
        {
            if(prev)
                prev->next = temp->next;
            else
                head = temp->next;

            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}
void print_jobs()
{
    job_t *temp = head;
    int i = 1;

    while(temp)
    {
        printf("[%d] %d %s\n", i++, temp->pid, temp->cmd);
        temp = temp->next;
    }
}

void delete_first_job()
{
    if(head)
    {
        job_t *temp = head;
        head = head->next;
        free(temp);
    }
}