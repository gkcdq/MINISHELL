#include "../../minishell.h"

void	init_quote_struct(t_quote *qt)
{
	qt->single_quotes = 0;
	qt->double_quotes = 0;
	qt->temp = NULL;
	qt->next_line = NULL;
}

void count_quotes(const char *input, t_quote *qt)
{
    int i = 0;
    qt->single_quotes = 0;
    qt->double_quotes = 0;
    while (input[i])
    {
        if (input[i] == '\'')
            (qt->single_quotes)++;
        else if (input[i] == '"')
            (qt->double_quotes)++;
        i++;
    }
}

char *append_next_line(char *input, t_ee *ee)
{
    char *next_line;
    char *new_input;

	next_line = readline("> "); 
    if (!next_line)
    {
        free(input);
        ee->minishell_check = 1;
        return NULL;
    }
    new_input = malloc(ft_strlen(input) + ft_strlen(next_line) + 2);
    if (!new_input)
    {
        free(input);
        free(next_line);
        return NULL;
    }
    strcpy(new_input, input);
    strcat(new_input, next_line);
    strcat(new_input, "\n");
    free(input);
    free(next_line);
    return new_input;
}

char *handle_unclosed_quotes(char *input, t_ee *ee)
{
    t_quote qt;

    init_quote_struct(&qt);
    while (input && *input)
    {
        count_quotes(input, &qt);
        if (qt.single_quotes % 2 != 0 || qt.double_quotes % 2 != 0)
        {
            input = append_next_line(input, ee);
            if (!input)
                return NULL;
        }
        else
            break;
    }
    return input;
}
//unclosed_quote

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

int skip_whitespace(const char *tmp)
{
    int i;

	i = 0;
    while (tmp[i] && tmp[i] <= 32)
        i++;
    return i;
}

int check_syntax_at_start(const char *tmp, t_ee *ee)
{
    int i = skip_whitespace(tmp);
    
    if (tmp[i] == '&')
    {
        if ((tmp[i + 1] == '&' && tmp[i + 2] == '&') || (tmp[i] == '&' && tmp[i + 1] != '&'))
            printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `&'\n");
        else if (tmp[i + 1] == '&')
            printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `&&'\n");
        ee->signal = 2;
        return 1;
    }
    
    if (tmp[i] == '|')
    {
        if ((tmp[i + 1] == '|' && tmp[i + 2] == '|') || (tmp[i] == '|' && tmp[i + 1] != '|'))
            printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `|'\n");
        else if (tmp[i + 1] == '|')
            printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `||'\n");
        ee->signal = 2;
        return 1;
    }
    return 0;
}

int check_syntax_in_body(const char *tmp, t_ee *ee)
{
    int i = 0;
    while (tmp[i])
    {
        if (tmp[i] == '&' && tmp[i + 1] == '&' && tmp[i + 2] == '&')
        {
            printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `&'\n");
            ee->signal = 2;
            return 1;
        }
        if (tmp[i] == '|' && tmp[i + 1] == '|' && tmp[i + 2] == '|')
        {
            printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `|'\n");
            ee->signal = 2;
            return 1;
        }
        if (tmp[i] == ';' && tmp[i + 1] == ';')
        {
            printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;;'\n");
            ee->signal = 2;
            return 1;
        }
        i++;
    }
    return 0;
}

int check_syntax_error(const char *tmp, t_ee *ee)
{
    if (check_syntax_at_start(tmp, ee))
        return 1;
    return check_syntax_in_body(tmp, ee);
}

int skip_whitespacee(const char *tmp, int i)
{
    while (tmp[i] && tmp[i] <= 32)
        i++;
    return i;
}

int check_unexpected_semicolon(char *tmp, t_ee *ee)
{
    int i = skip_whitespacee(tmp, 0);

    if (tmp[i] == ';')
    {
        i++;
        i = skip_whitespacee(tmp, i);
        
        if (tmp[i] == '\0')
        {
            printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;'\n");
            ee->signal = 2;
            return 1;
        }
    }
    return 0;
}

int find_trap(char *input)
{
    int i = 0;

    if (!input)
        return (0);
    while (input[i] && input[i] != ';')
        i++;
    if (input[i] == ';')
    {
        i++;
        while (input[i] && input[i] <= 32)
            i++;
        if (input[i] == ';')
        {
            printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;'\n");
            return (1);
        }
    }
    return (0);
}


// (parsing)

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


int	check_tmp_not_null(char *tmp, t_ee *ee)
{
	if (tmp == NULL)
	{
		ee->minishell_check = 1;
		free(tmp);
		return (1);
	}
	return (0);
}

void cleanup_loop(t_loop *loop)
{
    if (loop)
    {
        if (loop->changed_args)
            free_split(loop->changed_args);
        if (loop->input)
            free(loop->input);
        free(loop);
    }
}

void loop(char *tmp, t_ee *ee)
{
    t_loop *loop;

    if ((!ee->envp || !ee->envp[0]) && ee->lock_path == 0)
        you_shall_not_path();

    loop = malloc(sizeof(t_loop));
    if (!loop)
        return;
    init_struct_loop(loop);

    tmp = readline("🍀_(^o^)_🍀  > ");
    if (check_tmp_not_null(tmp, ee))
    {
        free(loop);
        return;
    }
    
    add_history(tmp);

    if (check_syntax_error(tmp, ee) || check_unexpected_semicolon(tmp, ee))
    {
        free(tmp);
        cleanup_loop(loop);
        return;
    }

    loop->input = cut_for_no_leaks_at_the_end(tmp);
    if (!loop->input)
    {
        free(tmp);
        cleanup_loop(loop);
        return;
    }

    if (ft_strcmp(tmp, loop->input) != 0)
        free(tmp);

    if (check_the_end(loop->input) == 1)
    {
        cleanup_loop(loop);
        return;
    }

    if (g_status == 130)
    {
        ee->signal = 130;
        g_status = 0;
    }

    loop->input = handle_unclosed_quotes(loop->input, ee);
    if (!loop->input || !*loop->input)
    {
        cleanup_loop(loop);
        return;
    }

    loop->cleaned_input = handle_quotes(loop->input, ee);
    free(loop->input);
    loop->input = loop->cleaned_input;

    if (!loop->input)
    {
        cleanup_loop(loop);
        return;
    }

    if (find_trap(loop->input))
    {
        cleanup_loop(loop);
        return;
    }

    cumulate_token(loop->input, ee);
    cleanup_loop(loop);
}



