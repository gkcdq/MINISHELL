#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

extern int	g_status;

typedef struct s_token
{
	int		token;
	int		found;
	char	*trimmed_input;
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

void		ft_exit(char *input, t_ee *ee);
void		ft_pwd(void);
void		ft_cd(char *input, t_ee *ee);
void		ft_ls(char *input);
void		ft_clear(char *input);
void		ft_env(t_ee *ee);
void		ft_unset(char *input, t_ee *ee);
void		ft_export(char *input, t_ee *ee);
void		ft_echo(char *input, t_ee *ee);
void		ft_wc(char *input);

// ------------------------------ //

// str_utils.c
int			ft_strcmp(char *s1, char *s2);
int			is_number(char *str);
void		free_split(char **array);
char		*skip_isspace_for_fonctions(char *input, t_token *token);
char		*ft_strcat(char *dest, const char *src);
int			check_string(char *input);

// token_utils.c
int			check_token_in_all_string(char *input, t_token *tok);
int			check_token(char *input, t_token *token);
int			token_found(char *input, t_token *tok);
int			check_after_token(char *input, int i);

// path_utils.c
char		*find_command_path(char *command);
void		you_shall_not_path(void);
int			calcul_check_path(char **check_path);
void		check_path_in_or_with_pipe(char *input, t_ee *ee);
char		*save_initial_path(t_ee *ee);
char		**copy_envp(char **envp);

// loop.c
void		loop(char *tmp, t_ee *ee);

// parse_token.c
char		*cut_for_no_leaks_at_the_end(char *input);
int			check_the_end(char *input);
int			do_you_find_or_what(char *input);
int			find_parenthesis(char *input);
char		**parse_dollars(char **args, t_ee *ee);

// parse_input.c
char		*reconstruct_input(char **changed_args);
char		*parse_input_pipeline(char *input);
char		**check_dollars(char *input, t_ee *ee);
char		*remove_parentheses(char *input);
char		*parse_exev_input(char *tmp_in);

// pipe.c
int			find_pipe(char *input);
int			execute_pipeline(char *input, t_ee *ee);
int			execute_pipeline_heredoc(char *input, t_ee *ee);

// redirection.c
int			find_redirection(char *input);
int			find_type_of_redirection(char *tmp_in);
char		*unstick_to_re_stick(char *input);
int			open_available(char **tmpfilename);
void		handle_herdoc_sigint(int status);
int			write_to_tmpfile(int fd, char *limit, t_ee *ee);
char		*handle_redirection_with_pipe(char *input, t_ee *ee,
				int *heredoc_fd, int *input_fd, int *output_fd);
void		handle_redirection(char *input, t_ee *ee);
char		*foud_delimiter(char **split_in);

// or.c
int			check_after_or(char *input);
int			find_or(char *input);
char		*copy_before_or(char *src);
char		*copy_after_or(char *src);

// command.c
char		*copy_pasta(char *input, int *i);
void		copy_until_parenthesis(char *l, int *i, char *copy, int *j);
int			process_segment(char *segment, t_ee *ee);
bool		process_token(char *input, int *i, t_ee *ee, bool success);
void		free_after_or(char *co, char *com, t_ee *ee);
void		if_confirmed_command_equal_to_zero(char *c, t_ee *ee);
void		after_find_or(char *input, t_ee *ee);
int			handle_or_and_redirection(char *input, t_ee *ee);
void		init_value_token(t_token *token, char *input);
void		at_the_end(t_token *token, t_ee *ee);
void		if_echo(char *input, t_ee *ee);
void		if_exit(t_token *token, char *input, t_ee *ee);
void		if_pwd(t_ee *ee);
void		if_cd(char *input, t_ee *ee);
void		if_env(t_ee *ee);
void		if_unset(char *input, t_ee *ee);
void		if_export(char *input, t_ee *ee);
void		if_dq(t_ee *ee, t_token *token);
void		if_else(t_token *token, char *input, t_ee *ee);
int			cumulate_token(char *input, t_ee *ee);
int			interprete_commande(char *input, t_ee *ee);
void		execute_external_command(char *command, t_ee *ee);

// parenthese.c
void		its_just_a_parenthese(char *input, t_ee *ee);

// quote_utilis.c
char		*find_env_var(const char *var_name, t_ee *ee);
char		*expand_variable(char *input, t_ee *ee);
char		*handle_quotes(char *input, t_ee *ee);

// signal.c
void		handle_sigint(int sig);
void		catch_signal(t_ee *ee);

////////////////////////////////////////////////////////////////////////

// cd
void		check_variable_pwd(t_ee *ee);
char		*ft_strjoin_cd(char *s1, char *s2);
int			ft_strcmpchar(char a, char b);
// export
void		check_if_path_is_set(t_ee *ee, char **args);
char		*ft_strcat_export(char *s1, char *s2);
void		sort_export(t_ee *ee, char **sorted);
char		**remove_duplicates_with_priority(char **env);
void		export_with_args(t_ee *ee, char **args);
char		**copi_colle(t_ee *ee);
void		handle_export_without_equals(char **args, char ***copy_export,
				int *len_export);
void		handle_env_with_equals(char **args, char ***copy_envp,
				int *len_envp);
int			ft_found_equal(char c);
char		*parse_input_simple_export(char *input);
int			ft_strcmp_dif(char *s1, char *s2);
char		*ft_strndup(const char *s, size_t n);
// env
void		check_variable_oldpwd(char **envp);

#endif
