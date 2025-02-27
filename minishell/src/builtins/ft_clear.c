/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmilin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:25:26 by tmilin            #+#    #+#             */
/*   Updated: 2025/02/27 19:25:27 by tmilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*parse_input_clear(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ';' && (input[i - 1] != ' '))
		{
			input[i] = '\0';
			break ;
		}
		i++;
	}
	return (input);
}

void	ft_clear(char *input)
{
	char	**args;

	input = parse_input_clear(input);
	args = ft_split(input, ' ');
	if (args[1] != NULL)
		printf("🦇 (¬‿¬) 🦇: Usage: clear\n");
	else
		system("clear");
	free_split(args);
}
