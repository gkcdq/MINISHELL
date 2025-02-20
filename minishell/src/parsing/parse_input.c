#include "../../minishell.h"

char	*reconstruct_input(char **changed_args)
{
	t_reconstruct data;

	data.i = 0;
	data.total_len = 0;
	while (changed_args[data.i])
	{
		data.total_len += strlen(changed_args[data.i]) + 1;
		data.i++;
	}
	data.new_input = malloc(sizeof(char) * data.total_len);
	if (!data.new_input)
		return (NULL);
	data.current_pos = data.new_input;
	data.i = 0;
	while (changed_args[data.i])
	{
		data.len = strlen(changed_args[data.i]);
		memcpy(data.current_pos, changed_args[data.i], data.len);
		data.current_pos += data.len;
		if (changed_args[data.i + 1])
			*data.current_pos++ = ' ';
		data.i++;
	}
	*data.current_pos = '\0';
	return (data.new_input);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

int	count_extra_spaces_pipeline(char *input)
{
	int i;
	int count;
	
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

char *parse_input_pipeline(char *input)
{
	t_pipeline_parser p;

	p.i = 0;
	p.j = 0;
	p.extra_spaces = count_extra_spaces_pipeline(input);
	p.tmp = malloc(sizeof(char) * (strlen(input) + p.extra_spaces + 1));
	if (!p.tmp)
		return (NULL);
	while (input[p.i])
	{
		if (input[p.i] == '|')
		{
			if (p.j > 0 && p.tmp[p.j - 1] > 32)
				p.tmp[p.j++] = ' ';
			p.tmp[p.j++] = '|';
			if (input[p.i + 1] && input[p.i + 1] > 32)
				p.tmp[p.j++] = ' ';
		}
		else
			p.tmp[p.j++] = input[p.i];
		p.i++;
	}
	p.tmp[p.j] = '\0';
	return (p.tmp);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

char	**check_dollars(char *input, t_ee *ee)
{
	char	**args;
	char	**changed_args;

	args = ft_split(input, ' ');
	if (!args)
		return (NULL);
	changed_args = parse_dollars(args, ee);
	if (!changed_args)
		return (NULL);
	free_split(args);
	return (changed_args);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

char *remove_parentheses(char *input)
{
	t_parentheses_remover r;

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

//                   la meme que celle du dessu mais pas raccourci V

/*char *remove_parentheses(char *input)
{
	t_parentheses_remover r;

	init_remover(&r, input);
	if (!r.tmp)
		return (NULL);
	while (input[r.i])
	{
		if (input[r.i] == '(')
		{
			if (r.level > 0)
				r.tmp[r.j++] = input[r.i];
			r.level++;
		}
		else if (input[r.i] == ')')
		{
			r.level--;
			if (r.level > 0)
				r.tmp[r.j++] = input[r.i];
		}
		else
		{
			if (r.level > 0 || (r.level == 0 && input[r.i] != ')'))
				r.tmp[r.j++] = input[r.i];
		}
		r.i++;
	}
	r.tmp[r.j] = '\0';
	return (r.tmp);
}*/

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

char	*parse_exev_input(char *tmp_in)
{
	int i = 0;
	int j = 0;
	char *return_input;

	while (tmp_in && tmp_in[i])
	{
		if (tmp_in[i] == '>' || tmp_in[i] == '<')
			break;
		i++;
	}
	return_input = malloc(sizeof(char) * i + 1);
	while (j < i)
	{
		return_input[j] = tmp_in[j];
		j++;
	}
	return_input[j] = '\0';
	return (return_input);
}

char	*parse_input_exit(char *input, t_token *exit)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == ';' && input[i + 1] == ';')
			exit->token = 1;
		if (input[i] == ';' && (input[i - 1] != ' '))
		{
			input[i] = '\0';
			break ;
		}
		i++;
	}
	return (input);
}

/*char	*reconstruct_input(char **changed_args)
{
	int		i;
	size_t	total_len;
	char	*new_input;
	char	*current_pos;
	size_t	len;

	i = 0;
	total_len = 0;
	while (changed_args[i])
	{
		total_len += ft_strlen(changed_args[i]) + 1;
		i++;
	}
	new_input = malloc(sizeof(char) * total_len);
	if (!new_input)
		return (NULL);
	current_pos = new_input;
	i = 0;
	while (changed_args[i])
	{
		len = ft_strlen(changed_args[i]);
		ft_memcpy(current_pos, changed_args[i], len);
		current_pos += len;
		if (changed_args[i + 1])
			*current_pos++ = ' ';
		i++;
	}
	*current_pos = '\0';
	return (new_input);
}*/

/*char *parse_input_pipeline(char *input)
{
	int i = 0, j = 0, count = 0;
	char *tmp;

	if (!input)
		return (NULL);
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
	tmp = malloc(sizeof(char) * (i + count + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (j > 0 && tmp[j - 1] > 32)
				tmp[j++] = ' ';
			tmp[j++] = '|';
			if (input[i + 1] && input[i + 1] > 32)
				tmp[j++] = ' ';
		}
		else
			tmp[j++] = input[i];
		i++;
	}
	tmp[j] = '\0';
	return (tmp);
}*/

/*char *remove_parentheses(char *input) 
{
    int i = 0, j = 0, level = 0;
    char *tmp;
	
	tmp = malloc(sizeof(char) * (ft_strlen(input) + 1));
    if (!tmp)
        return NULL;
    while (input[i])  
	{
        if (input[i] == '(') 
		{
            if (level > 0)
                tmp[j++] = input[i];
            level++;
        } else if (input[i] == ')') 
		{
            level--;
            if (level > 0)
                tmp[j++] = input[i];
        } 
		else 
		{
            if (level > 0 || (level == 0 && input[i] != ')'))
                tmp[j++] = input[i];
        }
        i++;
    }
    tmp[j] = '\0';
    return tmp;
}*/
