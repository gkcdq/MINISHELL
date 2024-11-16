#include "../minishell.h"

int main (int ac, char **av)
{
    if (ac == 2)
    {
        int i = ft_strlen(av[1]);
        ft_printf("%d\n", i);
    }
    return 0;
}