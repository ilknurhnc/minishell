/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:18:43 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/03 18:45:44 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_heredoc(t_exec *current, t_executor *cmd, int i)
{
	int	j;

	j = 0;
	if (current && current->next)
	{
		if (!cmd->heredoc_delimiters)
		{
			cmd->heredoc_delimiters = malloc(sizeof(char *) * 10);
			if (!cmd->heredoc_delimiters)
			{
				free_resources(current->program);
				exit(1);
			}
			while (j < 10)
			{
				cmd->heredoc_delimiters[j] = NULL;
				j++;
			}
		}
		if (cmd->heredoc_delimiters[i])
			free(cmd->heredoc_delimiters[i]);
		cmd->heredoc_delimiters[i] = ft_strdup(current->next->content);
	}
}

void	set_redirect(t_exec *current, t_executor *cmd)
{
	char	*filename;

	while (current && current->next && current->rank != 1 && current->rank != 4)
	{
		filename = ft_strdup(current->next->content);
		check_redirect_file(cmd, filename, current->rank);
		current = current->next->next;
		free(filename);
		if (cmd->error != NULL)
			return ;
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
		open_and_redirect(node->outfile, O_WRONLY | O_CREAT | O_TRUNC,
			STDOUT_FILENO);
	if (node->append)
		open_and_redirect(node->append, O_WRONLY | O_CREAT | O_APPEND,
			STDOUT_FILENO);
}

void	free_heredoc_delimiters(t_executor *cmd)
{
	int	i;

	if (!cmd->heredoc_delimiters)
		return ;
	i = 0;
	while (i < 10)
	{
		free(cmd->heredoc_delimiters[i]);
		i++;
	}
	free(cmd->heredoc_delimiters);
	cmd->heredoc_delimiters = NULL;
}
