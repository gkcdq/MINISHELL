#include "../minishell.h"

int		g_status = 0;

t_redir_simple	*init_redirr_handler(char *input)
{
	t_redir_simple	*hr;

	hr = malloc(sizeof(t_redir_simple));
	if (!hr)
		return (NULL);
	hr->input_execv = parse_exev_input(input);
	hr->if_need_sep = unstick_to_re_stick(input);
	hr->split_execv = ft_split(hr->input_execv, ' ');
	hr->split_in = ft_split(hr->if_need_sep, ' ');
	if (!hr->split_execv || !hr->split_execv[0] || !hr->split_in
		|| !hr->split_in[0])
	{
		free_if_simple_redir_fail(hr);
		return (NULL);
	}
	hr->last_name = ft_strlonglen(hr->split_in);
	hr->path = find_command_path(hr->split_in[0]);
	hr->heredoc_tmpfile = NULL;
	hr->heredoc_fd = -1;
	hr->heredoc_count = 0;
	hr->delimiter = NULL;
	return (hr);
}

void	init_p_fd(t_pipeline *p)
{
	p->heredoc_fd = -1;
	p->input_fd = -1;
	p->output_fd = -1;
}

int	i_want_to_sing_a_song_hiihi(t_redir_handler *hr)
{
	if (!hr->split_in || !hr->split_in[0])
	{
		free(hr->tmp);
		free_split(hr->split_in);
		return (1);
	}
	return (0);
}

void	init_redir_handler(t_redir_handler *handler, char *input)
{
	handler->tmp = unstick_to_re_stick(input);
	if (!handler->tmp)
		return ;
	handler->split_in = ft_split(handler->tmp, ' ');
	if (i_want_to_sing_a_song_hiihi(handler))
		return ;
	handler->split_execv = NULL;
	handler->last_name = ft_strlonglen(handler->split_in);
	handler->file = -1;
	handler->pid = 0;
	handler->input_execv = parse_exev_input(input);
	handler->path = find_command_path(handler->split_in[0]);
	handler->heredoc_tmpfile = NULL;
	handler->heredoc_fd = -1;
	handler->delimiter = NULL;
	handler->heredoc_count = 0;
	handler->fd = -1;
	handler->status = 0;
	handler->i = 1;
	handler->re = malloc(sizeof(t_redir));
	if (!handler->re)
		return ;
	handler->re->command_fail = 0;
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

void	init_quote_handler(t_quote_handler *qh)
{
	qh->size = 1024;
	qh->i = 0;
	qh->j = 0;
	qh->quote = '\0';
	qh->expanded = NULL;
	qh->expanded_len = 0;
	qh->result = malloc(qh->size);
}

void	init_remover(t_parentheses_remover *r, char *input)
{
	r->i = 0;
	r->j = 0;
	r->level = 0;
	r->tmp = malloc(sizeof(char) * (strlen(input) + 1));
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
