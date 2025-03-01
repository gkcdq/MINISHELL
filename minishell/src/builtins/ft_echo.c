/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmilin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:25:36 by tmilin            #+#    #+#             */
/*   Updated: 2025/02/27 19:25:37 by tmilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int confirme_n(char *input)
{
	int i;

	i = 0;
	if (input[i] != '-')
		return (0);
	i = 1;
	while (input[i] && input[i] == 'n')
	{
		if (input[i + 1] == '\0')
			return (1);
		i++;
	}
	return (0);
}

void	ft_echo(char *input, t_ee *ee)
{
	char	**args;
	bool	no_newline;
	int		i;

	no_newline = false;
	i = 1;
	args = ft_split(input, ' ');
	if (args[i] && confirme_n(args[i]) == 1)
	{
		no_newline = true;
		i++;
	}
	while (args[i])
	{
		if (ft_strcmp(args[i], "$?") == 0)
			ft_printf("%d", ee->signal);
		else
			ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!no_newline)
		ft_printf("\n");
	free_split(args);
}
