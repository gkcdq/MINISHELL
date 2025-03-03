/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cumulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmilin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:27:44 by tmilin            #+#    #+#             */
/*   Updated: 2025/02/27 19:27:48 by tmilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void init_normalizer(t_quote_normalizer *q)
{
    q->i = 0;
    q->quote_type = '\0';
    q->k = 0;
}

int check_quotes(char *arg, t_quote_normalizer *q)
{
    int len = strlen(arg);
    if (len >= 2 && (arg[0] == '"' || arg[0] == '\'') && arg[0] == arg[len - 1])
    {
        q->quote_type = arg[0];
        return 1;
    }
    return 0;
}

void process_quotes(char *arg, t_quote_normalizer *q)
{
    int len = strlen(arg);
    
    q->result[q->k++] = arg[0];
    q->j = 1;
    while (q->j < len - 1) 
    {
        if ((q->quote_type == '"' && arg[q->j] == '\'') || 
            (q->quote_type == '\'' && arg[q->j] == '"'))
        {
            q->j++; 
            continue;
        }
        q->result[q->k++] = arg[q->j]; 
        q->j++;  
    }
    q->result[q->k++] = arg[len - 1];
    q->result[q->k] = '\0';
}


void normalize_quotes(char **args)
{
    t_quote_normalizer q;

    init_normalizer(&q);
    while (args[q.i])
    {
        if (check_quotes(args[q.i], &q))
        {
            process_quotes(args[q.i], &q);
            strcpy(args[q.i], q.result);
        }
        q.i++;
    }
}


/*void normalize_quotes(char **args)
{
    int i = 0;
    while (args[i])
    {
        int j = 0, k = 0;
        char quote_type = '\0';
        char result[2048];
        int len = strlen(args[i]);
        if (len >= 2 && (args[i][0] == '"' || args[i][0] == '\'') && args[i][0] == args[i][len - 1])
            quote_type = args[i][0];
        if (quote_type)
        {
            result[k++] = args[i][0];
            for (j = 1; j < len - 1; j++)
            {
                if ((quote_type == '"' && args[i][j] == '\'') || 
                    (quote_type == '\'' && args[i][j] == '"'))
                    continue;
                result[k++] = args[i][j];
            }
            result[k++] = args[i][len - 1];
            result[k] = '\0';
            strcpy(args[i], result);
        }
        i++;
    }
}*/

////////////////////////////////////////////////////


int	process_segment(char *segment, t_ee *ee)
{
	char	**changed_args;
	char	*input;
	int		result;

	changed_args = check_dollars(segment, ee);
	if (!changed_args)
		return (1);
	normalize_quotes(changed_args);
	input = reconstruct_input(changed_args);
	result = 0;
	if (find_parenthesis(segment) && found_single_or_double_quote(input) == 0)
		its_just_a_parenthese(segment, ee);
	else if (find_pipe(input) && !find_or(input)
		&& !find_redirection(input))
		execute_pipeline(input, ee);
	else if (find_redirection_and_pipe(input) && found_single_or_double_quote(input) == 0)
		execute_pipeline_heredoc(input, ee);
	else
		result = interprete_commande(input, ee);
	free_split(changed_args);
	free(input);
	return (result);
}

char	*copy_pasta(char *l, int *i, t_ee *ee)
{
	int		j;
	char	*copy;

	(void)ee;
	j = 0;
	copy = malloc(2048);
	if (!copy)
		return (NULL);
	while (l[*i] != '\0' && l[*i] != ';' && !(l[*i] == '&' && l[*i + 1] == '&'))
	{
		if (l[*i] == '(')
			copy_until_parenthesis(l, i, copy, &j);
		else if (l[*i] == '"' || l[*i] == '\'')
			copy_until_close_quote(l, i, copy, &j);
		else
		{
			copy[j++] = l[*i];
			(*i)++;
		}
	}
	copy[j] = '\0';
	return (copy);
}

bool	process_token(char *input, int *i, t_ee *ee, bool success)
{
	char	*copy;

	copy = copy_pasta(input, i, ee);
	if (!copy)
		return (false);
	if (input[*i] == '"' || input[*i] == '\'')
	{
		char quote_type = input[*i];
		(*i)++;
		while (input[*i] && input[*i] != quote_type)
			(*i)++;
		if (input[*i] == quote_type)
			(*i)++;
	}
	if (input[*i] == '&' && input[*i + 1] == '&')
		ee->command_with_and = 1;
	if (success)
		success = (process_segment(copy, ee) == 0);
	ee->command_with_and = 0;
	free(copy);
	if (ee->check_and_validity)
	{
		success = false;
		ee->check_and_validity = 0;
	}
	return (success);
}

int	cumulate_token(char *input, t_ee *ee)
{
	int		i;
	bool	success;

	i = 0;
	success = true;
	while (input[i] != '\0')
	{
		success = process_token(input, &i, ee, success);
		if (input[i] == ';')
		{
			success = true;
			if (check_after_token(input, i) == 0)
				break ;
			i++;
		}
		if (input[i] == '&' && input[i + 1] == '&')
			i += 2;
	}
	return (0);
}

void	free_after_or(char *command_before_or, char *command_after_or, t_ee *ee)
{
	if (command_before_or)
		free(command_before_or);
	if (command_after_or)
		free(command_after_or);
	ee->command_with_or = 0;
	ee->confirmed_command = 0;
	return ;
}
