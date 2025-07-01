/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:18:43 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/01 01:35:21 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void set_heredoc(t_exec *current, t_executor *cmd, int i)
{
	if (current && current->next)
	{
		if (!cmd->heredoc_delimiters)
		{
			cmd->heredoc_delimiters = malloc(sizeof(char *) * 10);
			if (!cmd->heredoc_delimiters)
				exit(1);
			for (int j = 0; j < 10; j++)
				cmd->heredoc_delimiters[j] = NULL;
		}
		if (cmd->heredoc_delimiters[i])
			free(cmd->heredoc_delimiters[i]);
		cmd->heredoc_delimiters[i] = ft_strdup(current->next->content);
	}
}

void set_redirect(t_exec *current, t_executor *cmd)
{
	char *filename;

	while (current && current->next && current->rank != 1 && current->rank != 4)
	{
		filename = ft_strdup(current->next->content);
		check_redirect_file(cmd, filename, current->rank);
		current = current->next->next;
		free(filename);
		if (cmd->error != NULL)
			return;
	}
}
void	open_and_redirect(const char *file, int flags, int fd_target)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		perror("minishell: open");
		exit(1);
	}
	dup2(fd, fd_target);
	close(fd);
}

void	redirect_handle(t_executor *node)
{
	if (node->infile)
		open_and_redirect(node->infile, O_RDONLY, STDIN_FILENO);
	if (node->outfile)
		open_and_redirect(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO);
	if (node->append)
		open_and_redirect(node->append, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO);
}

void free_heredoc_delimiters(t_executor *cmd)
{
	if (!cmd->heredoc_delimiters)
		return;
	for (int i = 0; i < 10; i++)
		free(cmd->heredoc_delimiters[i]);
	free(cmd->heredoc_delimiters);
	cmd->heredoc_delimiters = NULL;
}

// void redirect_handle(t_executor *node)
// {
// 	int fd;

// 	if (node->infile)
// 	{
// 		fd = open(node->infile, O_RDONLY);
// 		if (fd < 0)
// 		{
// 			perror("minishell: open infile");
// 			exit(1);
// 		}
// 		dup2(fd, STDIN_FILENO);
// 		close(fd);
// 	}
// 	if (node->outfile)
// 	{
// 		fd = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (fd < 0)
// 		{
// 			perror("minishell: open outfile");
// 			exit(1);
// 		}
// 		dup2(fd, STDOUT_FILENO);
// 		close(fd);
// 	}
// 	if (node->append)
// 	{
// 		fd = open(node->append, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		if (fd < 0)
// 		{
// 			perror("minishell: open append outfile");
// 			exit(1);
// 		}
// 		dup2(fd, STDOUT_FILENO);
// 		close(fd);
// 	}
// }


