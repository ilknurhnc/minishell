/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 13:54:45 by hbayram           #+#    #+#             */
/*   Updated: 2025/06/26 21:04:50 by hbayram          ###   ########.fr       */
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

int is_numeric(char *str)
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

void exit_helper(t_executor *cmd, int *exit_code)
{
	if (is_numeric(cmd->argv[1]))
	{
		int num = ft_atoi(cmd->argv[1]);
		*exit_code = (unsigned char)num; // Bash uyumu için cast
		set_exit_status_code(*exit_code);
	}
	else
	{
		printf("minishell: exit: %s: numeric argument required\n", cmd->argv[1]);
		set_exit_status_code(255);
		free_resources(cmd->program);
		exit(255);
	}
}

int ft_exit(t_executor *cmd)
{
	int argc = 0;
	int exit_code = 0;

	while (cmd->argv[argc])
		argc++;
	printf("exit\n");

	if (argc == 1)
		exit_code = *get_exit_status_code();
	else if (argc == 2)
		exit_helper(cmd, &exit_code);
	else
	{
		printf("minishell: exit: too many arguments\n");
		set_exit_status_code(1);
		return (1);
	}

	free_resources(cmd->program);
	exit(exit_code);
	return (0);
}
