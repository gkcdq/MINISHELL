#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h" // Bibliothèque personnelle
# include <curses.h>      // tputs, tgoto, etc.
# include <dirent.h>      // opendir, readdir, closedir, etc.
# include <errno.h>       // errno, perror, etc.
# include <fcntl.h>
# include <readline/history.h>  // Historique des commandes
# include <readline/readline.h> // readline, add_history, etc.
# include <signal.h>            // signal, sigaction, kill, etc.
# include <sys/stat.h>          // stat, lstat, fstat
# include <sys/types.h>         // pid_t, etc.
# include <sys/wait.h>          // wait, waitpid, etc.
# include <termios.h>           // tcsetattr, tcgetattr

extern int	g_status;

typedef struct s_token
{
	int		token;
	int		found;
}			t_token;

typedef struct s_env_fonction
{
	char	*copy_pwd;
	char	*copy_oldpwd;
}			t_env;

typedef struct s_cd_fonction
{
	char	**args;
	char	*home;
}			t_cd;

typedef struct s_wc_fonction
{
	int		lines;
	int		words;
	int		chars;
	int		longest_line;
	int		current_line_length;
	int		bytes_read;
	int		flag_l;
	int		flag_w;
	int		flag_c;
	int		flag_m;
	int		flag_L;
	int		j;
}			t_wc;

typedef struct s_envp_copy
{
	int		minishell_check;
	char	**envp;
	int		change_confirmed;
	char	*copy_pwd;
	char	*copy_oldpwd;
	int		if_unset__pwd;
	int		if_unset__oldpwd;
	int		if_unset__shlvl;
	int		lock_path;
	int		path_is_not_able;
	char	**copy_export_env;
	char	*save_initial_path;
	int		check_and_validity;
	int		command_with_and;
	int		command_with_or;
	int		confirmed_command;
	int		signal;
	int		save_result;
}			t_ee;

typedef struct s_redirection
{
	int		command_fail;
}			t_redir;

// init_c
int			ft_strlonglen(char **s);
void		init_struct(t_ee *ee);

// ---------- BUILTINS ----------- //

// exit.c
void		ft_exit(char *input, t_ee *ee);
// pwd.c
void		ft_pwd(void);
// cd.c
void		ft_cd(char *input, t_ee *ee);
// ls.c
void		ft_ls(char *input);
// clear.c
void		ft_clear(char *input);
// env.c
void		ft_env(t_ee *ee);
// unset.c
void		ft_unset(char *input, t_ee *ee);
// export.c
void		ft_export(char *input, t_ee *ee);
// echo.c
void		ft_echo(char *input, t_ee *ee);
// wc.c
void		ft_wc(char *input);

// ------------------------------ //

// str_utils.c
int			ft_strcmp(char *s1, char *s2);
int			is_number(char *str);
void		free_split(char **array);
char		*skip_isspace_for_fonctions(char *input, t_token *token);
char		*ft_strcat(char *dest, const char *src);
int			check_string(char *input);
////////////////////////////////////////////////////

// token_utils.c
int			check_token_in_all_string(char *input, t_token *tok);
int			check_token(char *input, t_token *token);
int			token_found(char *input, t_token *tok);
int			check_after_token(char *input, int i);
///////////////////////////////////////////////////

// path_utils.c
char		*find_command_path(char *command);
void		you_shall_not_path(void);
int			calcul_check_path(char **check_path);
void		check_path_in_or_with_pipe(char *input, t_ee *ee);
char		*save_initial_path(t_ee *ee);
///////////////////////////////////////////////////

// loop.c
void		loop(char *tmp, t_ee *ee);
///////////////////////////////////////////////////

// parse_token.c
char		*cut_for_no_leaks_at_the_end(char *input);
int			check_the_end(char *input);
char		**check_dollars(char *input, t_ee *ee);
int			do_you_find_or_what(char *input);
int			find_parenthesis(char *input);
char		**parse_dollars(char **args, t_ee *ee);
///////////////////////////////////////////////////

int			check_string(char *input);
char		**copy_envp(char **envp);
void		check_variable_pwd(t_ee *ee);
void		check_variable_oldpwd(char **envp);
char		*ft_strjoin_cd(char *s1, char *s2);
int			ft_strcmpchar(char a, char b);

void		check_if_path_is_set(t_ee *ee, char **args);
int			execute_pipeline(char *input, t_ee *ee);
char		*reconstruct_input(char **changed_args);
char		**check_dollars(char *input, t_ee *ee);
int			interprete_commande(char *input, t_ee *ee);
int			find_pipe(char *input);
int			find_or(char *input);
void		handle_redirection(char *input, t_ee *ee);
int			find_redirection(char *input);
char		*handle_redirection_with_pipe(char *input, t_ee *ee,
				int *heredoc_fd, int *input_fd, int *output_fd);
int			execute_pipeline_heredoc(char *input, t_ee *ee);
int			cumulate_token(char *input, t_ee *ee);
char		*copy_before_or(char *src);
char		*copy_after_or(char *src);
void		check_path_in_or_with_pipe(char *input, t_ee *ee);
int			check_after_or(char *input);
char		*handle_quotes(char *input, t_ee *ee);
void		handle_sigint(int sig);
void		catch_signal(t_ee *ee);

#endif
