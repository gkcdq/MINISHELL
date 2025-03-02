/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmilin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:38:03 by tmilin            #+#    #+#             */
/*   Updated: 2025/02/27 19:38:04 by tmilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**check_dollars(char *input, t_ee *ee)
{
	char	**args;
	char	**changed_args;

	args = ft_split(input, ' ');
	if (!args)
		return (NULL);
	int i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "$OLDPWD") == 0)
		{
			free(args[i]);
			if (ee->copy_oldpwd)
				args[i] = ft_strdup(ee->copy_oldpwd);
			else
				args[i] = ft_strdup("");
			if (!args[i])
			{
				free_split(args);
				return (NULL);
			}
		}
		i++;
	}
	changed_args = parse_dollars(args, ee);
	if (!changed_args)
		return (NULL);
	free_split(args);
	return (changed_args);
}

char	*remove_parentheses(char *input)
{
	t_parentheses_remover	r;

	init_remover(&r, input);
	if (!r.tmp)
		return (NULL);
	while (input[r.i])
	{
		if (input[r.i] == '(' && r.level++ > 0)
			r.tmp[r.j++] = input[r.i];
		else if (input[r.i] == ')' && --r.level > 0)
			r.tmp[r.j++] = input[r.i];
		else if (r.level > 0 || input[r.i] != ')')
			r.tmp[r.j++] = input[r.i];
		r.i++;
	}
	r.tmp[r.j] = '\0';
	return (r.tmp);
}

int	count_extra_spaces_pipeline(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (i > 0 && input[i - 1] > 32)
				count++;
			if (input[i + 1] && input[i + 1] > 32)
				count++;
		}
		i++;
	}
	return (count);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	if (!dest || !src)
		return (0);
	i = 0;
	if (size > 0)
	{
		while (src[i] && i < size - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	if (!dest || !src)
		return (NULL);
	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
