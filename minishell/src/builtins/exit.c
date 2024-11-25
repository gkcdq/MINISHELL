#include "../../minishell.h"

void	ft_exit(char *input)
{
	char	**args;

	args = ft_split(input, ' ');
	if (args[1] && !is_number(args[1]))
	{
		ft_printf("exit\n");
		ft_printf("🚧_(⊙_⊙;)_🚧 : exit: %s: numeric argument required\n", args[1]);
		free_split(args);
		g_minishell_check = 1;
	}
	else if (args[2] != NULL)
	{
		ft_printf("exit\n");
		ft_printf("💥_(╬ಠ益ಠ)_💥: exit: too many arguments\n");
	}
	else
	{
		ft_printf("🏃 exit\n");
		free_split(args);
		g_minishell_check = 1;
	}
}
