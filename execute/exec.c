/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:10:48 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/06 16:34:00 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error_and_exit(t_main *program, char *cmd, char *msg, int code)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, msg, ft_strlen(msg));
	free_resources(program);
	exit(code);
}

void	single_built_in(t_executor *current)
{
	int	saved_stdin;
	int	saved_stdout;
	int	builtin_status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	redirect_handle(current);
	if (current->heredoc_file != -1)
	{
		dup2(current->heredoc_file, STDIN_FILENO);
		close(current->heredoc_file);
	}
	builtin_status = execute_builtin(current);
	set_exit_status_code(builtin_status);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}

void	is_executable(char *cmd, t_main *program)
{
	struct stat	path_stat;

	if (ft_strchr(cmd, '/') != NULL)
	{
		if (stat(cmd, &path_stat) == -1)
			print_error_and_exit(program, cmd,
				": No such file or directory\n", 127);
		if (S_ISDIR(path_stat.st_mode))
			print_error_and_exit(program, cmd,
				": Is a directory\n", 126);
		if (access(cmd, X_OK) < 0)
			print_error_and_exit(program, cmd,
				": Permission denied\n", 126);
	}
}

void	run_execve(t_executor *node, int input_fd, int output_fd)
{
	char	*cmd_path;

	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	is_executable(node->argv[0], node->program);
	cmd_path = find_command_path(node->program, node->argv[0]);
	if (!cmd_path)
		print_error_and_exit(node->program, node->argv[0],
			" : command not found\n", 127);
	execve(cmd_path, node->argv, node->program->env_str);
	perror("execve failed");
	free_resources(node->program);
	free(cmd_path);
	exit(1);
}

void	main_execute(t_executor *exec, int prev_fd)
{
	int			pipefds[2];
	int			output_fd;
	t_executor	*current;
	pid_t		last_pid;
	int			last_status;

	last_pid = -1;
	last_status = 0;
	current = exec;
	g_signal_exit = 5;
	while (current)
	{
		output_fd = STDOUT_FILENO;
		if (current->heredoc_delimiters && current->heredoc_delimiters[0])
			handle_heredoc(current, current->program);
		set_fork(current, pipefds, &output_fd, prev_fd);
		if (current->next)
			prev_fd = pipefds[0];
		else
			prev_fd = STDIN_FILENO;
		current = current->next;
	}
	wait_child(&last_pid, &last_status);
	if (last_pid != -1)
		set_exit_status_code(last_status);
}
