#include "../minishell.h"

int ft_strlonglen(char **s)
{
    int i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

void	init_global(char **envp)
{
	int i = 0;
	g_minishell_check = 0;
    g_envp = malloc(sizeof(char *) * (ft_strlonglen(envp) + 1));
	while (envp[i])
	{
		g_envp[i] = ft_strdup(envp[i]);
		i++;
	}
    g_envp[i] = NULL;
}