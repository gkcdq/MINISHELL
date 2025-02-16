#include "../../minishell.h"

void loop(char *tmp, t_ee *ee)
{
    t_token *tok;
    char **changed_args;
	char *cleaned_input;
	char *input = NULL;

	//// Pour les quotes 
    int single_quotes = 0;
    int double_quotes = 0;
    int i;
	char *temp;
	char *next_line;
	////

    changed_args = NULL;
    if ((!ee->envp || !ee->envp[0]) && ee->lock_path == 0)
        you_shall_not_path();
    tok = malloc(sizeof(t_token));
    tok->found = 0;
    tmp = readline("🍀_(^o^)_🍀  > ");
    if (tmp == NULL)
	{
        ee->minishell_check = 1;
		free(tmp);
		free(tok);
		return ;
	}
    input = cut_for_no_leaks_at_the_end(tmp);
	if (ft_strcmp(tmp, input) != 0)
		free(tmp);
	add_history(input);
	int check = check_the_end(input);
	if (check == 1)
	{
		free(input);
		free(tok);
		return ;
	}
	i = 0;
	while (input[i] && input[i] <= 32)
		i++;
	if (input[i] && input[i] == '&')
	{
		if (input[i + 1] == '&')
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `&&'\n");
		else
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `&'\n");
		ee->signal = 2;
		free(input);
    	free(tok);
    	return;
	}
	if (input[i] && input[i] == '|')
	{
		if (input[i + 1] == '|')
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `||'\n");
		else
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `|'\n");
		ee->signal = 2;
		free(input);
    	free(tok);
    	return;
	}
	if (input[i] && input[i] == ';')
	{
		if (input[i + 1] == ';')
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;;'\n");
		else
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;'\n");
		ee->signal = 2;
		free(input);
    	free(tok);
    	return;
	}
	i = 0;
	while (input[i])
	{
		if (input[i] == '&' && input[i + 1] == '&' && input[i + 2] == '&')
		{
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `&'\n");
			ee->signal = 2;
			free(input);
    		free(tok);
    		return;
		}
		if (input[i] == '|' && input[i + 1] == '|' && input[i + 2] == '|')
		{
			printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `|'\n");
			ee->signal = 2;
			free(input);
    		free(tok);
    		return;
		}
		i++;
	}
	if (g_status == 130)
    {
        ee->signal = 130;
        g_status = 0;
    }
	while (input && *input)
    {
        single_quotes = 0;
        double_quotes = 0;
		i = 0;
        while (input[i])
        {
            if (input[i] == '\'')
                single_quotes++;
            else if (input[i] == '"')
                double_quotes++;
			i++;
        }
        if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
        {
            next_line = readline("> ");
            if (next_line == NULL)
            {
                free(input);
                ee->minishell_check = 1;
                return;
            }
            temp = malloc(ft_strlen(input) + ft_strlen(next_line) + 1);
            if (!temp)
            {
                free(input);
                free(next_line);
                return;
            }
            strcpy(temp, input);
            strcat(temp, next_line);
			strcat(temp, "\n");
            free(input);
            free(next_line);
            input = temp;
        }
        else
		{
			break ;
		}
    }
    if (input && *input)
	{
        cleaned_input = handle_quotes(input, ee);
        free(input);
        input = cleaned_input;
        if (check_string(input) == 0) 
		{
            if (token_found(input, tok) == 1) 
			{
                if (check_token_in_all_string(input, tok) == 1) 
				{
                    if (tok->token == 2) 
					{
                        printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;'\n");
                        tok->token = 0;
                    } 
					else if (tok->token == 3)
					{
                        printf("🛠️_(>_<;)_🛠️   : syntax error near unexpected token `;;'\n");
					}
					ee->signal = 2;
                    free(input);
                    free(tok);
                    return;
                }
            }
			cumulate_token(input, ee);
        }
    }
    free_split(changed_args);
    free(input);
    free(tok);
}