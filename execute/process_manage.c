/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_manage.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 03:58:57 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/06 16:00:51 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	manage_io(t_executor *current, int *pipefds, int output_fd, int prev_fd)
{
	if (current->next)
		close(pipefds[0]);
	if (current->heredoc_file != -1)
	{
		dup2(current->heredoc_file, STDIN_FILENO);
		close(current->heredoc_file);
	}
	else if (prev_fd != STDIN_FILENO)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	redirect_handle(current);
}

void	child_process(t_executor *current, int *pipefds, int output_fd,
		int prev_fd)
{
	int	code;

	manage_io(current, pipefds, output_fd, prev_fd);
	if (current->error || !current->argv[0])
	{
		if (current->error)
		{
			write(2, "minishell: ", 11);
			write(2, current->error, ft_strlen(current->error));
			write(2, "\n", 1);
		}
		set_exit_status_code(1);
		free_resources(current->program);
		exit(1);
	}
	if (is_builtin_command(current->argv[0]))
	{
		code = execute_builtin(current);
		free_resources(current->program);
		set_exit_status_code(1);
		exit(code);
	}
	else
		run_execve(current, STDIN_FILENO, STDOUT_FILENO);
}

void	ft_fork(t_executor *current, int *pipefds, int output_fd, int prev_fd)
{
	pid_t	pid;

	if (!current->error && is_builtin_command(current->argv[0])
		&& !current->next && prev_fd == STDIN_FILENO
		&& output_fd == STDOUT_FILENO)
		single_built_in(current);
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed.");
			free_resources(current->program);
			exit(1);
		}
		if (pid == 0)
			child_process(current, pipefds, output_fd, prev_fd);
	}
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
	if (current->heredoc_file != -1)
		close(current->heredoc_file);
	if (current->next)
		close(pipefds[1]);
}

void	set_fork(t_executor *current, int *pipefds, int *output_fd, int prev_fd)
{
	if (current->next)
	{
		if (pipe(pipefds) == -1)
		{
			perror("pipe failed");
			free_resources(current->program);
			exit(1);
		}
		*output_fd = pipefds[1];
	}
	ft_fork(current, pipefds, *output_fd, prev_fd);
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
}

void	wait_child(pid_t *last_pid, int *last_status)
{
	pid_t	pid;
	int		status;

	pid = wait(&status);
	while (pid > 0)
	{
		if (WIFEXITED(status))
			*last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*last_status = 128 + WTERMSIG(status);
		*last_pid = pid;
		pid = wait(&status);
	}
}
