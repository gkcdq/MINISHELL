#include "../../minishell.h"

int	ft_strcmp_dif(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		else
			i++;
	}
	return (0);
}

char	*parse_input_simple_export(char *input)
{
	int i = 0;
	int j = 0;
	char *copy;

	while (input[i] && input[i] <= 32)
		i++;
	int start = i;
	while (input[i] && input[i] > 32)
		i++;
	copy = malloc(sizeof(char) * (i - start + 1));
	if (!copy)
		return (NULL);
	while (start < i)
	{
		copy[j] = input[start];
		start++;
		j++;
	}
	copy[j] = '\0';
	return (copy);
}

void	sort_export(t_ee *ee);



//void	export_in_env_or_not(t_ee *ee, char **args)
//{



//}



void	ft_export(char *input, t_ee *ee)
{
	char **args;

	args = ft_split(input, ' ');
	if (!ee->envp || !ee->envp[0])
	{
		if (ee->copy_oldpwd == NULL)
			printf("🏗️ 🏡 OLDPWD\n");
		else if (ee->copy_oldpwd && ft_strcmp(ee->copy_oldpwd,
				ee->copy_pwd) != 0 && (ee->if_unset__oldpwd == 0))
			printf("🏗️ 🏡 OLDPWD=%s\n", ee->copy_oldpwd);
		if (ee->copy_pwd == NULL)
			ee->copy_pwd = getcwd(NULL, 0);
		if (ee->if_unset__pwd == 0)
			printf("🏗️ 🏡 PWD=%s\n", ee->copy_pwd);
		if (ee->if_unset__shlvl == 0)
			printf("🏗️ 🏡 SHLVL=1\n");
		free_split(args);
		return ;
	}
	if (ft_strcmp(args[0], "export=") == 0)
	{
		free_split(args);
		return ;
	}
	if (args[1] == NULL)
	{
		input = parse_input_simple_export(input);
		if (ft_strcmp(input, "export") == 0)
			sort_export(ee);
		free(input);
	}
	//else
	//{
	//	export_in_env_or_not(ee, args);
	//}
	free_split(args);
}

char	*ft_strcat_export(char *s1, char *s2);

void	sort_export(t_ee *ee)
{
	char **sorted_env;
	int len = 0;
	int i, j;
	char *tmp;
	char *oldpwd;

	while (ee->envp[len])
		len++;
	sorted_env = malloc(sizeof(char *) * (len + 2));
	if (!sorted_env)
		return ;
	i = 0;
	while (i < len)
	{
		sorted_env[i] = ft_strdup(ee->envp[i]);
		i++;
	}
	if (ee->copy_oldpwd)
	{
		oldpwd = ft_strcat_export("OLDPWD=", ee->copy_oldpwd);
		if (oldpwd)
		{
			sorted_env[len] = oldpwd;
			len++;
		}
	}
	else
	{
		oldpwd = ft_strdup("OLDPWD");
		sorted_env[len] = oldpwd;
		len++;
	}
	sorted_env[len] = NULL;
	i = 0;
	while (i < len - 1)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strcmp_dif(sorted_env[i], sorted_env[j]) > 0)
			{
				tmp = sorted_env[i];
				sorted_env[i] = sorted_env[j];
				sorted_env[j] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < len)
	{
		if (sorted_env[i])
			ft_printf("🏗️ 🏡 %s\n", sorted_env[i]);
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

char	*ft_strcat_export(char *s1, char *s2)
{
	int len1 = ft_strlen(s1);
	int len2 = ft_strlen(s2);
	char *result = malloc(sizeof(char) * (len1 + len2 + 1));

	if (!result)
		return (NULL);
	int i = 0, j = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		result[i] = s2[j];
		i++;
		j++;
	}
	result[i] = '\0';
	return (result);
}
