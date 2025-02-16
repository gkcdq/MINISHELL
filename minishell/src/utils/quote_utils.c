#include "../../minishell.h"

char *find_env_var(const char *var_name, t_ee *ee)
{
    int i = 0;
    int name_len = ft_strlen(var_name);

    while (ee->envp && ee->envp[i])
	{
        if (ft_strncmp(ee->envp[i], var_name, name_len) == 0 && ee->envp[i][name_len] == '=')
            return ft_strdup(&ee->envp[i][name_len + 1]);
        i++;
    }
    return NULL;
}

char *expand_variable(char *input, t_ee *ee)
{
    char var_name[256];
    int i = 1;
    int j = 0;
    char *value;

    while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
	{
        if (j < 255)
            var_name[j++] = input[i++];
    }
    var_name[j] = '\0';

    value = find_env_var(var_name, ee);
    if (value)
        return value;
    return ft_strdup("");
}

char *handle_quotes(char *input, t_ee *ee)
{
    char *result = malloc(1024);
    int size = 1024;
    int i = 0, j = 0;
    char quote = '\0';
    char *expanded;
	int expanded_len;

    if (!result)
        return NULL;
    while (input[i])
	{
        if ((input[i] == '\'' || input[i] == '"') && quote == '\0')
            quote = input[i++];
		else if (input[i] == quote)
		{
            quote = '\0';
            i++;
        } 
		else
		{
            if (quote == '"' && input[i] == '$')
			{
                expanded = expand_variable(&input[i], ee);
                if (expanded)
				{
                    expanded_len = ft_strlen(expanded);
                    if (j + expanded_len >= size)
					{
                        size *= 2;
                        result = realloc(result, size);
                        if (!result) {
                            free(expanded);
                            return NULL;
                        }
                    }
                    strcpy(&result[j], expanded);
                    j += expanded_len;
                    free(expanded);
                    i++;
                    while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                        i++;
                    continue;
                }
            }
            if (j + 1 >= size)
			{
                size *= 2;
                result = realloc(result, size);
                if (!result)
                    return NULL;
            }
            result[j++] = input[i++];
        }
    }
    result[j] = '\0';
    return result;
}
