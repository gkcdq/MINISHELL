#include "../../minishell.h"

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

void	at_the_end(t_token *token, t_ee *ee)
{
	if (token->trimmed_input)
		free(token->trimmed_input);
	if (token)
		free(token);
	ee->signal = 0;
}

void	all_is__freed(t_ee *ee)
{
	int	i;

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
}
