#include "../../minishell.h"

void	at_the_end(t_token *token, t_ee *ee)
{
	if (token->trimmed_input)
		free(token->trimmed_input);
	if (token)
		free(token);
	ee->signal = 0;
}