#include "../minishell.h"

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
		return (";");
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

int	check_token_in_all_string(char *input, t_token *tok)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ';')
		{
			if (input[i + 1] == ';')
				return (1);
			while (input[i + 1] <= 32)
				i++;
			if (input[i + 1] == ';')
			{
				tok->token = 2;
				return (1);
			}
		}
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
		return (1);
	}
	return (0);
}

int	token_found(char *input, t_token *tok)
{
	int	i;

	i = 0;
	while (input[i] <= 32)
		i++;
	if (input[i] == '\0')
		return (0);
	while (input[i])
	{
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

void	interprete_commande(char *input, t_ee *ee)
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
			ee->minishell_check = 1;
		}
		else
			ft_exit(input, ee);
	}
	else if (ft_strcmp(trimmed_input, "pwd") == 0)
		ft_pwd();
	else if ((ft_strcmp(trimmed_input, "cd") == 0) || (ft_strcmp(trimmed_input, "~") == 0))
	{
		if (ee->copy_oldpwd)
			free(ee->copy_oldpwd);
		if(ee->copy_pwd)
			free(ee->copy_pwd);
		ft_cd(input, ee);
	}
	else if (ft_strcmp(trimmed_input, "ls") == 0)
		ft_ls(input);
	else if (ft_strcmp(trimmed_input, "clear") == 0)
		ft_clear(input);
	else if (ft_strcmp(trimmed_input, "env") == 0)
		ft_env(ee);
	else if (ft_strcmp(trimmed_input, "wc") == 0)
		ft_wc();
	else if (ft_strcmp(trimmed_input, "unset") == 0)
		ft_unset(input, ee);
	else if ((ft_strcmp(trimmed_input, "export") == 0) || (ft_strcmp(trimmed_input, "export=") == 0))
		ft_export(input, ee);
	else
	{
		if (ee->minishell_check == 0)
			ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", trimmed_input);
	}
	free(token);
	free(trimmed_input);
}


void	cumulate_token(char *input, t_ee *ee)
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
	interprete_commande(copy, ee);
	// printf("%c\n", input[i]);
	if (input[i] == ';')
		i++;
	if (input[i] != '\0')
	{
		// printf("input de fin %c\n", input[i]);
		cumulate_token(input, ee);
	}
	else
	{
		i = 0;
		// printf("input apres reset %c\n", input[i]);
	}
	return ;
}

void	loop(char *input, t_ee *ee)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	tok->found = 0;
	input = readline("🍀_(^o^)_🍀  > ");
	if (input && *input)
	{
		add_history(input);
		if (check_string(input) == 0)
		{
			if (token_found(input, tok) == 1)
			{
				if (check_token_in_all_string(input, tok) == 1)
				{
					if (tok->token == 2)
					{
						printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;'\n");
						tok->token = 0;
					}
					else
						printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;;'\n");
					free(input);
					free(tok);
					return ;
				}
				cumulate_token(input, ee);
			}
			else
				interprete_commande(input, ee);
		}
	}
	free(input);
	free(tok);
}

int	check_string(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] > 32)
			return (0);
		i++;
	}
	return (1);
}

char **copy_envp(char **envp)
{
    int i;
    char **copy;

	copy = malloc(sizeof(char *) * (ft_strlonglen(envp) + 1));
	i = 0;
	check_variable_oldpwd(envp);
    while (envp[i])
	{
        copy[i] = ft_strdup(envp[i]);
		i++;
	}
    copy[i] = NULL;
    return (copy);
}


int	main(int ac, char **av, char **envp)
{
	t_ee	*ee;
	char	*input;

	ee = malloc(sizeof(t_ee));
	input = NULL;
	(void)ac;
	(void)av;
	init_struct(ee);
	ee->envp = copy_envp(envp);
	while (ee->minishell_check == 0)
	{
		loop(input, ee);
	}
	int i = 0;
	while (ee->envp[i])
	{
		free(ee->envp[i]);
		i++;
	}
	if (ee->copy_oldpwd)
		free(ee->copy_oldpwd);
	if(ee->copy_pwd)
		free(ee->copy_pwd);
	free(ee->envp);
	free(ee);
	clear_history();
	return (0);
}
