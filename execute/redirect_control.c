/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_control.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 21:52:28 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/06 19:28:42 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	if (S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

void	set_error_and_exit(char **dst, char *msg)
{
	if (dst == NULL)
		return ;
	if (*dst)
		free(*dst);
	*dst = ft_strdup(msg);
	set_exit_status_code(1);
}

void	check_redirect_access_input(const char *filename, t_executor *cmd)
{
	if (is_directory(filename))
	{
		set_error_and_exit(&cmd->error, ": Is a directory");
		return ;
	}
	if (access(filename, F_OK) < 0)
	{
		set_error_and_exit(&cmd->error, ": No such file or directory");
		return ;
	}
	if (access(filename, R_OK) < 0)
	{
		set_error_and_exit(&cmd->error, ": Permission denied");
		return ;
	}
}

int	check_redirect_access(const char *filename, int rank, char **error,
		int flags)
{
	int	fd;

	if (is_directory(filename))
	{
		set_error_and_exit(error, ": Is a directory");
		return (-1);
	}
	if (access(filename, F_OK) == 0 && access(filename, W_OK) < 0)
	{
		set_error_and_exit(error, ": Permission denied");
		return (-1);
	}
	if (rank == 6)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (rank == 5)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		set_error_and_exit(error, ": Failed to open file");
		return (-1);
	}
	close(fd);
	return (0);
}

void	check_redirect_file(t_executor *cmd, char *filename, int rank)
{
	if (rank == 2)
	{
		check_redirect_access_input(filename, cmd);
		if (cmd->error)
			return ;
		free(cmd->infile);
		cmd->infile = ft_strdup(filename);
	}
	else if (rank == 6 || rank == 5)
	{
		if (check_redirect_access(filename, rank, &cmd->error, 0) < 0)
			return ;
		if (rank == 6)
		{
			free(cmd->outfile);
			cmd->outfile = ft_strdup(filename);
		}
		else
		{
			free(cmd->append);
			cmd->append = ft_strdup(filename);
		}
	}
}
