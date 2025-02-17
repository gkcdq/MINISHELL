#include "../../minishell.h"

int	process_segment(char *segment, t_ee *ee)
{
	char	**changed_args;
	char	*reconstructed_input;
	int		result;

	changed_args = check_dollars(segment, ee);
	if (!changed_args)
		return (1);
	reconstructed_input = reconstruct_input(changed_args);
	result = 0;
	if (find_parenthesis(segment))
		its_just_a_parenthese(segment, ee);
	else if (find_pipe(reconstructed_input) && !find_or(reconstructed_input)
		&& !find_redirection(reconstructed_input))
		execute_pipeline(reconstructed_input, ee);
	else if (find_pipe(reconstructed_input)
		&& find_redirection(reconstructed_input))
		execute_pipeline_heredoc(reconstructed_input, ee);
	else
		result = interprete_commande(reconstructed_input, ee);
	free_split(changed_args);
	free(reconstructed_input);
	return (result);
}

char	*copy_pasta(char *l, int *i)
{
	int		j;
	char	*copy;

	j = 0;
	copy = malloc(2048);
	if (!copy)
		return (NULL);
	while (l[*i] != '\0' && l[*i] != ';' && !(l[*i] == '&' && l[*i + 1] == '&'))
	{
		if (l[*i] == '(')
			copy_until_parenthesis(l, i, copy, &j);
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

	copy = copy_pasta(input, i);
	if (!copy)
		return (false);
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
