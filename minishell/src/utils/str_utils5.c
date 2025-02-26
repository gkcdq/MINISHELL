#include "../../minishell.h"

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (to_find && to_find[i] && str[i + j])
		{
			if (to_find[j] == '\0')
				return (str + i);
			j++;
		}
		i++;
	}
	return (NULL);
}

int	check_atoi_overflow(char *str)
{
	long long int	result;
	int				sign;
	int				digit;

	result = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			return (1);
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (result > (LLONG_MAX - digit) / 10)
			return (1);
		result = result * 10 + digit;
		str++;
	}
	return (0);
}
