/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmilin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:27:56 by tmilin            #+#    #+#             */
/*   Updated: 2025/02/27 19:27:58 by tmilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	if_path_is_incorrect(char **args, t_ee *ee)
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

void	if_pid_is_equal_to_zero(char *path, char **args, t_ee *ee,
		char *command)
{
	int		i;

	i = 0;
	if (command[i] == '.' && command [i + 1] == '/')
	{
		if (command[i + 2] <= 32)
		{
			ft_printf("🚧_(⊙_⊙;)_🚧 : ./: Is a directory\n");
			exit(126);
		}
		if (access(command, X_OK) == -1)
		{
			ft_printf("%s: Permission denied\n", command);
			exit(126);
		}
	}
	if (ee->reset_sigint == 1)
		signal(SIGINT, handle_sigint);
	ee->reset_sigint = 0;
	if (execve(path, args, ee->envp) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void	if_pid_is_sup_to_zero(t_ee *ee, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (!g_status)
		{
			ee->signal = WEXITSTATUS(status);
		}
		else
			ee->signal = 0;
	}
	if (ee->reset_sigint == 1)
		signal(SIGINT, handle_sigint);
	ee->reset_sigint = 0;
}

void	check_signal(char *command, t_ee *ee)
{
	if (ft_strcmp(command, "./minishell") == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		ee->reset_sigint = 1;
	}
}

void	execute_external_command(char *command, t_ee *ee)
{
	char	**args;
	char	*path;
	pid_t	pid;

	check_signal(command, ee);
	args = ft_split(command, ' ');
	path = find_command_path(args[0]);
	if (!path)
	{
		if_path_is_incorrect(args, ee);
		return ;
	}
	else
		ee->confirmed_command = 1;
	pid = fork();
	if (pid == 0)
		if_pid_is_equal_to_zero(path, args, ee, command);
	else if (pid > 0)
		if_pid_is_sup_to_zero(ee, pid);
	else
		perror("fork");
	free(path);
	free_split(args);
}
