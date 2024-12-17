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
void	sort_export_plus(t_ee *ee);

int	ft_found_equal(char c)
{
	if (c == '=')
		return (1);
	return (0);
}

void	export_with_args(t_ee *ee, char **args)
{
	int i = 1;
	int len_envp = 0;
	int found;
	int len = 0;
	int j;
	char **copy_envp;
	///////////////////////////////
	char **copy_export;
	int len_export = 0;

	while (args[len])
		len++;
	len -= 1;
	while (ee->envp && ee->envp[len_envp])
		len_envp++;
	copy_envp = malloc(sizeof(char *) * (len_envp + len + 1));
	if (!copy_envp)
		return ;
	//
	/////////////////////////////////////////////////////////////////////////////////////////
	if (ee->copy_export_env != NULL)
	{
		while (ee->copy_export_env[len_export])
			len_export++;
		copy_export = malloc(sizeof(char *) * (len_export + len + 1));
		if (!copy_export)
			return ;
		len_export = 0;
		while (ee->copy_export_env[len_export])
		{
			copy_export[len_export] = ft_strdup(ee->copy_export_env[len_export]);
			len_export++;
		}
		copy_export[len_export] = NULL;
	}
	else
	{
		copy_export = malloc(sizeof(char *) * (len + 1));
		if (!copy_export)
			return ;
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	j = 0;
	while (j < len_envp)
	{
		copy_envp[j] = ft_strdup(ee->envp[j]);
		j++;
	}
	copy_envp[len_envp] = NULL;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			found = 0;
			j = 0;
			while (j < len_envp)
			{
				if (ft_strncmp(copy_envp[j], args[i], ft_strchr(args[i], '=')
						- args[i]) == 0)
				{
					free(copy_envp[j]);
					copy_envp[j] = ft_strdup(args[i]);
					found = 1;
					break ;
				}
				j++;
			}
			if (!found)
			{
				copy_envp[len_envp] = ft_strdup(args[i]);
				len_envp++;
				copy_envp[len_envp] = NULL;
			}
		}
		else
		{
			found = 0;
			j = 0;
			while (j < len_export)
			{
				if (ft_strcmp(copy_export[j], args[i]) == 0)
				{
					found = 1;
					break ;
				}
				j++;
			}
			if (!found)
			{
				copy_export[len_export] = ft_strdup(args[i]);
				len_export++;
				copy_export[len_export] = NULL;
			}
		}
		i++;
	}
	free_split(ee->envp);
	ee->envp = copy_envp;
	free_split(ee->copy_export_env);
	ee->copy_export_env = copy_export;
}

void	display_in_env_if_equal(t_ee *ee)
{
	int i, j, k;
	char **tmp;
	int len_one;
	int equal = 0;

	i = 0;
	while (ee->copy_export_env[i])
	{
		if (ft_strchr(ee->copy_export_env[i], '='))
			equal++;
		i++;
	}
	len_one = ft_strlonglen(ee->envp);
	tmp = malloc(sizeof(char *) * (len_one + equal + 1));
	i = 0;
	while (ee->envp[i])
	{
		tmp[i] = ft_strdup(ee->envp[i]);
		i++;
	}
	j = 0;
	while (ee->copy_export_env[j])
	{
		int key_length;
		char *equal_sign = ft_strchr(ee->copy_export_env[j], '=');
		if (equal_sign)
			key_length = equal_sign - ee->copy_export_env[j];
		else
			key_length = ft_strlen(ee->copy_export_env[j]);
		char *new_key = ft_substr(ee->copy_export_env[j], 0, key_length);
		int found = 0;
		k = 0;
		while (k < i)
		{
			equal_sign = ft_strchr(tmp[k], '=');
			if (equal_sign)
				key_length = equal_sign - tmp[k];
			else
				key_length = ft_strlen(tmp[k]);

			char *existing_key = ft_substr(tmp[k], 0, key_length);

			if (ft_strcmp(existing_key, new_key) == 0)
			{
				if (ft_strchr(ee->copy_export_env[j], '='))
				{
					free(tmp[k]);
					tmp[k] = ft_strdup(ee->copy_export_env[j]);
				}
				found = 1;
			}
			free(existing_key);
			k++;
		}
		free(new_key);
		if (!found && ft_strchr(ee->copy_export_env[j], '='))
		{
			tmp[i] = ft_strdup(ee->copy_export_env[j]);
			i++;
		}
		j++;
	}
	tmp[i] = NULL;
	free(ee->envp);
	ee->envp = tmp;
}

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
		if (ee->copy_export_env)
			sort_export_plus(ee);
		free(input);
	}
	else
	{
		export_with_args(ee, args);
		// free_split(ee->copy_export_env);
		// display_in_env_if_equal(ee);
	}
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

void	sort_export_plus(t_ee *ee)
{
	char **sorted_env;
	int len = 0;
	int i, j;
	char *tmp;

	while (ee->copy_export_env[len])
		len++;
	sorted_env = malloc(sizeof(char *) * (len + 1));
	if (!sorted_env)
		return ;
	i = 0;
	while (i < len)
	{
		sorted_env[i] = ft_strdup(ee->copy_export_env[i]);
		i++;
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
