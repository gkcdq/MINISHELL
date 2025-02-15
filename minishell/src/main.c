#include "../minishell.h"

static int g_status = 0;


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
	int single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (input[i])
	{
		if (input[i] == ';' && input[i + 1] == ';')
			break ;
		if (input[i] == 39)
			single_quote++;
		if (input[i] == 34)
			double_quote++;
		i++;
	}
	if (single_quote % 2 != 0 || double_quote % 2 != 0)
		return (0);
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{

		}
		if (input[i] == ';')
		{
			if (input[i + 1] == '\0')
				return (0);
			if (input[i + 1] == ';')
			{
				tok->token = 3;
				return (1);
			}
			i++;
			while (input[i] && input[i] <= 32)
				i++;
			if (input[i] == ';')
			{
				tok->token = 2;
				return (1);
			}
			if (input[i] == '\0')
				return (0);
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

void	execute_external_command(char *command, t_ee *ee);


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




void	execute_external_command(char *command, t_ee *ee)
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
		ee->signal = 127;
		if (ee->command_with_and == 1)
			ee->check_and_validity = 1;
		ee->command_with_or = 1;
		ee->confirmed_command = 0;
		free_split(args);
		return ;
	}
	else
		ee->confirmed_command = 1;
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
		if (WIFEXITED(status)) 
		{
    		if (!g_status) 
			{
    	   			ee->signal = WEXITSTATUS(status);
					if (ee->signal != 0 && ee->signal != 1 && ee->signal != 2)
						ee->signal = 128 + ee->signal;
    		}
			else
				ee->signal = 0;
		}
	}
	else
		perror("fork");
	free(path);
	free_split(args);
}

int	find_parenthesis(char *input)
{
	int i = 0;

	while (input[i])
	{
		if (input[i] == ')')
			return (1);
		i++;
	}
	return (0);
}


char *remove_parentheses(char *input) {
    int i = 0, j = 0, level = 0;
    char *tmp = malloc(sizeof(char) * (ft_strlen(input) + 1)); // +1 pour '\0'
    if (!tmp)
        return NULL; // Gestion d'erreur malloc

    while (input[i]) {
        if (input[i] == '(') {
            if (level > 0) // On garde seulement le contenu interne
                tmp[j++] = input[i];
            level++;
        } else if (input[i] == ')') {
            level--;
            if (level > 0) // On garde seulement le contenu interne
                tmp[j++] = input[i];
        } else {
            if (level > 0 || (level == 0 && input[i] != ')'))
                tmp[j++] = input[i];
        }
        i++;
    }
    tmp[j] = '\0';
    return tmp;
}



/*char *remove_parentheses(char *input)
{
    int i = 0;
	int j = 0;
	char *tmp;

	tmp = malloc(sizeof(char) * (ft_strlen(input) - 1));
	while (input[i] && input[i] != '(')
	{
		tmp[j++] = input[i++];
	}
	// je suis sur la parenthese
	i++;
	while (input[i] && input[i] != ')')
	{
		if (input[i] == ')')
			break;
		tmp[j++] = input[i++];
	}
	tmp[j] = '\0';
	return (tmp);

}*/
int	do_you_find_or_what(char *input)
{
	int i = 0;

	while (input[i])
	{
		if (input[i] == '(')
		{
			while (input[i] != ')')
				i++;
		}
		if (input[i] == '|' && input[i + 1] == '|')
			return (1);
		i++;
	}
	return (0);
}


void	its_just_a_parenthese(char *input, t_ee *ee)
{
    char *clean_input;
	char *command_before_or;
	char *command_after_or;
	bool has_parentheses = false;

	if (do_you_find_or_what(input) == 1)
	{
		command_before_or = copy_before_or(input);
		command_after_or = copy_after_or(input);
		has_parentheses = (find_parenthesis(command_after_or) == 1);
        cumulate_token(command_before_or, ee);
		if (ee->signal != 0)
		{
			if (has_parentheses)
			{
				pid_t pid = fork();
				if (pid == -1)
				{
					perror("Erreur fork()");
					free(command_before_or);
					free(command_after_or);
					return;
				}
				if (pid == 0)
				{
					its_just_a_parenthese(command_after_or, ee);
					exit(ee->signal);
				}
				else
				{
					int status;
					waitpid(pid, &status, 0);
					if (WIFEXITED(status))
						ee->signal = WEXITSTATUS(status);
				}
			}
			else
			{
				cumulate_token(command_after_or, ee);
			}
		}
		free(command_before_or);
		free(command_after_or);
		return;
	}
	clean_input = remove_parentheses(input);
	if (!clean_input)
		return;
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Erreur fork()");
        free(clean_input);
        return;
    }
    if (pid == 0)
    {
        cumulate_token(clean_input, ee);
        exit(ee->signal);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            ee->signal = WEXITSTATUS(status);
		//if (ee->signal == 127)
		//	ee->check_and_validity = 1;
    }
    free(clean_input);
}



int cumulate_token(char *input, t_ee *ee)
{
    int i = 0, j = 0;
    char copy[2048];
    char **changed_args;
    bool success = true;
	char *reconstructed_input;
	
    if (!input || input[i] == '\0')
        return (1);
    while (input[i] != '\0')
    {
        j = 0;
        while (input[i] != '\0' && input[i] != ';' && !(input[i] == '&' && input[i + 1] == '&'))
		{
				if (input[i] == '(')
				{
					while (input[i] != ')')
					{
						copy[j++] = input[i++];
					}
				}
				else
            		copy[j++] = input[i++];
		}
        copy[j] = '\0';
		//printf("\tcopy = %s\n", copy);
		if (input[i] == '&' && input[i + 1] == '&')
			ee->command_with_and = 1;
		//printf("--copy = %s\n", copy);
        	if (success)
        	{
        	    changed_args = check_dollars(copy, ee);
        	    if (!changed_args)
        	        return 1;
        	    reconstructed_input = reconstruct_input(changed_args);
				if (find_parenthesis(copy) == 1)
				{
					//ft_printf("-  1  -\n");
					its_just_a_parenthese(copy, ee);
					//printf("ee->signal = %d\n", ee->signal);
				}
				else if (find_pipe(reconstructed_input) == 1 && find_or(reconstructed_input) == 0 && find_redirection(reconstructed_input) == 0)
				{
					//printf("-  2  -\n");
					execute_pipeline(reconstructed_input, ee);
					//printf("%d\n", ee->check_and_validity);
				}
				else if (find_pipe(reconstructed_input) == 1 && find_redirection(reconstructed_input) == 1)
				{
					//printf("-  3  -\n");
					execute_pipeline_heredoc(reconstructed_input, ee);
				}
				else
				{
        	    	success = interprete_commande(reconstructed_input, ee) == 0;
				}
				if (changed_args)
        	    	free_split(changed_args);
				if (reconstructed_input)
        	    	free(reconstructed_input);
        	}
        	if (ee->check_and_validity == 1)
        	{
				//printf("qwer\n");
				success = false;
				ee->check_and_validity = 0;
        	}
			ee->command_with_and = 0;
        	if (input[i] == ';')
        	{
        	    success = true;
        	    i++;
        	}
			if (input[i] == '&' && input[i + 1] == '&')
				i += 2;
   		}
    return 0;
}















void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = 130;
	}
}

void	catch_signal(t_ee *ee)
{
	(void)ee;
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

char	**parse_dollars(char **args, t_ee *ee)
{
	char	*copy;
	char	**changed_args;
	char	*after_equal;
	char	*before_equal;
	int		lock;
	int i, j, k, x, y, m;
	
	changed_args = malloc(sizeof(char *) * (ft_strlonglen(args) + 1));
	if (!changed_args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		lock = 0;
		if (args[i][0] == '$')
		{
			copy = malloc(sizeof(char) * ft_strlen(args[i]));
			if (!copy)
				return (NULL);
			j = 1;
			k = 0;
			while (args[i][j])
				copy[k++] = args[i][j++];
			copy[k] = '\0';
			x = 0;
			while (ee->envp[x])
			{
				y = 0;
				if (ft_strncmp(ee->envp[x], copy, ft_strlen(copy)) == 0
					&& ee->envp[x][ft_strlen(copy)] == '=')
				{
					y = ft_strlen(copy) + 1;
					after_equal = ft_strdup(ee->envp[x] + y);
					if (!after_equal)
						return (NULL);
					changed_args[i] = after_equal;
					lock = 1;
					break ;
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
			if (args[i][j] == '$')
			{
				m = j;
				before_equal = malloc(sizeof(char) * (m + 1));
				if (!before_equal)
					return (NULL);
				k = 0;
				while (k < m)
				{
					before_equal[k] = args[i][k];
					k++;
				}
				before_equal[m] = '\0';
				changed_args[i] = before_equal;
				lock = 1;
			}
			if (!lock)
				changed_args[i] = ft_strdup(args[i]);
		}
		i++;
	}
	changed_args[i] = NULL;
	return (changed_args);
}

char	**check_dollars(char *input, t_ee *ee)
{
	char	**args;
	char	**changed_args;

	args = ft_split(input, ' ');
	if (!args)
		return (NULL);
	changed_args = parse_dollars(args, ee);
	if (!changed_args)
		return (NULL);
	free_split(args);
	return (changed_args);
}

char	*reconstruct_input(char **changed_args)
{
	int		i;
	size_t	total_len;
	char	*new_input;
	char	*current_pos;
	size_t	len;

	i = 0;
	total_len = 0;
	while (changed_args[i])
	{
		total_len += ft_strlen(changed_args[i]) + 1;
		i++;
	}
	new_input = malloc(sizeof(char) * total_len);
	if (!new_input)
		return (NULL);
	current_pos = new_input;
	i = 0;
	while (changed_args[i])
	{
		len = ft_strlen(changed_args[i]);
		ft_memcpy(current_pos, changed_args[i], len);
		current_pos += len;
		if (changed_args[i + 1])
			*current_pos++ = ' ';
		i++;
	}
	*current_pos = '\0';
	return (new_input);
}
int calcul_check_path(char **check_path)
{
	int i = 0;

	while (check_path[i])
	{
		if (ft_strcmp(check_path[i], "|") == 0)
			break;
		i++;
	}
	i++;
	return (i);
}	

int execute_pipeline(char *input, t_ee *ee) 
{
    char **commands;
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;
    int i = 0;

    commands = ft_split(input, '|');
    while (commands[i])
	{
        if (pipe(pipe_fd) == -1)
		{
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        pid = fork();
        if (pid == -1)
		{
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
		{
            if (prev_fd != -1) 
			{
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (commands[i + 1])
			{
                dup2(pipe_fd[1], STDOUT_FILENO);
            }
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            interprete_commande(commands[i], ee);
            exit(EXIT_FAILURE);
        }
		else
		{
            close(pipe_fd[1]);
            if (prev_fd != -1)
			{
                close(prev_fd);
            }
            prev_fd = pipe_fd[0];
        }
        i++;
    }
    if (prev_fd != -1) 
        close(prev_fd);
    while (wait(NULL) > 0);
    free_split(commands);
	char **check_path = ft_split(input, ' ');
	//printf("%s\n", check_path[0]);
	char *path = find_command_path(check_path[0]);
	if (!path)
	{
		ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", check_path[0]);
		//ee->signal = 127;

	}
	free_split(check_path);
	free(path);
	check_path = ft_split(input, ' ');
	path = find_command_path(check_path[calcul_check_path(check_path)]);
	if (!path)
	{
		ee->signal = 127;
		ee->confirmed_command = 0;
	}
	else
		ee->confirmed_command = 1;
	free_split(check_path);
	free(path);
	return (0);
}


int execute_pipeline_heredoc(char *input, t_ee *ee)
{
    char **commands;
	char **check_path;
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;
    int i = 0;

    commands = ft_split(input, '|');
    while (commands[i])
    {
        int heredoc_fd, input_fd, output_fd;
        char *final_command = handle_redirection_with_pipe(commands[i], ee, &heredoc_fd, &input_fd, &output_fd);
        if (!final_command)
        {
            perror("🔒 Erreur lors du traitement des redirections");
            free_split(commands);
            return -1;
        }
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (heredoc_fd != -1)
            {
                dup2(heredoc_fd, STDIN_FILENO);
                close(heredoc_fd);
            }
            else if (input_fd != -1)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (commands[i + 1])
                dup2(pipe_fd[1], STDOUT_FILENO);
            if (output_fd != -1)
            {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            interprete_commande(final_command, ee);
            exit(EXIT_FAILURE);
        }
        else
        {
            close(pipe_fd[1]);
            if (prev_fd != -1)
                close(prev_fd);
            prev_fd = pipe_fd[0];
            if (heredoc_fd != -1)
                close(heredoc_fd);
            if (input_fd != -1)
                close(input_fd);
            if (output_fd != -1)
                close(output_fd);
        }
        free(final_command);
        i++;
    }
    if (prev_fd != -1)
        close(prev_fd);
    while (wait(NULL) > 0);
    free_split(commands);
	check_path = ft_split(input, ' ');
	char *path = find_command_path(check_path[0]);
	if (!path)
	{
		ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", check_path[0]);
	}
	free_split(check_path);
	free(path);
	check_path = ft_split(input, ' ');
	path = find_command_path(check_path[calcul_check_path(check_path)]);
	if (!path)
	{
		ee->signal = 127;
		ee->confirmed_command = 0;
	}
    return 0;
}


char *find_env_var(const char *var_name, t_ee *ee)
{
    int i = 0;
    int name_len = ft_strlen(var_name);

    while (ee->envp && ee->envp[i])
	{
        if (ft_strncmp(ee->envp[i], var_name, name_len) == 0 && ee->envp[i][name_len] == '=')
            return ft_strdup(&ee->envp[i][name_len + 1]);
        i++;
    }
    return NULL;
}


char *expand_variable(char *input, t_ee *ee)
{
    char var_name[256];
    int i = 1;
    int j = 0;
    char *value;

    while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
	{
        if (j < 255)
            var_name[j++] = input[i++];
    }
    var_name[j] = '\0';

    value = find_env_var(var_name, ee);
    if (value)
        return value;
    return ft_strdup("");
}


char *handle_quotes(char *input, t_ee *ee)
{
    char *result = malloc(1024);
    int size = 1024;
    int i = 0, j = 0;
    char quote = '\0';
    char *expanded;
	int expanded_len;

    if (!result)
        return NULL;
    while (input[i])
	{
        if ((input[i] == '\'' || input[i] == '"') && quote == '\0')
            quote = input[i++];
		else if (input[i] == quote)
		{
            quote = '\0';
            i++;
        } 
		else
		{
            if (quote == '"' && input[i] == '$')
			{
                expanded = expand_variable(&input[i], ee);
                if (expanded)
				{
                    expanded_len = ft_strlen(expanded);
                    if (j + expanded_len >= size)
					{
                        size *= 2;
                        result = realloc(result, size);
                        if (!result) {
                            free(expanded);
                            return NULL;
                        }
                    }
                    strcpy(&result[j], expanded);
                    j += expanded_len;
                    free(expanded);
                    i++;
                    while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                        i++;
                    continue;
                }
            }
            if (j + 1 >= size)
			{
                size *= 2;
                result = realloc(result, size);
                if (!result)
                    return NULL;
            }
            result[j++] = input[i++];
        }
    }
    result[j] = '\0';
    return result;
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

char	*save_initial_path(t_ee *ee)
{
	int i = 0;
	char *tmp;

	tmp = NULL;
	while (ee->envp[i])
	{
		if (ee->envp[i][0] == 'P' && ee->envp[i][1] == 'A' && ee->envp[i][2] == 'T' && ee->envp[i][3] == 'H' && ee->envp[i][4] == '=')
		{
			tmp = ft_strdup(ee->envp[i]);
			break ;
		}
		i++;
	}
	return (tmp);
}




int	find_redirection(char *input)
{
	int i = 0;

	while(input && input[i])
	{
		if (input[i] == '>' || input[i] == '<')
			return (1);
		i++;
	}
	return (0);
}

int find_type_of_redirection(char *tmp_in)
{
    int i = 0;

    while (tmp_in && tmp_in[i])
    {
        if (tmp_in[i] == '>' && tmp_in[i + 1] != '>')
            return (3);
        else if (tmp_in[i] == '>' && tmp_in[i + 1] == '>')
            return (2);
        
        else if (tmp_in[i] == '<' && tmp_in[i + 1] != '<')
            return (1);  // Redirection d'entrée simple "<"
        else if (tmp_in[i] == '<' && tmp_in[i + 1] == '<')
            return (4); 
        
        i++;
    }
    return (0);  // Pas de redirection trouvée
}


char	*parse_exev_input(char *tmp_in)
{
	int i = 0;
	int j = 0;
	char *return_input;

	while (tmp_in && tmp_in[i])
	{
		if (tmp_in[i] == '>' || tmp_in[i] == '<')
			break;
		i++;
	}
	return_input = malloc(sizeof(char) * i + 1);
	while (j < i)
	{
		return_input[j] = tmp_in[j];
		j++;
	}
	return_input[j] = '\0';
	return (return_input);
}

int	for_same_comportement(t_redir *re, char **split_in)
{
	(void)re;
	char *path = find_command_path(split_in[0]);
	if (!path)
		return (1);
	if (path)
		free(path);
	return (0);
}

char *unstick_to_re_stick(char *input)
{
    int i = 0, j = 0;
    int count_how_many_sep = 0;
    char *good_sep;

    while (input && input[i])
    {
        if ((input[i] > 32 && (input[i + 1] == '<' || input[i + 1] == '>')) ||
            ((input[i] == '<' || input[i] == '>') && input[i + 1] > 32))
        {
            count_how_many_sep++;
        }
        i++;
    }
    good_sep = malloc(sizeof(char) * (i + count_how_many_sep + 1));
    if (!good_sep)
        return NULL;
i = 0;
    while (input[i])
    {
		if (input[i - 1] > 32 && (input[i] == '<' || input[i] == '>'))
			good_sep[j++] = ' ';
        if (input[i] == '<' && input[i + 1] == '<')
        {
            good_sep[j++] = input[i];  
            good_sep[j++] = input[i+1]; 
            good_sep[j++] = ' ';      
            i++;
        }
        else if (input[i] == '>' && input[i + 1] == '>')
        {
            good_sep[j++] = input[i];  
            good_sep[j++] = input[i+1];
            good_sep[j++] = ' ';    
            i++;
        }
        else if (input[i] == '<' || input[i] == '>')
        {
            good_sep[j++] = input[i];
            good_sep[j++] = ' ';       
        }
        else
            good_sep[j++] = input[i];
        i++;
    }
    good_sep[j] = '\0';
    return good_sep;
}




























int open_available(char **tmpfilename)
{
    int fd;
    char *tmp_dir = "/tmp/tmp_for_heredoc_";
    char *idxstr;
    char *final_filename;
    int i;

    i = 0;
    while (i < 65535)
    {
        idxstr = ft_itoa(i);
        final_filename = (char *)malloc(strlen(tmp_dir) + strlen(idxstr) + 1); 
        if (!final_filename)
        {
            free(idxstr);
            return (-1);
        }
        strcpy(final_filename, tmp_dir);
        ft_strcat(final_filename, idxstr);
        free(idxstr);
        fd = open(final_filename, O_WRONLY | O_CREAT | O_EXCL, 0644);
        if (fd >= 0)
        {
            *tmpfilename = final_filename;
            return fd;
        }
        free(final_filename);
        i++;
    }
    return (-1);
}

void handle_herdoc_sigint(int status)
{
	rl_replace_line("", 0);
    ft_putendl_fd("\n", 1);
    rl_on_new_line();
	unlink("/tmp/heredoc_tmp");
    exit(status);
}

int write_to_tmpfile(int fd, char *limit, t_ee *ee)
{
    char *input = NULL;
	(void)ee;

    signal(SIGINT, handle_herdoc_sigint);
    while (1)
    {
        input = readline("🗒️🖊️  ");
        if (!input || ft_strcmp(input, limit) == 0)
        {
            free(input);
            break;
        }
        ft_putendl_fd(input, fd);
        free(input);
    }
    return 0;
}


char *foud_delimiter(char **split_in);






char *handle_redirection_with_pipe(char *input, t_ee *ee, int *heredoc_fd, int *input_fd, int *output_fd)
{
    char **split_in = NULL;
    char *input_execv = NULL;
    int last_name;
    char *path;
    char *final_command;
    t_redir *re = NULL;
    // Here-doc
    char *heredoc_tmpfile = NULL;
    int fd;
    char *delimiter = NULL;

    *heredoc_fd = -1;
    *input_fd = -1;
    *output_fd = -1;

    re = malloc(sizeof(t_redir));
    if (!re)
        return NULL;
    re->command_fail = 0;
    input_execv = parse_exev_input(input);
    split_in = ft_split(unstick_to_re_stick(input), ' ');
    if (!split_in || !split_in[0])
    {
        free_split(split_in);
        free(re);
        free(input_execv);
        return NULL;
    }
    last_name = ft_strlonglen(split_in);
    path = find_command_path(split_in[0]);
    for (int i = 1; i < last_name; i++)
    {
        if (strcmp(split_in[i], "<<") == 0)
        {
            fd = open_available(&heredoc_tmpfile);
            if (fd < 0)
            {
                perror("🔒 Erreur création fichier here-doc");
                return NULL;
            }

            delimiter = split_in[i + 1];
            int ret = write_to_tmpfile(fd, delimiter, ee);
            close(fd);
            if (ret != 0)
            {
                perror("🔒 Erreur écriture here-doc");
                unlink(heredoc_tmpfile);
                return NULL;
            }

            *heredoc_fd = open(heredoc_tmpfile, O_RDONLY);
            if (*heredoc_fd < 0)
            {
                perror("🔒 Erreur ouverture fichier here-doc");
                unlink(heredoc_tmpfile);
                return NULL;
            }
            i++;
        }
        else if (strcmp(split_in[i], "<") == 0)
        {
            *input_fd = open(split_in[i + 1], O_RDONLY);
            if (*input_fd < 0)
            {
                perror("🔒 Erreur ouverture fichier '<'");
                return NULL;
            }
            i++;
        }
        else if (strcmp(split_in[i], ">") == 0)
        {
            *output_fd = open(split_in[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (*output_fd < 0)
            {
                perror("🔒 Erreur ouverture fichier '>'");
                return NULL;
            }
            i++;
        }
        else if (strcmp(split_in[i], ">>") == 0)
        {
            *output_fd = open(split_in[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
            if (*output_fd < 0)
            {
                perror("🔒 Erreur ouverture fichier '>>'");
                return NULL;
            }
            i++;
        }
    }
    final_command = strdup(input_execv);
    if (!final_command)
    {
        perror("🔒 Erreur allocation mémoire");
        return NULL;
    }
    free_split(split_in);
    free(input_execv);
    free(path);
    free(re);
    return final_command;
}










void handle_redirection(char *input, t_ee *ee)
{
    char *tmp_in;
    char *if_need_sep;
    char **split_in = NULL;
    int last_name;
    int file = -1;
    pid_t pid;
    char *path;
    char *input_execv = NULL;
    char **split_execv = NULL;
    t_redir *re = NULL;

    // here-doc
    char *heredoc_tmpfile = NULL;  
    int heredoc_fd = -1;
    char *tmpfilenames[10];
    int heredoc_count = 0;
    int fd;
    int status;
    char *delimiter = NULL;

    re = malloc(sizeof(t_redir));
    if (!re)
        return;
    re->command_fail = 0;
    tmp_in = input;
    input_execv = parse_exev_input(tmp_in);
    split_execv = ft_split(input_execv, ' ');
    if (!split_execv || !split_execv[0]) 
    {
        free_split(split_execv);
        free(re);
        free(input_execv);
        return;
    }
    if_need_sep = unstick_to_re_stick(tmp_in);
    split_in = ft_split(if_need_sep, ' ');
    if (!split_in || !split_in[0]) 
    {
        free_split(split_in);
        free_split(split_execv);
        free(re);
        free(input_execv);
        return;
    }
    last_name = ft_strlonglen(split_in);
    path = find_command_path(split_in[0]);
    pid = fork();
    if (pid == -1)
    {
        free_split(split_in);
        free_split(split_execv);
        free(re);
        free(input_execv);
        return;
    }
    signal(SIGINT, SIG_IGN);
    if (pid == 0)
    {
        heredoc_fd = open_available(&heredoc_tmpfile);
        if (heredoc_fd < 0)
        {
            perror("🔒 Erreur création fichier temporaire here-doc final");
            exit(EXIT_FAILURE);
        }
        for (int i = 1; i < last_name; i++)
        {
            if (strcmp(split_in[i], "<<") == 0)
            {
                fd = open_available(&tmpfilenames[heredoc_count]);
                if (fd < 0)
                {
                    perror("🔒 Erreur création fichier here-doc");
                    exit(EXIT_FAILURE);
                }
                delimiter = split_in[i + 1];
                int ret = write_to_tmpfile(fd, delimiter, ee);
                close(fd);
                if (ret != 0) 
                {
                    perror("🔒 Erreur écriture here-doc");
                    unlink(tmpfilenames[heredoc_count]);
                    exit(EXIT_FAILURE);
                }
                fd = open(tmpfilenames[heredoc_count], O_RDONLY);
                if (fd < 0) 
                {
                    perror("🔒 Erreur ouverture fichier here-doc");
                    unlink(tmpfilenames[heredoc_count]);
                    exit(EXIT_FAILURE);
                }
                char buffer[1024];
                ssize_t bytes;
                while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
                {
                    write(heredoc_fd, buffer, bytes);
                }
                close(fd);
                unlink(tmpfilenames[heredoc_count]);
                heredoc_count++;
                i++;
            }
        }
        close(heredoc_fd);
        heredoc_fd = open(heredoc_tmpfile, O_RDONLY);
        if (heredoc_fd < 0) 
        {
            perror("🔒 Erreur ouverture fichier here-doc final");
            exit(EXIT_FAILURE);
        }
        dup2(heredoc_fd, STDIN_FILENO);
        close(heredoc_fd);
        unlink(heredoc_tmpfile);
        for (int i = 1; i < last_name; i++)
        {
            if (strcmp(split_in[i], ">") == 0)
            {
                file = open(split_in[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
                if (file == -1)
                {
                    perror("🔒 Erreur ouverture fichier '>'");
                    exit(EXIT_FAILURE);
                }
                dup2(file, STDOUT_FILENO);
                close(file);
                i++;
            }
            else if (strcmp(split_in[i], ">>") == 0)
            {
                file = open(split_in[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
                if (file == -1)
                {
                    perror("🔒 Erreur ouverture fichier '>>'");
                    exit(EXIT_FAILURE);
                }
                dup2(file, STDOUT_FILENO);
                close(file);
                i++;
            }
        }
        execv(path, split_execv);
        exit(EXIT_SUCCESS);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) 
        {
            if (!g_status) 
            {
                ee->signal = WEXITSTATUS(status);
                if (ee->signal != 0 && ee->signal != 1)
                    ee->signal = 128 + ee->signal;
            }
        }
    }
	if (!path)
	{
		ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", split_in[0]);
		ee->signal = 127;
		ee->confirmed_command = 0;
	}
	else
		ee->confirmed_command = 1;
    signal(SIGINT, handle_sigint);  
    free_split(split_execv);
    free_split(split_in);
    free(input_execv);
    free(if_need_sep);
    free(path);
    free(re);
}


















char *foud_delimiter(char **split_in)
{
	char *tmp;
	int i = 0;
	while (split_in[i])
	{
		if (ft_strcmp(split_in[i], "<") == 0)
			break ;
		i++;
	}
	i++;
	i++;
	tmp = ft_strdup(split_in[i]);
	return (tmp);
}


int check_after_or(char *input)
{
	int i = 0;

	while (input[i])
		i++;
	while (i > 0)
	{
		if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
			return (0);
		if (input[i] == '|')
			return (1);
		i--;
	}
	return (0);
}


///////////////////////////////////////////////////////////////////////////

int	find_or(char *input)
{
	int i = 0;

	while (input && input[i])
	{
		if (input[i] == '|' && input[i + 1] == '|')
			return (1);
		i++;
	}
	return (0);
}

char *copy_before_or(char *src)
{
	char *tmp;
	int i = 0;
	int j = 0;

	while (src && src[i])
	{
		if (src[i] == '(')
		{
			while (src[i] != ')')
				i++;
		}
		if (src[i] == '|' && src[i + 1] == '|')
			break;
		i++;
	}
	tmp = malloc(sizeof(char) * i + 1);
	while (j < i)
	{
		tmp[j] = src[j];
		j++;
	}
	tmp[j] = '\0';
	return (tmp);
}

char *copy_after_or(char *src)
{
	int i = 0;
	int k = 0;
	int j = 0;
	char *tmp;

	//printf("src = %s\n", src);
	while (src && src[i])
	{
		if (src[i] == '(')
		{
			while (src[i] != ')')
				i++;
		}
		if (src[i] == '|' && src[i + 1] == '|')
			break;
		i++;
	}
	i += 2;
	if (src[i] == '\0')
		return (NULL);
	//printf("src[i] = %c\n", src[i]);
	k = ft_strlen(src);
	tmp = malloc(sizeof(char) * (k - i + 1));
	if (!tmp)
	{
		free (tmp);
		return (NULL);
	}
	while (i < k)
	{
		tmp[j] = src[i];
		i++;
		j++;
	}
	tmp[j] = '\0';
	return (tmp);
}

void check_path_in_or_with_pipe(char *input, t_ee *ee)
{
	char **split_input;
	char *path;

	//ft_printf("% -- s\n", input);
	split_input = ft_split(input, ' ');
	//ft_printf(" // %s\n", split_input[0]);
	path = find_command_path(split_input[0]);
	if (path)
		ee->confirmed_command = 1;
	free_split(split_input);
	free(path);
	return ;
}

///////////////////////////////////////////////////////////////////////////
int	interprete_commande(char *input, t_ee *ee)
{
	char	*trimmed_input;
	t_token	*token;
	// pour ||
	char *command_before_or;
	char *command_after_or = NULL;
	//char *path;


	//ft_printf("\tinterpret_command_input > %s\n", input);
	if (find_or(input) == 1)
	{
		if (check_after_or(input) == 0)
		{
			command_before_or = copy_before_or(input);
			command_after_or = copy_after_or(input);
			char *see_what_after = copy_after_or(command_after_or);
			char *path = find_command_path(command_after_or);
			if (see_what_after == NULL && !path)
			{
				ee->save_result = 1;
			}
			free(path);
			//ft_printf("%s\n", command_after_or);
			cumulate_token(command_before_or, ee);
			//printf("confirmed_command = %d\n", ee->confirmed_command);
        	if (ee->confirmed_command == 0)
			{
        	    cumulate_token(command_after_or, ee);
				if (ee->save_result == 1)
					ee->check_and_validity = 1;
			}
			if (command_before_or)
        		free(command_before_or);
			if (command_after_or)
				free(command_after_or);
			ee->command_with_or = 0;
			ee->confirmed_command = 0;
        	return (0);
		}
    }
	//printf("'%s'\n", command_after_or);
	if (find_redirection(input) == 1)
	{
		handle_redirection(input, ee);
		return (0);
	}
	else
	{
		token = malloc(sizeof(t_token));
		token->token = 0;
		if (!token)
			return (0);
		token->token = 0; 
		trimmed_input = skip_isspace_for_fonctions(input, token);
		if (!trimmed_input)
		{
			if (token)
				free(token);
			return (0);
		}
		if (token->token == 1)
		{
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token ';'\n");
			if (token)
				free(token);
			return (0);
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
		{
			ft_pwd();
			ee->confirmed_command = 1;
		}
		else if ((ft_strcmp(trimmed_input, "cd") == 0) || (ft_strcmp(trimmed_input, "~") == 0))
		{
			if (ee->copy_oldpwd)
				free(ee->copy_oldpwd);
			ft_cd(input, ee);
			ee->confirmed_command = 1;
		}
		else if (ft_strcmp(trimmed_input, "env") == 0)
		{
			if (ee->path_is_not_able == 0 && ee->lock_path == 0)
			{
				ft_env(ee);
				ee->confirmed_command = 1;
			}
			else
			{
				ft_printf("🌳(´•︵•`)🌳: env: No such file or directory\n");
				ee->confirmed_command = 0;
			}
		}
		else if (ft_strcmp(trimmed_input, "unset") == 0)
		{
			ft_unset(input, ee);
			ee->confirmed_command = 1;
		}
		else if ((ft_strcmp(trimmed_input, "export") == 0) || (ft_strcmp(trimmed_input, "export=") == 0))
		{
			ft_export(input, ee);
			ee->confirmed_command = 1;
		}
		else if (ft_strcmp(trimmed_input, "echo") == 0)
		{
			ft_echo(input, ee);
			ee->confirmed_command = 1;
		}
		else if (ft_strcmp(trimmed_input, "$?") == 0)
		{
			ft_printf("🍁_(`へ´*)_🍁: 0: command not found\n");
			ee->signal = 127;
		}
		else if (find_pipe(input) == 1)
			cumulate_token(input, ee);
		else
		{
			if (ee->minishell_check == 0)
				execute_external_command(input, ee);
			if (token)
				free(token);
			if (trimmed_input)
				free(trimmed_input);
			return (0);
		}
		if (token)
			free(token);
		if (trimmed_input)
			free(trimmed_input);
	}
	ee->signal = 0;
	return (0);
}
/////////////////////////////////////////////////////////////////




int	find_pipe(char *input)
{
    int i = 0;

    while (input && input[i])
    {
        if (input[i] == '|' && (input[i + 1] != '|'))
            return (1);
        if (input[i] == '|' && input[i + 1] == '|')
            return (0);
        i++;
    }
    return (0);
}

/////////////////////////////////////////////////////////////////

void loop(char *input, t_ee *ee)
{
    t_token *tok;
    char **changed_args;
	char *cleaned_input;

	//// Pour les quotes 
    int single_quotes = 0;
    int double_quotes = 0;
    int i;
	char *temp;
	char *next_line;
	////

    changed_args = NULL;
    if ((!ee->envp || !ee->envp[0]) && ee->lock_path == 0)
        you_shall_not_path();
    tok = malloc(sizeof(t_token));
    tok->found = 0;
    input = readline("🍀_(^o^)_🍀  > ");
    if (input == NULL)
	{
        ee->minishell_check = 1;
		free(input);
		free(tok);
		return ;
	}
	printf("%s\n", input);
	i = 0;
	////////////////////////////////////////
	while (input[i] && input[i] <= 32)
		i++;
	if (input[i] && input[i] == '&')
	{
		if (input[i + 1] == '&')
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `&&'\n");
		else
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `&'\n");
		ee->signal = 2;
		free(input);
    	free(tok);
    	return;
	}
	if (input[i] && input[i] == '|')
	{
		if (input[i + 1] == '|')
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `||'\n");
		else
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `|'\n");
		ee->signal = 2;
		free(input);
    	free(tok);
    	return;
	}
	if (input[i] && input[i] == ';')
	{
		if (input[i + 1] == ';')
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;;'\n");
		else
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;'\n");
		ee->signal = 2;
		free(input);
    	free(tok);
    	return;
	}
	i = 0;
	while (input[i])
	{
		if (input[i] == '&' && input[i + 1] == '&' && input[i + 2] == '&')
		{
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `&'\n");
			ee->signal = 2;
			free(input);
    		free(tok);
    		return;
		}
		if (input[i] == '|' && input[i + 1] == '|' && input[i + 2] == '|')
		{
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `|'\n");
			ee->signal = 2;
			free(input);
    		free(tok);
    		return;
		}
		i++;
	}
	///////////////////////////////////////
	if (g_status == 130)
    {
        // Affiche le code d'erreur immédiatement après Ctrl+C
        ee->signal = 130;
        g_status = 0; // Réinitialiser g_status après l'affichage du message
    }
	while (input && *input)
    {
        single_quotes = 0;
        double_quotes = 0;
		i = 0;
        while (input[i])
        {
            if (input[i] == '\'')
                single_quotes++;
            else if (input[i] == '"')
                double_quotes++;
			i++;
        }
        if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
        {
            next_line = readline("> ");
            if (next_line == NULL)
            {
                free(input);
                ee->minishell_check = 1;
                return;
            }
            temp = malloc(strlen(input) + strlen(next_line) + 1);
            if (!temp)
            {
                free(input);
                free(next_line);
                return;
            }
            strcpy(temp, input);
            strcat(temp, next_line);
			strcat(temp, "\n");
            free(input);
            free(next_line);
            input = temp;
        }
        else
		{
			break ;
		}
    }
    if (input && *input)
	{
        add_history(input);
        cleaned_input = handle_quotes(input, ee);
        free(input);
        input = cleaned_input;
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
					{
                        printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;;'\n");
					}
					ee->signal = 2;
                    free(input);
                    free(tok);
                    return;
                }
            }
			printf("%s\n", input);
            if (find_pipe(input) == 0 && cumulate_token(input, ee) == 1)
            	execute_pipeline(input, ee);
			else if (find_pipe(input) == 1 && cumulate_token(input, ee) == 1)
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
	ee->save_initial_path = save_initial_path(ee);
	catch_signal(ee);
	while (ee->minishell_check == 0)
		loop(input, ee);
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
	if (ee->copy_export_env)
		free_split(ee->copy_export_env);
	if (ee->save_initial_path)
		free(ee->save_initial_path);
	free(ee);
	clear_history();
	return (0);
}