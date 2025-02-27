/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmilin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:26:37 by tmilin            #+#    #+#             */
/*   Updated: 2025/02/27 19:26:38 by tmilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_ls(char *input, t_ee *ee)
{
	t_ls	ls;

	if (ee->lock_path == 0)
	{
		init_ls(&ls, input);
		if (!open_directory(&ls))
		{
			clean_up_ls(&ls);
			return ;
		}
		print_files(&ls);
		clean_up_ls(&ls);
	}
	else
		ft_printf("🍁_(`へ´*)_🍁: ls: command not found\n");
}

int	open_directory(t_ls *ls)
{
	ls->dir = opendir(ls->path);
	if (!ls->dir)
	{
		printf("💔_(ಥ﹏ಥ)_💔: cannot access '%s': No such file or directory\n",
			ls->path);
		return (0);
	}
	return (1);
}

void	print_files(t_ls *ls)
{
	ls->file_found = 0;
	while (1)
	{
		ls->entry = readdir(ls->dir);
		if (ls->entry == NULL)
			break ;
		if (ls->entry->d_name[0] != '.')
		{
			printf("%s ", ls->entry->d_name);
			ls->file_found = 1;
		}
	}
	if (ls->file_found)
		printf("\n");
}

/*void	ft_ls(char *input)
{
	char			**args;
	char			*path;
	DIR				*dir;
	int				file_found;
	struct dirent	*entry;

	file_found = 0;
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
		printf("💔_(ಥ﹏ಥ)_💔: cannot access '%s': No such file or directory\n",
			args[1]);
		return ;
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] != '.')
		{
			printf("%s ", entry->d_name);
			file_found = 1;
		}
	}
	if (file_found)
		printf("\n");
	free_split(args);
	closedir(dir);
}*/
