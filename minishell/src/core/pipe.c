#include "../../minishell.h"

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

int execute_pipeline(char *input, t_ee *ee) 
{
    char **commands;
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;
    int i = 0;

	input = parse_input_pipeline(input);
	//printf("pipe input = %s\n", input);
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
	char *path = find_command_path(check_path[0]);
	if (!path)
		ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", check_path[0]);
	free_split(check_path);
	free(path);
	check_path = ft_split(input, ' ');
	path = find_command_path(check_path[calcul_check_path(check_path)]);
	if (!path)
	{
		ee->signal = 127;
		ee->check_and_validity = 1;
		ee->confirmed_command = 0;
	}
	else
	{
		ee->signal = 0;
		ee->check_and_validity = 0;
		ee->confirmed_command = 1;
	}
	free_split(check_path);
	free(path);
	free(input);
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

    input = parse_input_pipeline(input);
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
		ee->check_and_validity = 1;
		ee->confirmed_command = 0;
	}
	else
	{
		ee->signal = 0;
		ee->check_and_validity = 0;
		ee->confirmed_command = 1;
	}
	free(input);
    return 0;
}
