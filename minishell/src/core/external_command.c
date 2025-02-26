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

void	if_pid_is_equal_to_zero(char *path, char **args, t_ee *ee)
{
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
			//if (ee->signal != 0 && ee->signal != 1 && ee->signal != 2)
			//	ee->signal = 128 + ee->signal;
		}
		else
			ee->signal = 0;
	}
}

void	execute_external_command(char *command, t_ee *ee)
{
	char	**args;
	char	*path;
	pid_t	pid;

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
		if_pid_is_equal_to_zero(path, args, ee);
	else if (pid > 0)
		if_pid_is_sup_to_zero(ee, pid);
	else
		perror("fork");
	free(path);
	free_split(args);
}

void	copy_until_parenthesis(char *l, int *i, char *copy, int *j)
{
	(*i)++;
	while (l[*i] != '\0' && l[*i] != ')')
	{
		copy[*j] = l[*i];
		(*j)++;
		(*i)++;
	}
	if (l[*i] == ')')
		(*i)++;
}
