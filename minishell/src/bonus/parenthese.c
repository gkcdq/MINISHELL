#include "../../minishell.h"

void	its_just_a_parenthese(char *input, t_ee *ee)
{
    char *clean_input;
	char *command_before_or;
	char *command_after_or;
	bool has_parentheses = false;

	if (do_you_find_or_what(input) == 1)
	{
		command_before_or = copy_before_or(input);
		command_after_or = copy_after_or(input);
		has_parentheses = (find_parenthesis(command_after_or) == 1);
        cumulate_token(command_before_or, ee);
		if (ee->signal != 0)
		{
			if (has_parentheses)
			{
				pid_t pid = fork();
				if (pid == -1)
				{
					perror("Erreur fork()");
					free(command_before_or);
					free(command_after_or);
					return;
				}
				if (pid == 0)
				{
					its_just_a_parenthese(command_after_or, ee);
					exit(ee->signal);
				}
				else
				{
					int status;
					waitpid(pid, &status, 0);
					if (WIFEXITED(status))
						ee->signal = WEXITSTATUS(status);
				}
			}
			else
				cumulate_token(command_after_or, ee);
		}
		free(command_before_or);
		free(command_after_or);
		return;
	}
	clean_input = remove_parentheses(input);
	if (!clean_input)
		return;
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Erreur fork()");
        free(clean_input);
        return;
    }
    if (pid == 0)
    {
        cumulate_token(clean_input, ee);
        exit(ee->signal);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            ee->signal = WEXITSTATUS(status);
		if (ee->signal == 127)
			ee->check_and_validity = 1;
    }
    free(clean_input);
}