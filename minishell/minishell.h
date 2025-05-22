#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_token	t_token;
typedef struct s_env	t_env;
typedef struct s_main	t_main;
typedef struct s_exec	t_exec;

typedef struct s_token
{
	char				*content;
	int					rank;
	int					flag;
	int					space;
	int					dollar;
	int					tick;
	t_main				*program;

	struct s_token		*next;

}						t_token;

typedef struct s_env
{
	char				*full_str;
	char				*before_eq;
	char				*after_eq;
	struct s_env		*next;
	t_main				*program;

}						t_env;

typedef struct s_main
{
	char				**env_str;
	t_token				*token;
	t_env				*env;
	t_exec				*exec;

}						t_main;

typedef struct s_executor
{
	char				**argv;
	char				*outfile;
	char				*infile;
	char				**heredoc;
	char				*append_outfile;
	int 				pipe;
	struct s_executor	*next;
	t_main				*program;
}						t_executor;

typedef struct s_exec
{
	char				*content;
	int					rank;
	int					tick;
	int					space;
	int 				pipe;
	t_executor			**executer;
	struct s_exec		*next;
	t_main				*program;

}						t_exec;



// utils
char					**ft_split(char *s, char c);
void					ft_lstadd_back(t_token **lst, t_token *new);
t_token					*ft_lstlast(t_token *lst);
t_token					*ft_lstnew(void *content);
int						ft_our_strchr(char *s, int c);
char					*ft_strchr(const char *s, int c);
char					*ft_strtrim(char *s1, char *set);
size_t					ft_strlen(char *s);
char					*ft_substr(char *s, unsigned int start, size_t len);
char					*ft_strjoin(char *s1, char *s2);
char					*my_join(char *line, char *s1, char *s2);
size_t					ft_strlcpy(char *dest, char *src, size_t destsize);
char					*ft_strdup(char *s1);
int						ft_isalnum(int c);
int						ft_strcmp(char *s1, char *s2);
int						ft_strstr(char *str, char *to_find);
void					ft_lstadd(t_token *node, t_token *new);
int						of_strchr(const char *s, int c);
t_env					*a_lstnew(char *before, char *after);
void					ft_envadd_back(t_env **lst, t_env *new);
t_exec					*ft_lstnew_exec(void *content);
void					ft_execadd_back(t_exec **lst, t_exec *new);
size_t					count_word(char *p, char c);
size_t					check(char **list, size_t count);
void					exec_init(t_main *program);
int						space_control(const char *s);

// signal
void					signal_init(void);
void					signal_handler(int signal);

// quotes
char					*empty_quotes(char *line);
int						find_quotes(char *line, int *ptr_i, int *ptr_j);
void					tokenize_args(char *line, t_token **token);

// init
void					token_init(t_main *program);
void					ft_init(t_main *program, char **env);

// dollar
void					dollar_control(t_token *token);
char					*find_dollar(char *content, int index);
char					*dollar_handle(char *dollar);

// control
int						pipe_control(t_token *token);
int						set_rank(t_token *token);
int						quote_control(char *line);

void					find_keys(t_token **token);
void					print_token(t_token *list);

// env
void					set_env(t_main *program, t_env *env);
void					get_env(t_env **envp, char **env);
void					print_env_array(t_main *program);

// free
void					free_program(t_main *program);
void					main_free(t_main program, char *line, int key);

void					ft_builtin(t_main *program);
void					setting_str(t_main *program);
void					setting_sign(t_main *program);

// exec

void					pipe_count(t_exec *node);
void					prep_exec(t_main *program);
void set_argv(t_executor **node, t_exec *temp, int i);


#endif
