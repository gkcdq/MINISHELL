/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmilin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:38:03 by tmilin            #+#    #+#             */
/*   Updated: 2025/02/27 19:38:04 by tmilin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int count_wordstt(const char *str, char sep)
{
    int i = 0;
    int count = 0;
    int in_quotes = 0;  // Variable pour vérifier si on est dans des guillemets

    while (str[i] != '\0')
    {
        if (str[i] == '"')  // On change l'état si on trouve un guillemet
            in_quotes = !in_quotes;
        
        while (!in_quotes && str[i] == sep && str[i] != '\0')  // On saute les séparateurs, sauf si on est dans des guillemets
            i++;
        
        if (!in_quotes && str[i] != sep && str[i] != '\0')  // Si on est hors des guillemets et qu'on trouve un mot
            count++;
        
        while (!in_quotes && str[i] != sep && str[i] != '\0')  // On saute les caractères du mot, sauf si on est dans des guillemets
            i++;
        
        if (in_quotes)  // Si on est dans les guillemets, on avance normalement
            i++;
    }
    return count;
}

static void ft_freetableautt(char **tab, int len)
{
    int i = 0;
    while (i < len)
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}

static void tableautt(char **tab, const char *str, char sep, int leng)
{
    int i = 0;
    int j = 0;
    int k;
    int m;
    int in_quotes = 0;  // Vérifie si on est dans des guillemets

    while (str[i] != '\0' && j < leng)
    {
        while (str[i] != '\0' && str[i] == sep && !in_quotes)  // Ne saute les séparateurs que si on est hors des guillemets
            i++;

        if (str[i] == '"')  // Si on trouve un guillemet, on entre ou sort des guillemets
        {
            in_quotes = !in_quotes;
            i++;
        }

        k = i;
        while (str[k] != '\0' && (str[k] != sep || in_quotes))  // On s'arrête à un séparateur, sauf si on est dans les guillemets
            k++;

        tab[j] = malloc(sizeof(char) * (k - i + 1));
        if (!tab)
            return (ft_freetableautt(tab, leng));
        
        m = 0;
        while (i < k)
            tab[j][m++] = str[i++];

        tab[j++][m] = '\0';
    }
    tab[j] = NULL;
}

char **ft_splittt(const char *st, char sep)
{
    char **dest;
    int leng;

    if (!st)
        return (NULL);

    leng = count_wordstt(st, sep);
    dest = malloc(sizeof(char *) * (leng + 1));
    if (!dest)
        return (NULL);

    tableautt(dest, st, sep, leng);
    return (dest);
}



//////////////////////////////////////////////////////////////////////
char *ft_stcatt(char c, const char *str)
{
    int len = strlen(str);
    char *new_str = malloc(len + 2);  // +1 pour le caractère, +1 pour le '\0'
    if (!new_str)
        return NULL;

    new_str[0] = c;  // Ajoute le guillemet au début
    strcpy(new_str + 1, str);  // Copie la chaîne existante après le guillemet
    return new_str;
}

///
int	f_q(char *input)
{
	int i = 0;

	while (input[i])
	{
		if (input[i] == '"')
			return (1);
		i++;
	}
	return (0);
} 
///
void for_quote_at_start(char ***args)
{
    int i = 0;
	int j = 0;

    while (args[i][j])
    {
        if (f_q(args[i][j]) == 1)
        {
            char *new_str = ft_stcatt('"', args[i][j]);
            free(args[i][j]); 
            args[i][j] = new_str;
        }
        j++;
    }
    return;
}
	

///
char	**check_dollars(char *input, t_ee *ee)
{
	char	**args;
	char	**changed_args;

	args = ft_splittt(input, ' ');
	if (!args)
		return (NULL);
	for_quote_at_start(&args);
	int i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "$OLDPWD") == 0)
		{
			free(args[i]);
			if (ee->copy_oldpwd)
				args[i] = ft_strdup(ee->copy_oldpwd);
			else
				args[i] = ft_strdup("");
			if (!args[i])
			{
				free_split(args);
				return (NULL);
			}
		}
		i++;
	}
	changed_args = parse_dollars(args, ee);
	if (!changed_args)
		return (NULL);
	free_split(args);
	return (changed_args);
}

char	*remove_parentheses(char *input)
{
	t_parentheses_remover	r;

	init_remover(&r, input);
	if (!r.tmp)
		return (NULL);
	while (input[r.i])
	{
		if (input[r.i] == '(' && r.level++ > 0)
			r.tmp[r.j++] = input[r.i];
		else if (input[r.i] == ')' && --r.level > 0)
			r.tmp[r.j++] = input[r.i];
		else if (r.level > 0 || input[r.i] != ')')
			r.tmp[r.j++] = input[r.i];
		r.i++;
	}
	r.tmp[r.j] = '\0';
	return (r.tmp);
}

int	count_extra_spaces_pipeline(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (i > 0 && input[i - 1] > 32)
				count++;
			if (input[i + 1] && input[i + 1] > 32)
				count++;
		}
		i++;
	}
	return (count);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	if (!dest || !src)
		return (0);
	i = 0;
	if (size > 0)
	{
		while (src[i] && i < size - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	if (!dest || !src)
		return (NULL);
	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
