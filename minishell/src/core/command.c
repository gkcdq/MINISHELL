#include "../../minishell.h"

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
		if (input[i] == '&' && input[i + 1] == '&')
			ee->command_with_and = 1;
        	if (success)
        	{
        	    changed_args = check_dollars(copy, ee);
        	    if (!changed_args)
        	        return (1);
        	    reconstructed_input = reconstruct_input(changed_args);
				if (find_parenthesis(copy) == 1)
				{
					its_just_a_parenthese(copy, ee);
				}
				else if (find_pipe(reconstructed_input) == 1 && find_or(reconstructed_input) == 0 && find_redirection(reconstructed_input) == 0)
				{
					execute_pipeline(reconstructed_input, ee);
				}
				else if (find_pipe(reconstructed_input) == 1 && find_redirection(reconstructed_input) == 1)
				{
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
				success = false;
				ee->check_and_validity = 0;
        	}
			ee->command_with_and = 0;
        	if (input[i] == ';')
        	{
        	    success = true;
				if (check_after_token(input, i) == 0)
				{
					printf("%d\n", i);
					break ;
				}
				else
        	    	i++;
        	}
			if (input[i] == '&' && input[i + 1] == '&')
				i += 2;
   		}
    return 0;
}

int	interprete_commande(char *input, t_ee *ee)
{
	char	*trimmed_input;
	t_token	*token;
	// pour ||
	char *command_before_or;
	char *command_after_or = NULL;

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
			free(see_what_after);
			free(path);
			cumulate_token(command_before_or, ee);
        	if (ee->confirmed_command == 0)
			{
        	    cumulate_token(command_after_or, ee);
				if (ee->save_result == 1)
				{
					ee->check_and_validity = 1;
					ee->save_result = 0;
				}
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
		if (ft_strcmp(trimmed_input, "echo") == 0)
		{
			ft_echo(input, ee);
			ee->confirmed_command = 1;
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
