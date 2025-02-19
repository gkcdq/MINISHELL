#include "../../minishell.h"

int	find_pipe(char *input)
{
    int i = 0;

    while (input && input[i])
    {
        if (input[i] == '|' && (input[i + 1] != '|'))
            return (1);
        if (input[i] == '|' && input[i + 1] == '|')
            return (0);
        i++;
    }
    return (0);
}

void	for_norminette(t_ee *ee)
{
	printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `&'\n");
	ee->signal = 2;
}

int	check_tmp_not_null(char *tmp, t_ee *ee)
{
	if (tmp == NULL)
	{
		ee->minishell_check = 1;
		free(tmp);
		return (1);
	}
	return (0);
}

int	skip_whitespace(const char *tmp)
{
	int	i;

	i = 0;
	while (tmp[i] && tmp[i] <= 32)
		i++;
	return (i);
}

int	skip_whitespacee(const char *tmp, int i)
{
	while (tmp[i] && tmp[i] <= 32)
		i++;
	return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		else
			i++;
	}
	return (0);
}

int	is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

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

char	*skip_isspace_for_fonctions(char *input, t_token *token)
{
	char	*s;
	int		i;

	i = 0;
	if (check_token(input, token) == 1)
		return (";");
	else
	{
		s = malloc(sizeof(char) * ft_strlen(input) + 1);
		while (*input <= 32)
			input++;
		while (*input > 32)
		{
			s[i] = *input;
			i++;
			input++;
		}
		s[i] = '\0';
	}
	return (s);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	while (dest[i])
		i++;
	j = 0;
	while (src[j])
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strlonglen(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
