#include "../../minishell.h"

void	redi_pipe_free(t_redir_handler *hr)
{
	free_split(hr->split_in);
	free(hr->re);
	free(hr->input_execv);
	free(hr->path);
	free(hr->heredoc_tmpfile);
	free(hr->tmp);
	free(hr);
	return ;
}

void	redi_last_free(t_redir_handler *hr)
{
	free_split(hr->split_in);
	free(hr->input_execv);
	free(hr->path);
	free(hr->re);
	if (hr->heredoc_tmpfile)
	{
		unlink(hr->heredoc_tmpfile);
		free(hr->heredoc_tmpfile);
	}
	free(hr->tmp);
	free(hr);
}

void	free_if_pid_is_n(t_redir_simple *hr)
{
	free_split(hr->split_execv);
	free_split(hr->split_in);
	free(hr->input_execv);
	free(hr->if_need_sep);
	free(hr->path);
	free(hr);
	return ;
}

void	free_at_last(t_redir_simple *hr)
{
	free_split(hr->split_execv);
	free_split(hr->split_in);
	free(hr->input_execv);
	free(hr->if_need_sep);
	free(hr->path);
	free(hr);
}

void	free_if_simple_redir_fail(t_redir_simple *hr)
{
	free(hr->input_execv);
	free(hr->if_need_sep);
	free_split(hr->split_execv);
	free_split(hr->split_in);
	free(hr);
	return ;
}
