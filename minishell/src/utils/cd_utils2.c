#include "../../minishell.h"

int	find_pwd_index(t_ee *ee)
{
	int	i;
	int	j;

	i = 0;
	while (ee->envp[i])
	{
		j = 0;
		while (ee->envp[i][j])
		{
			if ((ft_strcmpchar(ee->envp[i][j], 'P') == 0)
				&& (ft_strcmpchar(ee->envp[i][j + 1], 'W') == 0)
				&& (ft_strcmpchar(ee->envp[i][j + 2], 'D') == 0))
			{
				return (i);
			}
			j++;
		}
		i++;
	}
	return (-1);
}

void	update_pwd_variable(t_ee *ee, int index)
{
	char	*copy;

	copy = ft_strjoin_cd("PWD=", ee->copy_pwd);
	if (!copy)
		return ;
	if (ee->envp[index] && ft_strcmp(ee->envp[index], ee->copy_pwd) != 0)
	{
		free(ee->envp[index]);
		ee->envp[index] = ft_strdup(copy);
	}
	free(copy);
}

void	check_variable_pwd(t_ee *ee)
{
	int	index;

	index = find_pwd_index(ee);
	if (index != -1)
		update_pwd_variable(ee, index);
}
