#include "../../minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	char *dup;
	size_t len = strnlen(s, n);

	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	memcpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}

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

void	sort_export(t_ee *ee, char **sorted);
void	sort_export_plus(t_ee *ee);

int	ft_found_equal(char c)
{
	if (c == '=')
		return (1);
	return (0);
}

void handle_env_with_equals(char **args, char ***copy_envp, int *len_envp)
{
    for (int i = 1; args[i]; i++)
    {
        if (args[i][0] == '=')
        {
            ft_printf("🔥_(╬ Ò﹏Ó)_🔥: export: %s: not a valid identifier\n", args[i]);
            continue;
        }
        if (ft_strchr(args[i], '=') && args[i][0] != '=')
        {
            int found = 0;
            for (int j = 0; j < *len_envp; j++)
            {
                size_t key_len = ft_strchr(args[i], '=') - args[i];
                if (ft_strncmp((*copy_envp)[j], args[i], key_len) == 0 && (*copy_envp)[j][key_len] == '=')
                {
                    free((*copy_envp)[j]);
                    (*copy_envp)[j] = ft_strdup(args[i]);
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                (*copy_envp)[*len_envp] = ft_strdup(args[i]);
                (*len_envp)++;
                (*copy_envp)[*len_envp] = NULL;
            }
        }
    }
}

int ft_check_equal(const char *s)
{
    return (ft_strchr(s, '=') != NULL);
}

void handle_export_without_equals(char **args, char ***copy_export, int *len_export)
{
    for (int i = 1; args[i]; i++)
    {
        if (ft_check_equal(args[i]))
            continue;

        int found = 0;
        for (int j = 0; j < *len_export; j++)
        {
            char *current_name = ft_strndup((*copy_export)[j], ft_strchr((*copy_export)[j], '=') - (*copy_export)[j]);
            if (ft_strcmp(current_name, args[i]) == 0)
            {
                found = 1;
                free(current_name);
                break;
            }
            free(current_name);
        }

        if (!found)
        {
            (*copy_export)[*len_export] = ft_strdup(args[i]);
            (*len_export)++;
            (*copy_export)[*len_export] = NULL;
        }
    }
}

void export_with_args(t_ee *ee, char **args)
{
    char **copy_envp = NULL;
    char **copy_export = NULL;
    int len_envp = 0, len_export = 0;
    int count_equal = 0, count_args_without_equal = 0;

    for (int i = 1; args[i]; i++)
    {
        if (ft_check_equal(args[i]))
            count_equal++;
        else
            count_args_without_equal++;
    }

    if (ee->envp)
    {
        while (ee->envp[len_envp])
            len_envp++;
        copy_envp = malloc(sizeof(char *) * (len_envp + count_equal + 1));
        for (int i = 0; i < len_envp; i++)
            copy_envp[i] = ft_strdup(ee->envp[i]);
        copy_envp[len_envp] = NULL;
    }
    else
        copy_envp = malloc(sizeof(char *) * (count_equal + 1));

    if (ee->copy_export_env)
    {
        while (ee->copy_export_env[len_export])
            len_export++;
        copy_export = malloc(sizeof(char *) * (len_export + count_args_without_equal + 1));
        for (int i = 0; i < len_export; i++)
            copy_export[i] = ft_strdup(ee->copy_export_env[i]);
    }
    else if (count_args_without_equal > 0)
        copy_export = malloc(sizeof(char *) * (count_args_without_equal + 1));

    if (copy_export)
        copy_export[len_export] = NULL;

    handle_env_with_equals(args, &copy_envp, &len_envp);
    handle_export_without_equals(args, &copy_export, &len_export);

    free_split(ee->envp);
    ee->envp = copy_envp;

    free_split(ee->copy_export_env);
    ee->copy_export_env = copy_export;
}

char **copi_colle(t_ee *ee)
{
    int len_envp = ft_strlonglen(ee->envp);
    int len_export = ft_strlonglen(ee->copy_export_env);
    char **tmp = malloc(sizeof(char *) * (len_envp + len_export + 1));

    int i = 0;
    for (; i < len_envp; i++)
        tmp[i] = ft_strdup(ee->envp[i]);

    for (int j = 0; j < len_export; j++, i++)
        tmp[i] = ft_strdup(ee->copy_export_env[j]);

    tmp[i] = NULL;
    return tmp;
}

void ft_export(char *input, t_ee *ee)
{
    char **args = ft_split(input, ' ');

    if (!ee->envp || !ee->envp[0])
    {
        if (!ee->copy_oldpwd && ee->if_unset__oldpwd == 0)
            printf("🏗️ 🏡 OLDPWD\n");
        else if (ee->copy_oldpwd && ft_strcmp(ee->copy_oldpwd, ee->copy_pwd) != 0 && ee->if_unset__oldpwd == 0)
            printf("🏗️ 🏡 OLDPWD=%s\n", ee->copy_oldpwd);
        if (!ee->copy_pwd)
            ee->copy_pwd = getcwd(NULL, 0);
        if (ee->if_unset__pwd == 0)
            printf("🏗️ 🏡 PWD=%s\n", ee->copy_pwd);
        if (ee->if_unset__shlvl == 0)
            printf("🏗️ 🏡 SHLVL=1\n");
        free_split(args);
        return;
    }

    if (ft_strcmp(args[0], "export=") == 0)
    {
        free_split(args);
        return;
    }

    if (!args[1])
    {
        char **sorted_env = copi_colle(ee);
        sort_export(ee, sorted_env);
        free_split(sorted_env);
    }
    else
    {
        export_with_args(ee, args);
        check_if_path_is_set(ee, args);
    }

    free_split(args);
}


char	*ft_strcat_export(char *s1, char *s2);

void	sort_export(t_ee *ee, char **sorted)
{
	char **sorted_env;
	int len = 0;
	int i, j;
	char *tmp;
	char *oldpwd;

	while (sorted[len])
		len++;
	sorted_env = malloc(sizeof(char *) * (len + 2));
	if (!sorted_env)
		return ;
	i = 0;
	while (i < len)
	{
		sorted_env[i] = ft_strdup(sorted[i]);
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

/*void	sort_export_plus(t_ee *ee)
{
	char **sorted_env;
	int len = 0;
	int i, j;
	char *tmp;

	if (!ee->copy_export_env)
		return ;
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
}*/

char	*ft_strcat_export(char *s1, char *s2)
{
	int len1 = ft_strlen(s1);
	int len2 = ft_strlen(s2);
	int i;
	int j;
	char *result;

	result = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
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

void	check_if_path_is_set(t_ee *ee, char **args)
{
	int i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "PATH=/bin:/usr/bin") == 0
			|| (ee->save_initial_path && ft_strcmp(args[i],
					ee->save_initial_path) == 0))
		{
			setenv("PATH", "/bin:/usr/bin", 1);
			ee->path_is_not_able = 0;
			break ;
		}
		i++;
	}
}
