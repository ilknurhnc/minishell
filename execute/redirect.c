/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:18:43 by hbayram           #+#    #+#             */
/*   Updated: 2025/06/23 19:03:21 by hbayram          ###   ########.fr       */
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

char *is_directory(const char *path)
{
	struct stat st;

	if (stat(path, &st) == -1)
		return NULL;
	if (S_ISDIR(st.st_mode))
		return ft_strdup(": Is a directory");
	return NULL;
}

void check_redirect_access_input(const char *filename, t_executor *cmd)
{
	char *err;

	err = is_directory(filename);
	if (err)
	{
		cmd->error = err;
		set_exit_status_code(1);
		return;
	}
	if (access(filename, F_OK) < 0)
	{
		cmd->error = ft_strdup(": No such file or directory");
		set_exit_status_code(1);
		return;
	}
	if (access(filename, R_OK) < 0)
	{
		cmd->error = ft_strdup(": Permission denied");
		set_exit_status_code(1);
		return;
	}
}


int check_redirect_access(const char *filename, int rank, char **error)
{
	int fd;
	int flags;
	char *err;

	err = is_directory(filename);
	if (err)
	{
		*error = err;
		return -1;
	}
	if (access(filename, F_OK) == 0 && access(filename, W_OK) < 0)
	{
		*error = ft_strdup(": Permission denied");
		return -1;
	}
	flags = 0;
	if (rank == 6)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (rank == 5)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		if (*error)
			return -1; //saçmalık
		*error = ft_strdup(": Failed to open file");
		return -1;
	}
	close(fd);
	return 0;
}

void check_redirect_file(t_executor *cmd, char *filename, int rank)
{
	if (rank == 2)
	{
		check_redirect_access_input(filename, cmd);
		if (cmd->error)
			return;
		free(cmd->infile);
		cmd->infile = ft_strdup(filename);
	}
	else if (rank == 6 || rank == 5)
	{
		if (check_redirect_access(filename, rank, &cmd->error) < 0)
			return;
		if (rank == 6)
		{
			free(cmd->outfile);
			cmd->outfile = ft_strdup(filename);
		}
		else if (rank == 5)
		{
			free(cmd->append);
			cmd->append = ft_strdup(filename);
		}
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

void redirect_handle(t_executor *node)
{
	int fd;

	if (node->infile)
	{
		fd = open(node->infile, O_RDONLY);
		if (fd < 0)
		{
			perror("minishell: open infile");
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (node->outfile)
	{
		fd = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("minishell: open outfile");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (node->append)
	{
		fd = open(node->append, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("minishell: open append outfile");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
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

