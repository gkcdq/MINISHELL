#include "../../minishell.h"

void    ft_exit(void)
{
    ft_printf("🏃 exit\n");
    g_minishell_check = 1;
}