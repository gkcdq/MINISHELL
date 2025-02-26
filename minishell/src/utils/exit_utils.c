#include "../../minishell.h"

int	check_simple_exit(char *input)
{
	int	i;

	i = 0;
	while (input[i] <= 32)
		i++;
	while (input[i] > 32)
		i++;
	return (0);
}

void	handle_exit_no_args(char **args, t_ee *ee, t_token *exit)
{
	ft_printf("🏃 exit\n");
	if (args)
		free_split(args);
	free(exit);
	ee->minishell_check = 1;
}

void	handle_exit_syntax_error(void)
{
	printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;;'\n");
}

void	handle_exit_numeric_error(char **args, t_ee *ee, t_token *exit)
{
	ft_printf("🏃 exit\n");
	ft_printf("🚧_(⊙_⊙;)_🚧 : exit: %s: numeric argument required\n", args[1]);
	free_split(args);
	free(exit);
	ee->code_exit = 2;
	ee->minishell_check = 1;
}

void	handle_exit_with_numeric_value(char **args, t_ee *ee, t_token  *exit)
{
	ft_printf("🏃 exit\n");
	if (check_atoi_overflow(args[1]) == 1)
		ee->code_exit = 2;
	else
		ee->code_exit = ft_atoi(args[1]);
	free_split(args);
	free(exit);
	ee->minishell_check = 1;
}

void	handle_exit_with_status_value(char **args, t_ee *ee, t_token *exit)
{
	ft_printf("🏃 exit\n");
	ee->code_exit = ee->signal;
	free_split(args);
	free(exit);
	ee->minishell_check = 1;
}

void	handle_exit_too_many_args(t_token *exit, char **args)
{
	ft_printf("🏃 exit\n");
	ft_printf("💥_(╬ಠ益ಠ)_💥: exit: too many arguments\n");
	free_split(args);
	free(exit);
}
