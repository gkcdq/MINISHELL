#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"       // Bibliothèque personnelle
# include <curses.h>            // tputs, tgoto, etc.
# include <dirent.h>            // opendir, readdir, closedir, etc.
# include <errno.h>             // errno, perror, etc.
# include <readline/history.h>  // Historique des commandes
# include <readline/readline.h> // readline, add_history, etc.
# include <signal.h>            // signal, sigaction, kill, etc.
# include <sys/stat.h>          // stat, lstat, fstat
# include <sys/types.h>         // pid_t, etc.
# include <sys/wait.h>          // wait, waitpid, etc.
# include <termios.h>           // tcsetattr, tcgetattr

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

typedef	struct s_envp_copy
{
	// signal exit
	int		minishell_check;
	// environnement
	char 	**envp;
	int		change_confirmed;
	char	*copy_pwd;
	char	*copy_oldpwd;
	char	*save_oldpwd;
	char 	*last_pwd;
}			t_ee;

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
void 		ft_unset(char *input, t_ee *ee);
// export.c
void		ft_export(char *input, t_ee *ee);

// wc.c
void		ft_wc(void);

// ------------------------------ //

// utils.c
int			ft_strcmp(char *s1, char *s2);
void		free_split(char **array);
int			is_number(char *str);
int			check_token(char *input, t_token *token);
int			check_string(char *input);
char 		**copy_envp(char **envp);
void		check_variable_pwd(t_ee *ee);
void 		check_variable_oldpwd(char **envp);
char		*ft_strjoin_cd(char *s1, char *s2);
int			ft_strcmpchar(char a, char b);

#endif