#include "../minishell.h"

int ft_strlonglen(char **s)
{
    int i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

void	init_global()
{
	g_minishell_check = 0;
    g_directory_changed = 0;
}