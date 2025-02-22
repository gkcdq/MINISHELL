#include "../../minishell.h"

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = 130;
	}
}

void	catch_signal(t_ee *ee)
{
	(void)ee;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_herdoc_sigint(int status)
{
	rl_replace_line("", 0);
	ft_putendl_fd("\n", 1);
	rl_on_new_line();
	unlink("/tmp/heredoc_tmp");
	exit(status);
}
