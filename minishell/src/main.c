#include "../minishell.h"

int		g_omg_le_plus_beau_du_tieks_ca_dit_koi_le_sang_trkl_la_bess_j_ai_vu_tu_connais_ici_c_est_la_debrouille;

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
			{
				tok->token = 3;
				return (1);
			}
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

void	execute_external_command(char *command);

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
	else if ((ft_strcmp(trimmed_input, "cd") == 0) || (ft_strcmp(trimmed_input,
				"~") == 0))
	{
		if (ee->copy_oldpwd)
			free(ee->copy_oldpwd);
		ft_cd(input, ee);
	}
	else if (ft_strcmp(trimmed_input, "env") == 0)
		ft_env(ee);
	else if (ft_strcmp(trimmed_input, "unset") == 0)
		ft_unset(input, ee);
	else if ((ft_strcmp(trimmed_input, "export") == 0)
		|| (ft_strcmp(trimmed_input, "export=") == 0))
		ft_export(input, ee);
	else if (ft_strcmp(trimmed_input, "echo") == 0)
		ft_echo(input);
	else
	{
		if (ee->minishell_check == 0)
			execute_external_command(input);
	}
	free(token);
	free(trimmed_input);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	while (dest[i])
		i++;
	j = 0;
	while (src[j])
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*find_command_path(char *command)
{
	char	*path_env;
	char	**dirs;
	char	*full_path;
	int		i;
	char	temp_path[2024];

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	full_path = NULL;
	i = 0;
	while (dirs[i])
	{
		temp_path[0] = '\0';
		ft_strcat(temp_path, dirs[i]);
		ft_strcat(temp_path, "/");
		ft_strcat(temp_path, command);
		if (access(temp_path, X_OK) == 0)
		{
			full_path = ft_strdup(temp_path);
			break ;
		}
		i++;
	}
	free_split(dirs);
	return (full_path);
}

void	execute_external_command(char *command)
{
	char	**args;
	char	*path;
	pid_t	pid;
	int		status;

	args = ft_split(command, ' ');
	path = find_command_path(args[0]);
	if (!path)
	{
		ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", args[0]);
		free_split(args);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (execv(path, args) == -1)
		{
			perror("execv");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
	}
	else
		perror("fork");
	free(path);
	free_split(args);
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

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	catch_signal(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	you_shall_not_path(void)
{
	char	*path_env;

	path_env = getenv("PATH");
	if (!path_env || *path_env == '\0')
		setenv("PATH", "/bin:/usr/bin", 1);
}

/*char **parse_dollars(char **args, t_ee *ee)
{
    int i, j, k, x, y, m;
    char *copy;
    char **changed_args;
    char *after_equal;
	char *before_equal;
	int lock = 0;

    changed_args = malloc(sizeof(char *) * (ft_strlonglen(args) + 1));
    if (!changed_args)
        return NULL;

    i = 0;
    while (args[i])
    {
        if (args[i][0] == '$')
        {
            copy = malloc(sizeof(char) * ft_strlen(args[i]));
            if (!copy)
                return NULL;
            j = 1;
            k = 0;
            while (args[i][j])
            {
                copy[k++] = args[i][j++];
            }
            copy[k] = '\0';
            x = 0;
            while (ee->envp[x])
            {
                y = 0;
                if (ft_strncmp(ee->envp[x], copy, ft_strlen(copy)) == 0 && ee->envp[x][ft_strlen(copy)] == '=')
                {
                    y = ft_strlen(copy) + 1;
                    after_equal = ft_strdup(ee->envp[x] + y);
                    if (!after_equal)
                        return NULL;
                    changed_args[i] = after_equal;
					lock = 1;
                    break;
                }
                x++;
            }
            if (!ee->envp[x]) 
                changed_args[i] = ft_strdup(args[i]);
            free(copy);
        }
        else
        {
            changed_args[i] = ft_strdup(args[i]);
        }
		j = 0;
		///////////////////////////////////////////////////////
		if (args[i][0] != '$' && lock == 0)
		{
			while(args[i][j] != '$')
				j++;
			printf("%c\n", args[i][j]);
			if(args[i][j] == '$')
			{
				m = j - 1;
				j = 0;
				before_equal = malloc(sizeof(char) * (m + 1));
				int h = 0;
				while(j < m)
				{
					before_equal[h] = args[i][j];
					j++;
					h++;
				}
				before_equal[h] = '\0';
				changed_args[i] = before_equal;
				//free(before_equal);
			}
		}
		/////////////////////////////////////////////////////////
        i++;
    }

    changed_args[i] = NULL; 
    return changed_args;
}*/
char **parse_dollars(char **args, t_ee *ee)
{
    int i, j, k, x, y, m;
    char *copy;
    char **changed_args;
    char *after_equal;
    char *before_equal;
    int lock;

    changed_args = malloc(sizeof(char *) * (ft_strlonglen(args) + 1));
    if (!changed_args)
        return NULL;

    i = 0;
    while (args[i])
    {
        lock = 0;
        if (args[i][0] == '$')
        {
            copy = malloc(sizeof(char) * ft_strlen(args[i]));
            if (!copy)
                return NULL;
            j = 1;
            k = 0;
            while (args[i][j])
            {
                copy[k++] = args[i][j++];
            }
            copy[k] = '\0';
            x = 0;
            while (ee->envp[x])
            {
                y = 0;
                if (ft_strncmp(ee->envp[x], copy, ft_strlen(copy)) == 0 && ee->envp[x][ft_strlen(copy)] == '=')
                {
                    y = ft_strlen(copy) + 1;
                    after_equal = ft_strdup(ee->envp[x] + y);
                    if (!after_equal)
                        return NULL;
                    changed_args[i] = after_equal;
                    lock = 1;
                    break;
                }
                x++;
            }
            if (!lock)
                changed_args[i] = ft_strdup(args[i]);
            free(copy);
        }
        else
        {
            j = 0;
            while (args[i][j] && args[i][j] != '$')
                j++;

            if (args[i][j] == '$') // Trouver un `$` dans la chaîne
            {
                m = j; // Longueur avant `$`
                before_equal = malloc(sizeof(char) * (m + 1));
                if (!before_equal)
                    return NULL;

                k = 0;
				while (k < m)
				{
                    before_equal[k] = args[i][k];
					k++;
				}
                before_equal[m] = '\0';

                changed_args[i] = before_equal;
                lock = 1; // Marquer que nous avons traité ce cas
            }

            if (!lock) // Si aucun `$` n'a été trouvé
                changed_args[i] = ft_strdup(args[i]);
        }

        i++;
    } //for (k = 0; k < m; k++) 

    changed_args[i] = NULL; 
    return changed_args;
}


char **check_dollars(char *input, t_ee *ee)
{
    char **args;
    char **changed_args;

    args = ft_split(input, ' ');
    if (!args)
        return NULL;
    changed_args = parse_dollars(args, ee);
    if (!changed_args)
        return NULL;
    free_split(args);
    return changed_args;
}

char *reconstruct_input(char **changed_args)
{
    int i = 0;
    size_t total_len = 0;
    char *new_input;
    char *current_pos;

    while (changed_args[i])
    {
        total_len += ft_strlen(changed_args[i]) + 1;
        i++;
    }

    new_input = malloc(sizeof(char) * total_len);
    if (!new_input)
        return NULL;
    current_pos = new_input;
    i = 0;
    while (changed_args[i])
    {
        size_t len = ft_strlen(changed_args[i]);
        ft_memcpy(current_pos, changed_args[i], len);
        current_pos += len;
        if (changed_args[i + 1])
            *current_pos++ = ' ';
        i++;
    }

    *current_pos = '\0';
    return new_input;
}



void	loop(char *input, t_ee *ee)
{
	t_token	*tok;
	char	**args;
	char **changed_args = NULL;

	if ((!ee->envp || !ee->envp[0]) && ee->lock_path == 0)
		you_shall_not_path();
	tok = malloc(sizeof(t_token));
	tok->found = 0;
	args = ft_split(input, ' ');
	input = readline("🍀_(^o^)_🍀  > ");
	if (input == NULL)
		ee->minishell_check = 1;
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
					else if (tok->token == 3)
						printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;;'\n");
					free(input);
					free(tok);
					return ;
				}
				changed_args = check_dollars(input, ee);
                if (!changed_args)
                    return;
				free(input);
				input = reconstruct_input(changed_args);
                cumulate_token(input, ee);
			}
			else
			{
				changed_args = check_dollars(input, ee);
                if (!changed_args)
                    return;
				free(input);
				input = reconstruct_input(changed_args);
                interprete_commande(input, ee);
			}
		}
	}
	free_split(changed_args);
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

char	**copy_envp(char **envp)
{
	int		i;
	char	**copy;

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
	int		i;

	ee = malloc(sizeof(t_ee));
	input = NULL;
	(void)ac;
	(void)av;
	init_struct(ee);
	ee->envp = copy_envp(envp);
	while (ee->minishell_check == 0)
	{
		catch_signal();
		loop(input, ee);
	}
	i = 0;
	while (ee->envp[i])
	{
		free(ee->envp[i]);
		i++;
	}
	if (ee->copy_oldpwd)
		free(ee->copy_oldpwd);
	if (ee->copy_pwd)
		free(ee->copy_pwd);
	if (ee->envp)
		free(ee->envp);
	free(ee);
	clear_history();
	return (0);
}
