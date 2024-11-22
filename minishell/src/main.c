#include "../minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (1);
		else
			i++;
	}
	return (0);
}

void	interprete_commande(char *input, t_global *global)
{
	if (strcmp(input, "exit") == 0)
	{
		printf("Big alt f4..\n");
		ft_exit(global);
	}
	else
	{
		printf("You entered: %s\n", input);
	}
}

int	main(int ac, char **av, char **envp)
{


	char *input;

    (void)ac;
    (void)av;
    (void)envp;
    t_global *global = malloc(sizeof(t_global));
    init_global(global);
	while (global->minishell_check == 0)
	{
        printf("%d", global->minishell_check);
		input = readline("🍀_(^o^)_🍀 -> ");
		if (input && *input)
		{
			add_history(input);
			interprete_commande(input, global);
		}
		free(input);

	}

	return (0);
}