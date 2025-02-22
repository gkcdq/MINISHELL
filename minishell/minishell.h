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

extern int		g_status;

typedef struct s_token
{
	int			token;
	int			found;
	char		*trimmed_input;
}				t_token;

typedef struct s_env_fonction
{
	char		*copy_pwd;
	char		*copy_oldpwd;
}				t_env;

typedef struct s_cd_fonction
{
	char		**args;
	char		*home;
}				t_cd;

typedef struct s_quote
{
	int			single_quotes;
	int			double_quotes;
	char		*temp;
	char		*next_line;
}				t_quote;

typedef struct s_loop
{
	char		**changed_args;
	char		*cleaned_input;
	char		*input;
	pid_t		pid;
}				t_loop;

typedef struct s_pipe
{
	char		**commands;
	int			prev_fd;
	pid_t		pid;
	int			i;
}				t_pipe;

typedef struct s_pipeline
{
	char		**commands;
	int			pipe_fd[2];
	int			prev_fd;
	pid_t		pid;
	int			i;
	char		*final_command;
	int			heredoc_fd;
	int			input_fd;
	int			output_fd;
}				t_pipeline;

typedef struct s_reconstruct
{
	int			i;
	size_t		total_len;
	char		*new_input;
	char		*current_pos;
	size_t		len;
}				t_reconstruct;

typedef struct s_pipeline_parser
{
	int			i;
	int			j;
	int			extra_spaces;
	char		*tmp;
}				t_pipeline_parser;

typedef struct s_parentheses_remover
{
	int			i;
	int			j;
	int			level;
	char		*tmp;
}				t_parentheses_remover;

typedef struct s_dollar_parser
{
	char		*copy;
	char		**changed_args;
	char		*after_equal;
	char		*before_equal;
	int			lock;
	int			i;
	int			j;
	int			k;
	int			x;
	int			y;
	int			m;
}				t_dollar_parser;

typedef struct s_open
{
	char		*tmp_dir;
	char		*idxstr;
	char		*final_filename;
	int			i;
	int			fd;
}				t_open;

typedef struct s_quote_handler
{
	char		*result;
	int			size;
	int			i;
	int			j;
	char		quote;
	char		*expanded;
	int			expanded_len;
}				t_quote_handler;

typedef struct s_separator_handler
{
	int			i;
	int			j;
	int			count_how_many_sep;
	char		*good_sep;
}				t_separator_handler;

typedef struct s_redir
{
	int			command_fail;
}				t_redir;

typedef struct s_redir_handler
{
	char		**split_in;
	char		**split_execv;
	char		*tmp;
	int			last_name;
	int			file;
	pid_t		pid;
	char		*path;
	char		*input_execv;
	char		*heredoc_tmpfile;
	int			heredoc_fd;
	char		*delimiter;
	int			heredoc_count;
	char		*tmpfilenames[10];
	int			fd;
	int			status;
	char		buffer[1024];
	int			i;
	t_redir		*re;
}				t_redir_handler;

typedef struct s_redir_simple
{
	char		*input_execv;
	char		*if_need_sep;
	char		**split_in;
	char		**split_execv;
	char		*path;
	char		*heredoc_tmpfile;
	char		*tmpfilenames[10];
	char		*delimiter;
	int			last_name;
	int			file;
	int			heredoc_fd;
	int			heredoc_count;
	int			fd;
	int			status;
	pid_t		pid;
}				t_redir_simple;

typedef struct s_wc_fonction
{
	int			lines;
	int			words;
	int			chars;
	int			longest_line;
	int			current_line_length;
	int			bytes_read;
	int			flag_l;
	int			flag_w;
	int			flag_c;
	int			flag_m;
	int			flag_L;
	int			j;
}				t_wc;

typedef struct s_envp_copy
{
	int			minishell_check;
	char		**envp;
	int			change_confirmed;
	char		*copy_pwd;
	char		*copy_oldpwd;
	int			if_unset__pwd;
	int			if_unset__oldpwd;
	int			if_unset__shlvl;
	int			lock_path;
	int			path_is_not_able;
	char		**copy_export_env;
	char		*save_initial_path;
	int			check_and_validity;
	int			command_with_and;
	int			command_with_or;
	int			confirmed_command;
	int			signal;
	int			save_result;
}				t_ee;

// ---------- BUILTINS ----------- //

void			ft_exit(char *input, t_ee *ee);
void			ft_pwd(void);
void			ft_cd(char *input, t_ee *ee);
void			ft_ls(char *input);
void			ft_clear(char *input);
void			ft_env(t_ee *ee);
void			ft_unset(char *input, t_ee *ee);
void			ft_export(char *input, t_ee *ee);
void			ft_echo(char *input, t_ee *ee);
// void		ft_wc(char *input);

// ------------------------------ //

int				ft_strcmp(char *s1, char *s2);
int				is_number(char *str);
int				ft_strlonglen(char **s);
void			init_struct(t_ee *ee);
void			free_split(char **array);
char			*skip_isspace_for_fonctions(char *input, t_token *token);
char			*ft_strcat(char *dest, const char *src);
int				skip_whitespace(const char *tmp);
int				skip_whitespacee(const char *tmp, int i);
int				check_token(char *input, t_token *token);
int				check_after_token(char *input, int i);
char			*find_command_path(char *command);
void			you_shall_not_path(void);
int				calcul_check_path(char **check_path);
void			check_path_in_or_with_pipe(char *input, t_ee *ee);
char			*save_initial_path(t_ee *ee);
char			**copy_envp(char **envp);
void			init_struct_loop(t_loop *loop);
void			loop(char *tmp, t_ee *ee);
char			*cut_for_no_leaks_at_the_end(char *input);
int				check_the_end(char *input);
int				do_you_find_or_what(char *input);
int				find_parenthesis(char *input);
char			**parse_dollars(char **args, t_ee *ee);
char			*reconstruct_input(char **changed_args);
char			*parse_input_pipeline(char *input);
char			**check_dollars(char *input, t_ee *ee);
char			*remove_parentheses(char *input);
char			*parse_exev_input(char *tmp_in);
int				find_pipe(char *input);
int				execute_pipeline(char *input, t_ee *ee);
int				execute_pipeline_heredoc(char *input, t_ee *ee);
int				find_redirection(char *input);
int				find_type_of_redirection(char *tmp_in);
char			*unstick_to_re_stick(char *input);
int				open_available(char **tmpfilename);
void			handle_herdoc_sigint(int status);
int				write_to_tmpfile(int fd, char *limit, t_ee *ee);
void			handle_redirection(char *input, t_ee *ee);
char			*foud_delimiter(char **split_in);
int				check_after_or(char *input);
int				find_or(char *input);
char			*copy_before_or(char *src);
char			*copy_after_or(char *src);
char			*copy_pasta(char *l, int *i, t_ee *ee);
void			copy_until_parenthesis(char *l, int *i, char *copy, int *j);
int				process_segment(char *segment, t_ee *ee);
bool			process_token(char *input, int *i, t_ee *ee, bool success);
void			free_after_or(char *co, char *com, t_ee *ee);
char			*parse_command_after(char *command_after_or);
void			if_confirmed_command_equal_to_zero(char *c, t_ee *ee);
void			check_valid_after(char *command_after_or, t_ee *ee);
void			after_find_or(char *input, t_ee *ee);
int				handle_or_and_redirection(char *input, t_ee *ee);
void			init_value_token(t_token *token, char *input);
void			at_the_end(t_token *token, t_ee *ee);
void			if_echo(char *input, t_ee *ee);
void			if_exit(t_token *token, char *input, t_ee *ee);
void			if_pwd(t_ee *ee);
void			if_cd(char *input, t_ee *ee);
void			if_env(t_ee *ee);
void			if_unset(char *input, t_ee *ee);
void			if_export(char *input, t_ee *ee);
void			if_dq(t_ee *ee, t_token *token);
void			if_else(t_token *token, char *input, t_ee *ee);
int				cumulate_token(char *input, t_ee *ee);
int				interprete_commande(char *input, t_ee *ee);
void			execute_external_command(char *command, t_ee *ee);
void			init_quote_struct(t_quote *qt);
char			*append_next_line(char *input, t_ee *ee);
char			*handle_unclosed_quotes(char *input, t_ee *ee);
int				check_syntax_at_start(const char *tmp, t_ee *ee);
int				check_syntax_in_body(const char *tmp, t_ee *ee);
int				check_syntax_error(const char *tmp, t_ee *ee);
int				check_unexpected_semicolon(char *tmp, t_ee *ee);
int				find_trap(char *input);
void			cleanup_loop(t_loop *loop);
int				check_tmp_not_null(char *tmp, t_ee *ee);
void			for_norminette(t_ee *ee);
void			its_just_a_parenthese(char *input, t_ee *ee);
char			*find_env_var(const char *var_name, t_ee *ee);
char			*expand_variable(char *input, t_ee *ee);
char			*handle_quotes(char *input, t_ee *ee);
void			handle_sigint(int sig);
void			catch_signal(t_ee *ee);
void			check_variable_pwd(t_ee *ee);
char			*ft_strjoin_cd(char *s1, char *s2);
int				ft_strcmpchar(char a, char b);
void			check_if_path_is_set(t_ee *ee, char **args);
char			*ft_strcat_export(char *s1, char *s2);
void			sort_export(t_ee *ee, char **sorted);
char			**remove_duplicates_with_priority(char **env);
void			export_with_args(t_ee *ee, char **args);
char			**copi_colle(t_ee *ee);
void			handle_export_without_equals(char **a, char ***c, int *l);
void			handle_env_with_equals(char **a, char ***c, int *l);
int				ft_found_equal(char c);
char			*ft_strndup(const char *s, size_t n);
void			check_variable_oldpwd(char **envp);
int				find_pipe(char *input);
void			path_checker(char *input, t_ee *ee);
void			path_confirm(char *input);
void			init_pipe_struct(t_pipe *pipe);
char			*handle_redi_with_pipe(char *input, t_ee *ee, t_pipeline *p);
void			close_fds_and_update(t_pipeline *p);
void			close_fds(t_pipeline *p);
void			pid_sup_to_zero_pipe_redi(t_pipeline *p);
int				handle_error_piperedi(const char *m, t_pipeline *p, char *t);
int				handle_error(const char *msg, t_pipeline *p, char *input);
void			init_remover(t_parentheses_remover *r, char *input);
char			*parse_input_exit(char *input, t_token *exit);
char			*ft_strncpy(char *dest, const char *src, size_t n);
size_t			ft_strlcpy(char *dest, const char *src, size_t size);
int				count_extra_spaces(char *input);
char			*parse_redirection_at_start(char *input);
char			*cut_for_no_leaks_at_the_end(char *input);
int				count_first_or_last(char *input, char c);
char			*extract_var_name(char *arg);
char			*get_env_value(char *var_name, t_ee *ee);
char			*handle_dollar_variable(char *arg, t_ee *ee);
char			*extract_text_before_dollar(char *arg);
int				count_extra_spaces_pipeline(char *input);
void			norminette_tricks(void);
void			init_quote_handler(t_quote_handler *qh);
int				handle_expansion(t_quote_handler *qh, char *input, t_ee *ee);
void			expand_buffer_if_needed(t_quote_handler *qh);
void			all_is__freed(t_ee *ee);
int				count_separators(char *input);
char			*allocate_good_sep(char *input, int extra_spaces);
void			fill_good_sep(t_separator_handler *sh, char *input);
int				does_exit_final_filename(t_open openn);
void			init_redir_handler(t_redir_handler *handler, char *input);
void			redi_pipe_free(t_redir_handler *hr);
void			redi_last_free(t_redir_handler *hr);
void			init_p_fd(t_pipeline *p);
void			gandalf_le_grand_prince(t_redir_handler *hr, char c);
int				norminette_backflip(char *final_command, t_redir_handler *hr);
int				find_redirection_and_pipe(char *input);
void			free_if_simple_redir_fail(t_redir_simple *hr);
t_redir_simple	*init_redirr_handler(char *input);
void			handle_parent_process(t_redir_simple *hr, t_ee *ee);
void			handle_child_simple_process(t_redir_simple *hr, t_ee *ee);
int				fork_and_execute(t_redir_simple *hr, t_ee *ee);
void			open_output_file(t_redir_simple *hr, char *filename, int flags);
void			redirect_output(t_redir_simple *hr);
void			free_at_last(t_redir_simple *hr);
void			free_if_pid_is_n(t_redir_simple *hr);
int				init_heredoc(t_redir_simple *hr);
int				handle_heredoc(t_redir_simple *hr, t_ee *ee);
void			handle_otpt_redirection(t_redir_simple *hr);
int				finalize_heredoc(t_redir_simple *hr);
int				process_heredoc_entries(t_redir_simple *hr, t_ee *ee);
int				read_and_write_heredoc(t_redir_simple *hr, char *buffer);
char			*complete_tmp(char *src, int i);
int				check_for_no_double(char *input);

// voir (ls || || pwd) et check les status

#endif
