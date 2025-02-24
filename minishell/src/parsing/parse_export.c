#include "../../minishell.h"

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

int	ft_found_equal(char c)
{
	if (c == '=')
		return (1);
	return (0);
}

int ft_check_equal(const char *s)
{
    return (ft_strchr(s, '=') != NULL);
}
