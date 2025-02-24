#include "../../minishell.h"

void	norminette_pop_shove_it(t_ee *ee)
{
	unsetenv("PATH");
	ee->path_is_not_able = 1;
}

void	norminette_hardflip(t_unset_data *data, t_ee *ee)
{
	data->new_envp[data->k] = ft_strdup(ee->envp[data->i]);
	data->k++;
}

void	norminette_heelflip(t_ee *ee)
{
	unsetenv("PATH");
	ee->lock_path = 1;
}
