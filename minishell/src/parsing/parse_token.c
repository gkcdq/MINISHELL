#include "../../minishell.h"

int count_extra_spaces(char *input)
{
    int i;
	int count;

	i = 0; 
	count = 0;
    while (input[i])
    {
        if (input[i] > 32 && input[i] != '<' && input[i] != '>' && (input[i + 1] == '<' || input[i + 1] == '>'))
            count++;
        i++;
    }
    return count;
}

char *parse_redirection_at_start(char *input)
{
    int i = 0, j = 0;
    int extra_spaces;
    char *tmp;

    extra_spaces = count_extra_spaces(input);
    tmp = malloc(sizeof(char) * (strlen(input) + extra_spaces + 1));
    if (!tmp)
        return (NULL);
    while (input[i])
    {
        if (input[i] > 32 && input[i] != '<' && input[i] != '>' && (input[i + 1] == '<' || input[i + 1] == '>'))
        {
            tmp[j++] = input[i]; 
            tmp[j++] = ' ';
        }
        else
            tmp[j++] = input[i];
        i++;
    }
    tmp[j] = '\0';
    printf("tmp = %s\n", tmp);
    return (tmp);
}

char *cut_for_no_leaks_at_the_end(char *input)
{
    char *tmp = parse_redirection_at_start(input);
    return (tmp);
}

int	check_the_end(char *input)
{
	int i = 0;
	int count_first = 0;
	int count_last = 0;

	while (input && input[i])
		i++;
	while (input[i - 1] <= 32)
		i--;
	i--;
	if (input[i] == '(')
	{
		ft_printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `newline'\n");
		return (1);
	}
	if ((input[i] == '|' || input[i] == '&'))
		return (1);
	i = 0;
	while (input[i])
	{
		if (input[i] == '(')
			count_first++;
		if (input[i] == ')')
			count_last++;
		i++;
	}
	if (count_last > count_first)
	{
		ft_printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `)'\n");
		return (1);
	}
	if (count_last < count_first)
		return (1);
	return (0);
}

int	do_you_find_or_what(char *input)
{
	int i = 0;

	while (input[i])
	{
		if (input[i] == '(')
		{
			while (input[i] != ')')
				i++;
		}
		if (input[i] == '|' && input[i + 1] == '|')
			return (1);
		i++;
	}
	return (0);
}

int	find_parenthesis(char *input)
{
	int i = 0;

	while (input[i])
	{
		if (input[i] == ')')
			return (1);
		i++;
	}
	return (0);
}

char	**parse_dollars(char **args, t_ee *ee)
{
	char	*copy;
	char	**changed_args;
	char	*after_equal;
	char	*before_equal;
	int		lock;
	int i, j, k, x, y, m;
	
	changed_args = malloc(sizeof(char *) * (ft_strlonglen(args) + 1));
	if (!changed_args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		lock = 0;
		if (args[i][0] == '$')
		{
			copy = malloc(sizeof(char) * ft_strlen(args[i]));
			if (!copy)
				return (NULL);
			j = 1;
			k = 0;
			while (args[i][j])
				copy[k++] = args[i][j++];
			copy[k] = '\0';
			x = 0;
			while (ee->envp[x])
			{
				y = 0;
				if (ft_strncmp(ee->envp[x], copy, ft_strlen(copy)) == 0
					&& ee->envp[x][ft_strlen(copy)] == '=')
				{
					y = ft_strlen(copy) + 1;
					after_equal = ft_strdup(ee->envp[x] + y);
					if (!after_equal)
						return (NULL);
					changed_args[i] = after_equal;
					lock = 1;
					break ;
				}
				x++;
			}
			if (!lock)
				changed_args[i] = ft_strdup(args[i]);
			free(copy);
		}
		else
		{
			j = 0;
			while (args[i][j] && args[i][j] != '$')
				j++;
			if (args[i][j] == '$')
			{
				m = j;
				before_equal = malloc(sizeof(char) * (m + 1));
				if (!before_equal)
					return (NULL);
				k = 0;
				while (k < m)
				{
					before_equal[k] = args[i][k];
					k++;
				}
				before_equal[m] = '\0';
				changed_args[i] = before_equal;
				lock = 1;
			}
			if (!lock)
				changed_args[i] = ft_strdup(args[i]);
		}
		i++;
	}
	changed_args[i] = NULL;
	return (changed_args);
}
