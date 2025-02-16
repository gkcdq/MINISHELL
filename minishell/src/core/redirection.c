#include "../../minishell.h"

int	find_redirection(char *input)
{
	int i = 0;

	while(input && input[i])
	{
		if (input[i] == '>' || input[i] == '<')
			return (1);
		i++;
	}
	return (0);
}

int find_type_of_redirection(char *tmp_in)
{
    int i = 0;

    while (tmp_in && tmp_in[i])
    {
        if (tmp_in[i] == '>' && tmp_in[i + 1] != '>')
            return (3);
        else if (tmp_in[i] == '>' && tmp_in[i + 1] == '>')
            return (2);
        
        else if (tmp_in[i] == '<' && tmp_in[i + 1] != '<')
            return (1);
        else if (tmp_in[i] == '<' && tmp_in[i + 1] == '<')
            return (4); 
        
        i++;
    }
    return (0);
}

char *unstick_to_re_stick(char *input)
{
    int i = 0, j = 0;
    int count_how_many_sep = 0;
    char *good_sep;

    while (input && input[i])
    {
        if ((input[i] > 32 && (input[i + 1] == '<' || input[i + 1] == '>')) ||
            ((input[i] == '<' || input[i] == '>') && input[i + 1] > 32))
        {
            count_how_many_sep++;
        }
        i++;
    }
    good_sep = malloc(sizeof(char) * (i + count_how_many_sep + 1));
    if (!good_sep)
        return NULL;
i = 0;
    while (input[i])
    {
		if (input[i - 1] > 32 && (input[i] == '<' || input[i] == '>'))
			good_sep[j++] = ' ';
        if (input[i] == '<' && input[i + 1] == '<')
        {
            good_sep[j++] = input[i];  
            good_sep[j++] = input[i+1]; 
            good_sep[j++] = ' ';      
            i++;
        }
        else if (input[i] == '>' && input[i + 1] == '>')
        {
            good_sep[j++] = input[i];  
            good_sep[j++] = input[i+1];
            good_sep[j++] = ' ';    
            i++;
        }
        else if (input[i] == '<' || input[i] == '>')
        {
            good_sep[j++] = input[i];
            good_sep[j++] = ' ';       
        }
        else
            good_sep[j++] = input[i];
        i++;
    }
    good_sep[j] = '\0';
    return good_sep;
}

int open_available(char **tmpfilename)
{
    int fd;
    char *tmp_dir = "/tmp/tmp_for_heredoc_";
    char *idxstr;
    char *final_filename;
    int i;

    i = 0;
    while (i < 65535)
    {
        idxstr = ft_itoa(i);
        final_filename = (char *)malloc(strlen(tmp_dir) + strlen(idxstr) + 1); 
        if (!final_filename)
        {
            free(idxstr);
            return (-1);
        }
        strcpy(final_filename, tmp_dir);
        ft_strcat(final_filename, idxstr);
        free(idxstr);
        fd = open(final_filename, O_WRONLY | O_CREAT | O_EXCL, 0644);
        if (fd >= 0)
        {
            *tmpfilename = final_filename;
            return fd;
        }
        free(final_filename);
        i++;
    }
    return (-1);
}

void handle_herdoc_sigint(int status)
{
	rl_replace_line("", 0);
    ft_putendl_fd("\n", 1);
    rl_on_new_line();
	unlink("/tmp/heredoc_tmp");
    exit(status);
}

int write_to_tmpfile(int fd, char *limit, t_ee *ee)
{
    char *input = NULL;
	(void)ee;

    signal(SIGINT, handle_herdoc_sigint);
    while (1)
    {
        input = readline("🗒️🖊️  ");
        if (!input || ft_strcmp(input, limit) == 0)
        {
            free(input);
            break;
        }
        ft_putendl_fd(input, fd);
        free(input);
    }
    return 0;
}

char *handle_redirection_with_pipe(char *input, t_ee *ee, int *heredoc_fd, int *input_fd, int *output_fd)
{
    char **split_in = NULL;
    char *input_execv = NULL;
    int last_name;
    char *path;
    char *final_command;
    t_redir *re = NULL;
    // Here-doc
    char *heredoc_tmpfile = NULL;
    int fd;
    char *delimiter = NULL;

    *heredoc_fd = -1;
    *input_fd = -1;
    *output_fd = -1;

    re = malloc(sizeof(t_redir));
    if (!re)
        return NULL;
    re->command_fail = 0;
    input_execv = parse_exev_input(input);
    split_in = ft_split(unstick_to_re_stick(input), ' ');
    if (!split_in || !split_in[0])
    {
        free_split(split_in);
        free(re);
        free(input_execv);
        return NULL;
    }
    last_name = ft_strlonglen(split_in);
    path = find_command_path(split_in[0]);
    for (int i = 1; i < last_name; i++)
    {
        if (strcmp(split_in[i], "<<") == 0)
        {
            fd = open_available(&heredoc_tmpfile);
            if (fd < 0)
            {
                perror("🔒 Erreur création fichier here-doc");
                return NULL;
            }

            delimiter = split_in[i + 1];
            int ret = write_to_tmpfile(fd, delimiter, ee);
            close(fd);
            if (ret != 0)
            {
                perror("🔒 Erreur écriture here-doc");
                unlink(heredoc_tmpfile);
                return NULL;
            }

            *heredoc_fd = open(heredoc_tmpfile, O_RDONLY);
            if (*heredoc_fd < 0)
            {
                perror("🔒 Erreur ouverture fichier here-doc");
                unlink(heredoc_tmpfile);
                return NULL;
            }
            i++;
        }
        else if (strcmp(split_in[i], "<") == 0)
        {
            *input_fd = open(split_in[i + 1], O_RDONLY);
            if (*input_fd < 0)
            {
                perror("🔒 Erreur ouverture fichier '<'");
                return NULL;
            }
            i++;
        }
        else if (strcmp(split_in[i], ">") == 0)
        {
            *output_fd = open(split_in[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (*output_fd < 0)
            {
                perror("🔒 Erreur ouverture fichier '>'");
                return NULL;
            }
            i++;
        }
        else if (strcmp(split_in[i], ">>") == 0)
        {
            *output_fd = open(split_in[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
            if (*output_fd < 0)
            {
                perror("🔒 Erreur ouverture fichier '>>'");
                return NULL;
            }
            i++;
        }
    }
    final_command = strdup(input_execv);
    if (!final_command)
    {
        perror("🔒 Erreur allocation mémoire");
        return NULL;
    }
    free_split(split_in);
    free(input_execv);
    free(path);
    free(re);
    return final_command;
}

void handle_redirection(char *input, t_ee *ee)
{
    char *tmp_in;
    char *if_need_sep;
    char **split_in = NULL;
    int last_name;
    int file = -1;
    pid_t pid;
    char *path;
    char *input_execv = NULL;
    char **split_execv = NULL;
    t_redir *re = NULL;

    // here-doc
    char *heredoc_tmpfile = NULL;  
    int heredoc_fd = -1;
    char *tmpfilenames[10];
    int heredoc_count = 0;
    int fd;
    int status;
    char *delimiter = NULL;

    re = malloc(sizeof(t_redir));
    if (!re)
        return;
    re->command_fail = 0;
    tmp_in = input;
    input_execv = parse_exev_input(tmp_in);
    split_execv = ft_split(input_execv, ' ');
    if (!split_execv || !split_execv[0]) 
    {
        free_split(split_execv);
        free(re);
        free(input_execv);
        return;
    }
    if_need_sep = unstick_to_re_stick(tmp_in);
    split_in = ft_split(if_need_sep, ' ');
    if (!split_in || !split_in[0]) 
    {
        free_split(split_in);
        free_split(split_execv);
        free(re);
        free(input_execv);
        return;
    }
    last_name = ft_strlonglen(split_in);
    path = find_command_path(split_in[0]);
    pid = fork();
    if (pid == -1)
    {
        free_split(split_in);
        free_split(split_execv);
        free(re);
        free(input_execv);
        return;
    }
    signal(SIGINT, SIG_IGN);
    if (pid == 0)
    {
        heredoc_fd = open_available(&heredoc_tmpfile);
        if (heredoc_fd < 0)
        {
            perror("🔒 Erreur création fichier temporaire here-doc final");
            exit(EXIT_FAILURE);
        }
        for (int i = 1; i < last_name; i++)
        {
            if (strcmp(split_in[i], "<<") == 0)
            {
                fd = open_available(&tmpfilenames[heredoc_count]);
                if (fd < 0)
                {
                    perror("🔒 Erreur création fichier here-doc");
                    exit(EXIT_FAILURE);
                }
                delimiter = split_in[i + 1];
                int ret = write_to_tmpfile(fd, delimiter, ee);
                close(fd);
                if (ret != 0) 
                {
                    perror("🔒 Erreur écriture here-doc");
                    unlink(tmpfilenames[heredoc_count]);
                    exit(EXIT_FAILURE);
                }
                fd = open(tmpfilenames[heredoc_count], O_RDONLY);
                if (fd < 0) 
                {
                    perror("🔒 Erreur ouverture fichier here-doc");
                    unlink(tmpfilenames[heredoc_count]);
                    exit(EXIT_FAILURE);
                }
                char buffer[1024];
                ssize_t bytes;
                while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
                {
                    write(heredoc_fd, buffer, bytes);
                }
                close(fd);
                unlink(tmpfilenames[heredoc_count]);
                heredoc_count++;
                i++;
            }
        }
        close(heredoc_fd);
        heredoc_fd = open(heredoc_tmpfile, O_RDONLY);
        if (heredoc_fd < 0) 
        {
            perror("🔒 Erreur ouverture fichier here-doc final");
            exit(EXIT_FAILURE);
        }
        dup2(heredoc_fd, STDIN_FILENO);
        close(heredoc_fd);
        unlink(heredoc_tmpfile);
        for (int i = 1; i < last_name; i++)
        {
            if (strcmp(split_in[i], ">") == 0)
            {
                file = open(split_in[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
                if (file == -1)
                {
                    perror("🔒 Erreur ouverture fichier '>'");
                    exit(EXIT_FAILURE);
                }
                dup2(file, STDOUT_FILENO);
                close(file);
                i++;
            }
            else if (strcmp(split_in[i], ">>") == 0)
            {
                file = open(split_in[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
                if (file == -1)
                {
                    perror("🔒 Erreur ouverture fichier '>>'");
                    exit(EXIT_FAILURE);
                }
                dup2(file, STDOUT_FILENO);
                close(file);
                i++;
            }
        }
        execv(path, split_execv);
        exit(EXIT_SUCCESS);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) 
        {
            if (!g_status) 
            {
                ee->signal = WEXITSTATUS(status);
                if (ee->signal != 0 && ee->signal != 1)
                    ee->signal = 128 + ee->signal;
            }
        }
    }
	if (!path)
	{
		ft_printf("🍁_(`へ´*)_🍁: %s: command not found\n", split_in[0]);
		ee->signal = 127;
		ee->confirmed_command = 0;
	}
	else
		ee->confirmed_command = 1;
    signal(SIGINT, handle_sigint);  
    free_split(split_execv);
    free_split(split_in);
    free(input_execv);
    free(if_need_sep);
    free(path);
    free(re);
}

char *foud_delimiter(char **split_in)
{
	char *tmp;
	int i = 0;
	while (split_in[i])
	{
		if (ft_strcmp(split_in[i], "<") == 0)
			break ;
		i++;
	}
	i++;
	i++;
	tmp = ft_strdup(split_in[i]);
	return (tmp);
}