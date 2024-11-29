#include "../../minishell.h"

char	*parse_input_ls(char *input)
{
	int	i;

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

void	ft_ls(char *input)
{
	char **args;
	char *path;
	DIR *dir;
	struct dirent *entry;

	input = parse_input_ls(input);
	args = ft_split(input, ' ');
    if (args[1] == NULL)
    {
        path = ".";
        dir = opendir(path);
    }
    else
	    dir = opendir(args[1]);
	if (!dir)
	{
		printf("💔_(ಥ﹏ಥ)_💔: cannot access\n");
		return ;
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] != '.')
			printf("%s ", entry->d_name);
	}
	printf("\n");
	closedir(dir);
}