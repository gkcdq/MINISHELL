#include "../minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_ee	*ee;
	char	*input;

	ee = malloc(sizeof(t_ee));
	input = NULL;
	(void)ac;
	(void)av;
	init_struct(ee);
	ee->envp = copy_envp(envp);
	ee->save_initial_path = save_initial_path(ee);
	ee->copy_home = save_initial_home(ee);
	catch_signal(ee);
	while (ee->minishell_check == 0)
		loop(input, ee);
	all_is__freed(ee);
	clear_history();
	return (0);
}
