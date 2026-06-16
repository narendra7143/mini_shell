#include"header.h"

void execute_external_commands(char *input_string)
{

    char *argv[20];
    int i = 0, j = 0;
    int flag = 0;   // 0 = space, 1 = word

    while (input_string[i] != '\0')
    {
       
        if (input_string[i] != ' ' && flag == 0)
        {
            argv[j++] = &input_string[i];   // word start
            flag = 1;
        }

        if (input_string[i] == ' ' && flag == 1)
        {
            input_string[i] = '\0';         // end of word
            flag = 0;
        }

        i++;
    }
    argv[j] = NULL;

    // execvp(argv[0], argv); 
    // checking pipe is present or not
   
    int pipe_count=0;
    i=0;
    while (argv[i] != NULL)
    {
       
     
        if(strcmp(argv[i],"|")==0)
        {
            pipe_count++;
        }
        i++;
     }
   
    if(pipe_count == 0)
{
    execvp(argv[0], argv);
    perror("execvp");   
    exit(1);            
}


     else 
     {

      //  printf("pipe count %d \n",pipe_count);
        
        int cmd[20];                            //create array
        int cmd_count=0;
        cmd[cmd_count++] = 0;   

        for(int i=0;argv[i]!=NULL;i++)
        {
            if(strcmp(argv[i],"|")==0)                      //checking pipe is present or not
            {
                argv[i] = NULL;                               //'|' pipe is present make it NULl
                cmd[cmd_count++] = i+1;                                // next command is  wc
                                         
            }  
        }
        int fd_in = 0;
        for(int i=0;i<=pipe_count;i++)              
        {
            int pipe_fd[2];

            if(i!=pipe_count)
            {
                pipe(pipe_fd);
            }

            int pid = fork();

            if(pid>0)                                               //parent processs
            {
                wait(NULL);

                if(i!=pipe_count)
                {
                    // dup2(pipe_fd[0],0);
                    close(pipe_fd[1]);
                    if(fd_in!=0)
                    {
                        close(fd_in);
                    }
                    fd_in = pipe_fd[0];
                }  
            }

            else if(pid==0)                                     //child process
            {
                dup2(fd_in, 0); 
                if(i!=pipe_count)
                {
                    dup2(pipe_fd[1],1);
                     close(pipe_fd[0]);
                }
                execvp(argv[cmd[i]],argv+cmd[i]);
                exit(1);
            }
        }
    }   
}




void extract_external_commands(char external_commands[152][15])
{
     memset(external_commands, 0, sizeof(char) * 152 * 15);
    int fd = open("ext_cmd.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return;
    }

    int i = 0, j = 0;
    char ch;

    while (read(fd, &ch, 1) == 1)
    {
        if (ch == '\n')
        {
            external_commands[i][j] = '\0';
            trim_string(external_commands[i]);  // Trim the line
            i++;
            j = 0;
            if (i >= 152)
                break;
        }
        else if (j < 14)
        {
            external_commands[i][j++] = ch;
        }
    }

    if (j > 0 && i < 152)
    {
        external_commands[i][j] = '\0';
        trim_string(external_commands[i]);  // Trim last line
        i++;
    }

    close(fd);
}


void trim_string(char *str)
{
    int start = 0;
    int end = strlen(str) - 1;

    while (str[start] == ' ' || str[start] == '\t')
        start++;

    while (end > start && 
          (str[end] == ' ' || str[end] == '\t' || str[end] == '\r' || str[end] == '\n'))
        end--;

    int i = 0;
    while (start <= end)
        str[i++] = str[start++];

    str[i] = '\0';
}
