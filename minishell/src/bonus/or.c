#include "../../minishell.h"

int	check_after_or(char *input)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	while (i > 0)
	{
		if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A'
				&& input[i] <= 'Z'))
			return (0);
		if (input[i] == '|')
			return (1);
		i--;
	}
	return (0);
}

int	find_or(char *input)
{
	int	i;

	i = 0;
	while (input && input[i])
	{
		if (input[i] == '|' && input[i + 1] == '|')
			return (1);
		i++;
	}
	return (0);
}

char	*copy_before_or(char *src)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (src && src[i])
	{
		if (src[i] == '(')
		{
			while (src[i] != ')')
				i++;
		}
		if (src[i] == '|' && src[i + 1] == '|')
			break ;
		i++;
	}
	tmp = malloc(sizeof(char) * i + 1);
	while (j < i)
	{
		tmp[j] = src[j];
		j++;
	}
	tmp[j] = '\0';
	return (tmp);
}

char	*copy_after_or(char *src)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	if (!src || src == NULL)
		return (NULL);
	while (src && src[i])
	{
		if (src[i] == '(')
		{
			while (src[i] != ')')
				i++;
		}
		if (src[i] == '|' && src[i + 1] == '|')
			break ;
		i++;
	}
	if (src[i] == '\0')
		return (NULL);
	i += 2;
	tmp = complete_tmp(src, i);
	return (tmp);
}

char	*complete_tmp(char *src, int i)
{
	int		k;
	int		j;
	char	*tmp;

	k = ft_strlen(src);
	tmp = malloc(sizeof(char) * (k - i + 1));
	if (!tmp)
	{
		free(tmp);
		return (NULL);
	}
	j = 0;
	while (i < k)
	{
		tmp[j] = src[i];
		i++;
		j++;
	}
	tmp[j] = '\0';
	if (tmp == NULL)
		free(tmp);
	return (tmp);
}

/*char *copy_after_or(char *src)
{
	int		i;
	int		k;
	int		j;
	char	*tmp;
	int		i;
	char	*tmp;

	i = 0;
	k = 0;
	j = 0;
	tmp = NULL;
	if (!src || src == NULL)
		return (NULL);
	while (src && src[i])
	{
		if (src[i] == '(')
		{
			while (src[i] != ')')
				i++;
		}
		if (src[i] == '|' && src[i + 1] == '|')
			break ;
		i++;
	}
	if (src[i] == '\0')
		return (NULL);
	i += 2;
	k = ft_strlen(src);
	tmp = malloc(sizeof(char) * (k - i + 1));
	if (!tmp)
	{
		free (tmp);
		return (NULL);
	}
	while (i < k)
	{
		tmp[j] = src[i];
		i++;
		j++;
	}
	tmp[j] = '\0';
	if (tmp == NULL)
		free(tmp);
	return (tmp);
}*/
