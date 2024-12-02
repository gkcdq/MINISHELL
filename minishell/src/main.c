#include "../minishell.h"

int		g_minishell_check;
int		g_directory_change;

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

char	*skip_isspace_for_fonctions(char *input, t_token *token)
{
	char	*s;
	int		i;

	i = 0;
	if (check_token(input, token) == 1)
		return (token->symbol);
	else
	{
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
	}
	return (s);
}

int	check_token_in_all_string(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ';' && input[i + 1] == ';')
			return (1);
		i++;
	}
	return (0);
}

int	check_token(char *input, t_token *token)
{
	int	i;

	i = 0;
	while (input[i] <= 32)
		i++;
	if (input[i] == ';' && input[i + 1] != ';')
	{
		token->token = 1;
		token->symbol = ";";
		return (1);
	}
	/*while (input[i])
	{
		if (input[i] == ';' && input[i + 1] == ';')
		{
			token->token = 1;
			token->symbol = ";;";
			return (1);
		}
		i++;
	}*/
	return (0);
}

int	token_found(char *input, t_token *tok)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] <= 32)
		{
			if (input[i] == '\0')
				return (0);
			i++;
		}
		if (input[i] == ';' && input[i + 1] == '\0')
			return (0);
		if (input[i] == ';' && input[i + 1] != '\0')
		{
			tok->found = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

////////////////////////////////////////////////////////

void	interprete_commande(char *input, char **envp)
{
	char	*trimmed_input;
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return ;
	token->token = 0;
	trimmed_input = skip_isspace_for_fonctions(input, token);
	if (!trimmed_input)
	{
		free(token);
		return ;
	}
	if (token->token == 1)
	{
		printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token ';'\n");
		free(token);
		return ;
	}
	else if (ft_strcmp(trimmed_input, "exit") == 0)
	{
		if (ft_strcmp(trimmed_input, "exit") == 0 && ft_strcmp(trimmed_input,
				input) == 0)
		{
			printf("🏃 exit\n");
			//free(token);
			g_minishell_check = 1;
		}
		else
		{
			ft_exit(input);
			// free(token);
		}
	}
	else if (ft_strcmp(trimmed_input, "pwd") == 0)
		ft_pwd();
	else if ((ft_strncmp(trimmed_input, "cd", 2) == 0)
		|| (ft_strcmp(trimmed_input, "~") == 0))
		ft_cd(input);
	else if (ft_strcmp(trimmed_input, "ls") == 0)
		ft_ls(input);
	else if (ft_strcmp(trimmed_input, "clear") == 0)
		ft_clear(input);
	else if (ft_strcmp(trimmed_input, "env") == 0)
		ft_env(envp);
	else
	{
		if (g_minishell_check == 0)
			ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", trimmed_input);
	}
	free(token);
	free(trimmed_input);
}

void	cumulate_token(char *input, char **envp)
{
	static int	i = 0;
	char		copy[1024];
	int			j;

	j = 0;
	// printf("Input: %s\n", input);
	// printf("Copy: %s\n", copy);
	if (!input || input[i] == '\0')
		return ;
	while (input[i] != '\0' && input[i] != ';')
		copy[j++] = input[i++];
	copy[j] = '\0';
	// printf("Input: %c\n", input[i]);
	// printf("Copy: %s\n", copy);
	interprete_commande(copy, envp);
	// printf("%c\n", input[i]);
	if (input[i] == ';')
		i++;
	if (input[i] != '\0')
	{
		// printf("input de fin %c\n", input[i]);
		cumulate_token(input, envp);
	}
	else
	{
		i = 0;
		// printf("input apres reset %c\n", input[i]);
	}
	return ;
}

void	loop(char *input, char **envp)
{
	t_token	*tok;

	// int i = 0;
	tok = malloc(sizeof(t_token));
	tok->found = 0;
	tok->stop = 0;
	input = readline("🍀_(^o^)_🍀  > ");
	if (input && *input)
	{
		add_history(input);
		if (token_found(input, tok) == 1)
		{
			// i = 1;
			// printf("%d\n", i);
			if (check_token_in_all_string(input) == 1)
			{
				printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;; KOKO'\n");
				free(input);
				free(tok);
				return ;
			}
			cumulate_token(input, envp);
		}
		else
			interprete_commande(input, envp);
	}
	free(input);
	free(tok);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;

	input = NULL;
	(void)ac;
	(void)av;
	init_global(envp);
	while (g_minishell_check == 0)
	{
		loop(input, envp);
	}
	clear_history();
	return (0);
}
