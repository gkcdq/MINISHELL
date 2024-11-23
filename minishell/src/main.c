#include "../minishell.h"

int g_minishell_check;

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
char	*skip_isspace_for_fonctions(char *input)
{
	char *s;
	int i;

	i = 0;
	s = malloc(sizeof(char) * ft_strlen(input) + 1);
	while (*input <= 32)
	{
		input++;
	}
	//ft_printf("%c\n", *input);
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
	input = skip_isspace_for_fonctions(input);
	if (ft_strcmp(input, "exit") == 0)
	{
		free(input);
		ft_exit();
	}
	else if (ft_strcmp(input, "pwd") == 0)
	{
	free(input);
		ft_pwd();
	}
	else
		ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", input);
}

int	main(int ac, char **av, char **envp)
{
	char *input;

    (void)ac;
    (void)av;
    (void)envp;
    init_global();
	while (g_minishell_check == 0)
	{
		input = readline("🍀_(^o^)_🍀  > ");
		if (input && *input)
		{
			add_history(input);
			interprete_commande(input);
		}
		free(input);
	}
	clear_history();
	return (0);
}
