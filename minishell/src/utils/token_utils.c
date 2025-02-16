#include "../../minishell.h"

int	check_token_in_all_string(char *input, t_token *tok)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (input[i])
	{
		if (input[i] == ';' && input[i + 1] == ';')
			break ;
		if (input[i] == 39)
			single_quote++;
		if (input[i] == 34)
			double_quote++;
		i++;
	}
	if (single_quote % 2 != 0 || double_quote % 2 != 0)
		return (0);
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
		}
		if (input[i] == ';')
		{
			if (input[i + 1] == '\0')
				return (0);
			if (input[i + 1] == ';')
			{
				tok->token = 3;
				return (1);
			}
			i++;
			while (input[i] && input[i] <= 32)
				i++;
			if (input[i] == ';')
			{
				tok->token = 2;
				return (1);
			}
			if (input[i] == '\0')
				return (0);
		}
		i++;
	}
	return (0);
}

int	check_token(char *input, t_token *token)
{
	int	i;

	i = 0;
	while (input[i] <= 32)
		i++;
	if (input[i] == ';' && input[i + 1] != ';')
	{
		token->token = 1;
		return (1);
	}
	return (0);
}

int	token_found(char *input, t_token *tok)
{
	int i;

	i = 0;
	while (input[i] <= 32)
		i++;
	if (input[i] == '\0')
		return (0);
	while (input[i])
	{
		if (input[i] == ';' && input[i + 1] == '\0')
			return (0);
		if (input[i] == ';' && input[i + 1] != '\0')
		{
			tok->found = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_after_token(char *input, int i)
{
	int confirme;

	confirme = 0;
	if (input[i] && input[i + 1])
		i++;
	while (input[i] && input[i] <= 32)
	{
		if (input[i] > 32)
		{
			confirme = 1;
			break ;
		}
		i++;
	}
	if (confirme == 1)
		return (0);
	else
		return (1);
}
