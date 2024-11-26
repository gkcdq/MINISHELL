#include "../../minishell.h"

char	*parse_input_exit(char *input)
{
	int i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ';')
		{
			input[i] = '\0';
			break;
		}
		else
			i++;
	}
	return (input);
}

void	ft_exit(char *input)
{
	char	**args;

	input = parse_input_exit(input);
	args = ft_split(input, ' ');
	if ((args[1] && !is_number(args[1])) || (args[1][0] == '-' || args[1][0] == '+'))
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
