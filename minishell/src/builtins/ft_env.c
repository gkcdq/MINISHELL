#include "../../minishell.h"

/*void	ft_env(char **envp)
{
	t_env		*env;
	int			i;
	static int	found_index = 0;
	static char	*dif_oldpwd = NULL;
	static int	save_value_for_print = 0;

	env = malloc(sizeof(t_env));
	if (!env)
		return ;
	env->copy_pwd = NULL;
	env->copy_oldpwd = NULL;
	env->print_the_same = 0;
	if (dif_oldpwd != NULL)
		env->copy_oldpwd = ft_strdup(dif_oldpwd);
	env->found_difference = found_index;
	copy_pwd(env);
	if (save_value_for_print == 0)
		save_value_for_print = env->print_the_same;
	env->print_the_same = save_value_for_print;
	found_index = env->found_difference;
	if (env->copy_oldpwd)
	{
		//if (dif_oldpwd)
		//	free(dif_oldpwd);
		if (env->print_the_same == 0)
			dif_oldpwd = ft_strdup(env->copy_oldpwd);
	}
	if (!envp || !envp[0])
	{
		printf("PWD=%s\n", env->copy_pwd);
		printf("SHLVL=1\n");
		printf("_=/usr/bin/env\n");
		// printf("%d\n", env->print_the_same);
		if ((env->found_difference == 1)|| env->print_the_same == 1)
		{
			if (env->print_the_same == 1)
			{
				printf("123\n");
				printf("OLDPWD=%s\n", env->copy_oldpwd);
				env->print_the_same = 0;
			}
			else
			{
				printf("987\n");
				printf("OLDPWD=%s\n", env->copy_oldpwd);
				//printf("\nBpwd=%s\n", env->copy_pwd);
				//printf("\nBOLDpwd=%s\n", env->copy_oldpwd);
				//env->copy_oldpwd = ft_strdup(env->copy_pwd);
				//dif_oldpwd = ft_strdup(env->copy_pwd);
				//printf("\nApwd=%s\n", env->copy_pwd);
				//printf("\nAOLDpwd=%s\n", env->copy_oldpwd);
				env->found_difference = 0;
			}
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
	static char *save = NULL;
	static char *last_save;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		env->copy_pwd = NULL;
		return ;
	}
	if (env->copy_pwd)
		free(env->copy_pwd);
	env->copy_pwd = ft_strdup(pwd);
	if ((save == NULL))
		save = ft_strdup(env->copy_pwd);
	if (env->copy_pwd && last_save == NULL)
		last_save = ft_strdup(env->copy_pwd);
	if (env->copy_pwd && ft_strcmp(save, env->copy_pwd) == 0)
	{
		env->found_difference = 0;
		env->print_the_same = 1;
		env->copy_oldpwd = ft_strdup(last_save);
		free(pwd);
		return ;
	}
	else if (env->copy_pwd && ft_strcmp(save, env->copy_pwd) == 1)
	{
		env->found_difference = 1;
		env->print_the_same = 0;
		last_save = ft_strdup(env->copy_pwd);
		env->copy_oldpwd = ft_strdup(env->copy_pwd);
	}
	//if (env->copy_oldpwd)
		//printf("%s\n", env->copy_oldpwd);
	//if (env->copy_pwd != NULL)
	//	env->copy_oldpwd = ft_strdup(pwd);
	free(pwd);
}
*/


//////////////////////////////////////////////////////////////

void	ft_env(char **envp)
{
	t_env		*env;
	int 		i;
	static char	*save_oldpwd = NULL;
	static char	*last_pwd = NULL;

	env = malloc(sizeof(t_env));
	if (!env)
		return ;
	env->copy_pwd = getcwd(NULL, 0);
	env->copy_oldpwd = NULL;
	if (save_oldpwd == NULL)
		save_oldpwd = ft_strdup(env->copy_pwd);
	if (last_pwd != NULL && ft_strcmp(last_pwd, env->copy_pwd) != 0)
	{
		free(save_oldpwd);
		save_oldpwd = ft_strdup(last_pwd);
	}
	if (last_pwd)
		free(last_pwd);
	last_pwd = ft_strdup(env->copy_pwd);
	if (!envp || !envp[0])
	{
		printf("PWD=%s\n", env->copy_pwd);
		printf("SHLVL=1\n");
		printf("_=/usr/bin/env\n");
		if (save_oldpwd && ft_strcmp(save_oldpwd, env->copy_pwd) != 0)
			printf("OLDPWD=%s\n", save_oldpwd);
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
	free(env);
}
/////////////////////////////////////////////////////

