/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:51:26 by hbayram           #+#    #+#             */
/*   Updated: 2025/06/27 14:53:21 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	do_heredoc_write(char *delimiter, int write_fd, t_main *program)
{
	char	*line;

	(void)program;
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			close(write_fd);
			exit(130);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
}

void	heredoc_child(t_executor *cmd, t_main *program, int pipefd[2])
{
	static char	str[10000];
	int			i;

	i = -1;
	signal(SIGINT, signal_handler);
	while (cmd->heredoc_delimiters[0][++i])
		str[i] = cmd->heredoc_delimiters[0][i];
	g_signal_exit = 1;
	free_resources(program);
	close(pipefd[0]);
	do_heredoc_write(str, pipefd[1], program);
	close(pipefd[1]);
	exit(0);
}

void	heredoc_parent(t_executor *cmd, int pipefd[2], pid_t pid)
{
	int	status;

	g_signal_exit = 2;
	signal(SIGINT, signal_handler);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		set_exit_status_code(130);
		close(pipefd[0]);
		cmd->heredoc_file = -1;
	}
	else
		cmd->heredoc_file = pipefd[0];
	g_signal_exit = 0;
}

void	handle_heredoc(t_executor *cmd, t_main *program)
{
	int		pipefd[2];
	pid_t	pid;

	if (!cmd->heredoc_delimiters || !cmd->heredoc_delimiters[0])
		return ;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		free_resources(program);
		exit(1);
	}
	pid = fork();
	if (pid == 0)
		heredoc_child(cmd, program, pipefd);
	else if (pid < 0)
	{
		perror("fork failed.");
		free_resources(program);
		exit(1);
	}
	else
		heredoc_parent(cmd, pipefd, pid);
}

void	pipe_count(t_exec *node)
{
	t_exec	*new;
	int		i;

	new = node;
	i = 0;
	while (new)
	{
		if (new->rank == 1)
			i++;
		new = new->next;
	}
	node->pipe = i;
}
