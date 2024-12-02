#include "../../minishell.h"


char	*parse_input_cd(char *input)
{
	int i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ';' && (input[i - 1] != ' '))
		{
			input[i] = '\0';
			break ;
		}
		i++;
	}
	return (input);
}

void	ft_cd(char *input)
{
	t_cd *cd;
	char *cwd;
	int result;

	cd = malloc(sizeof(t_cd));
	input = parse_input_cd(input);
	cd->args = ft_split(input, ' ');
	if (cd->args[1] == NULL || ft_strcmp(cd->args[0], "~") == 0)
	{
		cd->home = getenv("HOME");
		if (cd->home == NULL)
		{
			printf("🍂_(´~`)_🍂: HOME not set\n");
			free_split(cd->args);
			return ;
		}
		result = chdir(cd->home);
	}
	else
		result = chdir(cd->args[1]);
	if (result != 0)
	{
		perror("🍂_(´~`)_🍂: cd");
		free_split(cd->args);
		return ;
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		setenv("PWD", cwd, 1);
		free(cwd);
	}
	else
		perror("getcwd");
	free_split(cd->args);
	free(cd);
}
