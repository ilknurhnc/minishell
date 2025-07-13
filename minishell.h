/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:53:09 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/08 14:09:11 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern int					g_signal_exit;

typedef struct s_token		t_token;
typedef struct s_env		t_env;
typedef struct s_main		t_main;
typedef struct s_exec		t_exec;
typedef struct s_executor	t_executor;

typedef struct s_token
{
	char					*content;
	int						rank;
	int						flag;
	int						space;
	int						dollar;
	int						tick;
	t_main					*program;

	struct s_token			*next;

}							t_token;

typedef struct s_env
{
	int						control;
	char					*full_str;
	char					*before_eq;
	char					*after_eq;
	struct s_env			*next;
	t_main					*program;

}							t_env;

typedef struct s_main
{
	int						control;
	int						exit_status;
	char					**env_str;
	t_token					*token;
	t_env					*env;
	t_exec					*exec;
	t_executor				**executer;

}							t_main;

typedef struct s_executor
{
	char					**argv;
	char					*outfile;
	char					*infile;
	int						heredoc_file;
	char					**heredoc_delimiters;
	char					*append;
	char					*error;
	int						pipe;
	struct s_executor		*next;
	t_main					*program;

}							t_executor;

typedef struct s_exec
{
	char					*content;
	int						rank;
	int						tick;
	int						space;
	int						pipe;
	t_executor				**executer;
	struct s_exec			*next;
	t_main					*program;

}							t_exec;

// utils
char						**ft_split(char *s, char c);
void						ft_lstadd_back(t_token **lst, t_token *new);
t_token						*ft_lstlast(t_token *lst);
t_token						*ft_lstnew(void *content);
int							ft_our_strchr(char *s, int c);
char						*ft_strchr(const char *s, int c);
char						*ft_strtrim(char *s1, char *set);
size_t						ft_strlen(char *s);
char						*ft_substr(char *s, unsigned int start, size_t len);
char						*ft_strjoin(char *s1, char *s2);
char						*my_join(char *line, char *s1, char *s2);
size_t						ft_strlcpy(char *dest, char *src, size_t destsize);
char						*ft_strdup(char *s1);
int							ft_isalnum(int c);
int							ft_isalpha(int c);
int							ft_strcmp(char *s1, char *s2);
int							ft_strstr(char *str, char *to_find);
void						ft_lstadd(t_token *node, t_token *new);
int							of_strchr(const char *s, int c);
t_env						*a_lstnew(char *before, char *after);
void						ft_envadd_back(t_env **lst, t_env *new);
t_exec						*ft_lstnew_exec(void *content);
void						ft_execadd_back(t_exec **lst, t_exec *new);
size_t						count_word(char *p, char c);
size_t						check(char **list, size_t count);
void						exec_init(t_main *program);
int							space_control(char *s);
int							ft_atoi(const char *str);
char						*ft_itoa(int num);
char						*ft_strcat(char *dest, const char *src);
char						*ft_strcpy(char *dest, char *src);
char						*ft_strncpy(char *dest, char *src, unsigned int n);
char						*ft_strnstr(const char *src, const char *to_find,
								size_t n);
int							ft_isdigit(int c);
void						*ft_memset(void *s, int c, size_t n);

// signal
void						signal_init(void);
void						signal_handler(int signal);
int							*get_exit_status_code(void);
void						set_exit_status_code(int status);

// parse
char						*empty_quotes(char *line);
int							find_quotes(char *line, int *ptr_i, int *ptr_j);
void						tokenize_args(char *line, t_token **token);
int							design_argv(t_executor **node, t_exec *current,
								int i);
void						prep_exec(t_main *program);
t_exec						*set_argv(t_executor **node, t_exec *start, int i);
char						*if_loop(char *line, int i);
void						find_keys(t_token **token);
char						*find_helper(char *line, int *ptr_i, int *ptr_j);
void						set_keys(t_token *temp);

// init
void						token_init(t_main *program);
void						ft_init(t_main *program);
void						init_exec(t_main *program, t_executor **node,
								int count);

// dollar
void						dollar_control(t_token *token);
char						*find_dollar(char *content, int index,
								t_main *program);
char						*dollar_handle(char *dollar, t_env *env);

// control
int							pipe_control(t_token *token);
int							set_rank(t_token *token);
int							quote_control(char *line);

void						find_keys(t_token **token);

// env
void						set_env(t_main *program, t_env *env);
void						get_env(t_env **envp, char **env);
void						fill_array(t_main *program, int i);
int							ft_lstsize_env(t_env *env);
void						update_or_add_env(t_main *prog, char *key,
								char *value);
char						*get_env_value(t_env *env, char *key);
int							ft_env(t_executor *exec_node);

// free
void						free_program(t_main *program, int key);
void						main_free(t_main program, char *line, int key);
void						free_env(t_main *program);
void						free_exec(t_main *program);
void						free_executer(t_main *program);
void						free_token(t_main *program);
void						setting_node(t_main *program);
void						free_executer(t_main *program);
void						free_resources(t_main *program);
int							cannot_open(t_main *program);

// exec
void						main_execute(t_executor *exec, int prev_fd);
void						set_fork(t_executor *current, int *pipefds,
								int *output_fd, int prev_fd);
void						wait_child(pid_t *last_pid, int *last_status);
void						run_execve(t_executor *node, int input_fd,
								int output_fd);
void						single_built_in(t_executor *current);

// redirect
void						set_heredoc(t_exec *current, t_executor *cmd,
								int i);
int							is_directory(const char *path);

void						check_redirect_access_input(const char *filename,
								t_executor *cmd);
int							check_redirect_access(const char *filename,
								int rank, char **error, int flags);

void						check_redirect_file(t_executor *cmd, char *filename,
								int rank);
void						set_redirect(t_exec *current, t_executor *cmd);
void						redirect_handle(t_executor *node);

// builtin
int							is_builtin_command(char *cmd);
int							execute_builtin(t_executor *cmd);
void						env_init(t_main *program, char **env);
int							ft_export(t_executor *node);
int							ft_unset(t_executor *node);
int							ft_cd(t_executor *node);
int							ft_exit(t_executor *cmd);
void						print_export_error(char *arg, int *flag);
int							is_valid_identifier(char *str);
int							print_export_format(t_env *env);
int							env_print_error(char *msg, int code);

// heredoc_utils
void						do_heredoc_write(char *delimiter, int write_fd,
								t_main *program);
void						heredoc_child(t_executor *cmd, t_main *program,
								int pipefd[2]);
void						heredoc_parent(t_executor *cmd, int pipefd[2],
								pid_t pid);
void						handle_heredoc(t_executor *cmd, t_main *program);
void						pipe_count(t_exec *node);

// path
char						*join_path(char *dir, const char *cmd);
char						*get_path_from_env(t_env *env);
char						*get_next_path_dir(char *path_str, int *start_pos);
char						*is_executable_path(char *command);
char						*find_command_path(t_main *program, char *command);

#endif
