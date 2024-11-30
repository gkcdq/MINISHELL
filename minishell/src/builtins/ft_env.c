#include "../../minishell.h"

void	ft_env(char **envp)
{
	t_env		*env;
	int			i;
	static int	found_index = 0;
	static char	*dif_oldpwd = NULL;
	static int	save_value_for_print = 0;
	static char	*i_m_done = NULL;

	env = malloc(sizeof(t_env));
	if (!env)
		return ;
	env->copy_pwd = NULL;
	env->copy_oldpwd = NULL;
	env->print_the_same = 0;
	if (dif_oldpwd != NULL)
		env->copy_oldpwd = ft_strdup(dif_oldpwd);
	env->found_difference = found_index;
	if (env->copy_pwd)
		i_m_done = ft_strdup(env->copy_pwd);
	copy_pwd(env);
	if (save_value_for_print == 0)
		save_value_for_print = env->print_the_same;
	env->print_the_same = save_value_for_print;
	found_index = env->found_difference;
	if (env->copy_oldpwd)
	{
		if (dif_oldpwd)
			free(dif_oldpwd);
		dif_oldpwd = ft_strdup(env->copy_oldpwd);
	}
	if (!envp || !envp[0])
	{
		printf("PWD=%s\n", env->copy_pwd);
		printf("SHLVL=1\n");
		printf("_=/usr/bin/env\n");
		// printf("%d\n", env->print_the_same);
		if ((env->found_difference == 1 && env->copy_oldpwd)
			|| env->print_the_same == 1)
		{
			//if (env->print_the_same == 1)
			//{
			//}
			//else
			//{
				printf("OLDPWD=%s\n", env->copy_oldpwd);
				// printf("\nBpwd=%s\n", env->copy_pwd);
				// printf("\nBOLDpwd=%s\n", env->copy_oldpwd);
				env->copy_oldpwd = ft_strdup(env->copy_pwd);
				dif_oldpwd = ft_strdup(env->copy_pwd);
				// printf("\nApwd=%s\n", env->copy_pwd);
				// printf("\nAOLDpwd=%s\n", env->copy_oldpwd);
				env->found_difference = 0;
			//}
		}
	}
	else
	{
		i = 0;
		while (envp[i])
		{
			printf("%s\n", envp[i]);
			i++;
		}
	}
	free(env->copy_pwd);
	free(env->copy_oldpwd);
	free(env);
}

void	copy_pwd(t_env *env)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		env->copy_pwd = NULL;
		return ;
	}
	if (env->copy_pwd)
		free(env->copy_pwd);
	env->copy_pwd = ft_strdup(pwd);
	if (env->copy_oldpwd && ft_strcmp(pwd, env->copy_oldpwd) != 0)
	{
		env->found_difference = 1;
		env->print_the_same = 1;
		return ;
	}
	else
		env->found_difference = 0;
	if (env->copy_pwd != NULL)
		env->copy_oldpwd = ft_strdup(pwd);
	free(pwd);
}
