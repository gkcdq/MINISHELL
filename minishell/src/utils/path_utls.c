#include "../../minishell.h"

void    path_confirm(char *input)
{
    char    **check_path;
    char    *path;

    check_path = ft_split(input, ' ');
	path = find_command_path(check_path[0]);
	if (!path)
		ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", check_path[0]);
	free_split(check_path);
	free(path);
}

void    path_checker(char *input, t_ee *ee)
{
    char **check_path;
    char *path;

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
}

char	*find_command_path(char *command)
{
	char	*path_env;
	char	**dirs;
	char	*full_path;
	int		i;
	char	temp_path[2024];

	i = 0;
	if (command[i] == '.' || command[i] == '/')
		return(ft_strdup(command));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	full_path = NULL;
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

void	you_shall_not_path(void)
{
	char	*path_env;

	path_env = getenv("PATH");
	if (!path_env || *path_env == '\0')
		setenv("PATH", "/bin:/usr/bin", 1);
}

int	calcul_check_path(char **check_path)
{
	int	i;

	i = 0;
	while (check_path[i])
	{
		if (ft_strcmp(check_path[i], "|") == 0)
			break ;
		i++;
	}
	i++;
	return (i);
}
void	check_path_in_or_with_pipe(char *input, t_ee *ee)
{
	char	**split_input;
	char	*path;

	split_input = ft_split(input, ' ');
	path = find_command_path(split_input[0]);
	if (path)
		ee->confirmed_command = 1;
	free_split(split_input);
	free(path);
	return ;
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
