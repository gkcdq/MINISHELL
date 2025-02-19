#include "../minishell.h"

int g_status = 0;

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

void	init_quote_struct(t_quote *qt)
{
	qt->single_quotes = 0;
	qt->double_quotes = 0;
	qt->temp = NULL;
	qt->next_line = NULL;
}

void	init_pipe_struct(t_pipe *pipe)
{
	pipe->commands = NULL;
	pipe->prev_fd = -1;
	pipe->i = 0;
	pipe->pid = 0;
}

void	init_struct_loop(t_loop *loop)
{
	loop->changed_args = NULL;
	loop->cleaned_input = NULL;
	loop->input = NULL;
}

void	init_struct(t_ee *ee)
{
	ee->minishell_check = 0;
	ee->change_confirmed = 0;
	ee->copy_pwd = NULL;
	ee->copy_oldpwd = NULL;
	ee->if_unset__pwd = 0;
	ee->if_unset__oldpwd = 0;
	ee->if_unset__shlvl = 0;
	ee->lock_path = 0;
	ee->copy_export_env = NULL;
	ee->path_is_not_able = 0;
	ee->save_initial_path = NULL;
	ee->check_and_validity = 0;
	ee->command_with_and = 0;
	ee->command_with_or = 0;
	ee->confirmed_command = 0;
	ee->signal = 0;
	ee->save_result = 0;
}
