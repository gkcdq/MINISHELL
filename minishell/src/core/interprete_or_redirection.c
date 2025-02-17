#include "../../minishell.h"

void	if_confirmed_command_equal_to_zero(char *command_after_or, t_ee *ee)
{
	cumulate_token(command_after_or, ee);
	if (ee->save_result == 1)
	{
		ee->check_and_validity = 1;
		ee->save_result = 0;
	}
}

/////////////////////////PROBLEME A REGLER (LOL || ls && pwd) (pwd ; (cat Makefile | lol || cat Makefile | kek) && pwd)

void	after_find_or(char *input, t_ee *ee)
{
	char	*command_before_or;
	char	*command_after_or;
	char	*see_what_after;
	char	*path;

	command_after_or = NULL;
	if (check_after_or(input) == 0)
	{
		command_before_or = copy_before_or(input);
		command_after_or = copy_after_or(input);
		see_what_after = copy_after_or(command_after_or);
		path = find_command_path(command_after_or);
		if (see_what_after == NULL && !path)
			ee->save_result = 1;
		else if (see_what_after == NULL && path)
			ee->save_result = 0;
		free(see_what_after);
		free(path);
		cumulate_token(command_before_or, ee);
		if (ee->confirmed_command == 0)
			if_confirmed_command_equal_to_zero(command_after_or, ee);
		free_after_or(command_before_or, command_after_or, ee);
		return ;
	}
}

int	handle_or_and_redirection(char *input, t_ee *ee)
{
	if (find_or(input) == 1)
	{
		after_find_or(input, ee);
		return (1);
	}
	if (find_redirection(input) == 1)
	{
		handle_redirection(input, ee);
		return (1);
	}
	return (0);
}

void	init_value_token(t_token *token, char *input)
{
	token->token = 0;
	token->trimmed_input = NULL;
	token->trimmed_input = skip_isspace_for_fonctions(input, token);
	if (!token->trimmed_input)
	{
		if (token)
			free(token);
		return ;
	}
	return ;
}

void	at_the_end(t_token *token, t_ee *ee)
{
	if (token->trimmed_input)
		free(token->trimmed_input);
	if (token)
		free(token);
	ee->signal = 0;
}
