/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 13:54:45 by hbayram           #+#    #+#             */
/*   Updated: 2025/06/03 13:54:59 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_numeric(char *str)
{
	if (!str || *str == '\0')
		return (0);
	while (*str)
	{
		if (!isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

void	free_resources(t_main *program)
{
	free_env(program);
	free_token(program);
	free_exec(program);
	free_executer(program);
}

int	ft_exit(t_executor *cmd)
{
	int	argc;
	int	exit_code;

	argc = 0;
	exit_code = 0;
	while (cmd->argv[argc])
		argc++;
	printf("[MINISHELL EXIT]:\n");
	printf("exit\n");
	if (argc == 1)
		exit_code = cmd->exit_status;
	else if (argc == 2)
	{
		if (is_numeric(cmd->argv[1]))
			exit_code = ft_atoi(cmd->argv[1]) % 256;
		else
		{
			printf("minishell: exit: %s: numeric argument required\n",
				cmd->argv[1]);
			free_resources(cmd->program);
			exit(255);
		}
	}
	else
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	free_resources(cmd->program);
	exit(exit_code);
	return (0);
}
