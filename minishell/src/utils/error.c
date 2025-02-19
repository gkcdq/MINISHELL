#include "../../minishell.h"

int handle_error_piperedi(const char *msg, t_pipeline *p, char *input)
{
    perror(msg); 
    free_split(p->commands); 
    free(input);  
    free(p);
    return -1;  
}

int	handle_error(const char *msg, t_pipeline *p, char *input)
{
	perror(msg);
	free_split(p->commands);
	free(input);
	free(p);
	return (-1);
}
