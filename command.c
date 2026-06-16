#include"header.h"

char *get_command(char *input)
{
    while (*input == ' ')
        input++;

    static char cmd[50];
    int i = 0;

    while (input[i] && input[i] != ' ')
    {
        cmd[i] = input[i];
        i++;
    }
    cmd[i] = '\0';
    return cmd;
}

int check_command_type(char *cmd,char external_commands[152][15])
{
    extern char *builtins[];
    for(int i=0;builtins[i]!=NULL;i++)
    {
        if(strcmp(builtins[i],cmd)==0)
        return BUILTIN;
    }
    int i=0;
    while(i<152 && external_commands[i][0] != '\0')
    {
        if(strcmp(external_commands[i],cmd)==0)
        return EXTERNAL;
        i++;
    }
    return NO_COMMAND;
}
