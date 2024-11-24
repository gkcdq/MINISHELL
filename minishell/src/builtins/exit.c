#include "../../minishell.h"

void	ft_exit(char *input)
{
 	char **args;

	args = ft_split(input, ' ');
    printf("%s", args[1]);
	if (args[1] == NULL && args[2] != NULL && args[3] == NULL)     
	{
        ft_printf("🏃 ALAIDE\n");
		ft_printf("🏃 exit\n");
		free(input);
		free_split(args);
		g_minishell_check = 1;        
	}
	else if (args[2] != NULL)
	{
		ft_printf("minishell: exit: too many arguments\n");
		g_minishell_check = 0;
	}
	else if (args[1] && !is_number(args[1]))
	{
		ft_printf("🏃 exit\n");
		ft_printf("minishell: exit: %s: numeric argument required\n", args[1]);
		free_split(args);
		g_minishell_check = 1; 
	}
	else 
	{
		ft_printf("🏃 exit\n");
		free_split(args);
		g_minishell_check = 1; 
	}
}