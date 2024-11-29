#include "../minishell.h"

int		g_minishell_check;

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		else
			i++;
	}
	return (0);
}
//    utils.c    /////////////////////////////////////////

int	is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	free_split(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*skip_isspace_for_fonctions(char *input)
{
	char	*s;
	int		i;

	i = 0;
	s = malloc(sizeof(char) * ft_strlen(input) + 1);
	while (*input <= 32)
		input++;
	while (*input > 32)
	{
		s[i] = *input;
		i++;
		input++;
	}
	s[i] = '\0';
	return (s);
}
////////////////////////////////////////////////////////

void	interprete_commande(char *input)
{
	char	*trimmed_input;

	trimmed_input = skip_isspace_for_fonctions(input);
	// ft_printf("input : %s\n", input);
	// ft_printf("trimmed_input ; %s\n", trimmed_input);
	if (ft_strcmp(trimmed_input, "exit") == 0)
	{
		if (ft_strcmp(trimmed_input, "exit") == 0 && ft_strcmp(trimmed_input,
				input) == 0)
		{
			printf("🏃 exit\n");
			g_minishell_check = 1;
		}
		else
			ft_exit(input);
	}
	else if (ft_strcmp(trimmed_input, "pwd") == 0)
		ft_pwd();
	else if ((ft_strcmp(trimmed_input, "cd") == 0) || (ft_strcmp(trimmed_input,
				"~") == 0))
		ft_cd(input);
	else if (ft_strcmp(trimmed_input, "ls") == 0)
		ft_ls(input);
	else if (ft_strcmp(trimmed_input, "clear") == 0)
		ft_clear(input);
	else
		ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", trimmed_input);
	free(trimmed_input);
}

void	loop(char *input)
{
	input = readline("🍀_(^o^)_🍀  > ");
	if (input && *input)
	{
		add_history(input);
		interprete_commande(input);
	}
	free(input);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;

	input = NULL;
	(void)ac;
	(void)av;
	(void)envp;
	init_global();
	while (g_minishell_check == 0)
	{
		loop(input);
	}
	clear_history();
	return (0);
}
