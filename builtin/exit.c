/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 13:54:45 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/04 19:10:19 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	*get_exit_status_code(void)
{
	static int	exit_status = 0;

	return (&exit_status);
}

void	set_exit_status_code(int status)
{
	*get_exit_status_code() = status;
}

int	is_numeric(char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

void	exit_helper(t_executor *cmd, int *exit_code)
{
	int	num;

	if (is_numeric(cmd->argv[1]))
	{
		num = ft_atoi(cmd->argv[1]);
		*exit_code = (unsigned char)num;
		set_exit_status_code(*exit_code);
	}
	else
	{
		write(2, "minishell: exit: ", 18);
		write(2, cmd->argv[1], ft_strlen(cmd->argv[1]));
		write(2, ": numeric argument required\n", 29);
		set_exit_status_code(2);
		free_resources(cmd->program);
		exit(2);
	}
}

int	ft_exit(t_executor *cmd)
{
	int	argc;
	int	exit_code;

	argc = 0;
	exit_code = 0;
	while (cmd->argv[argc])
		argc++;
	printf("exit\n");
	if (argc == 1)
		exit_code = *get_exit_status_code();
	else if (argc == 2 || !is_numeric(cmd->argv[1]))
		exit_helper(cmd, &exit_code);
	else
	{
		write(2, "minishell: too many arguments\n", 31);
		set_exit_status_code(1);
		return (1);
	}
	free_resources(cmd->program);
	exit(exit_code);
	return (0);
}
