/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 00:01:30 by ilknurhance       #+#    #+#             */
/*   Updated: 2025/06/04 02:40:54 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	is_valid_n_flag(char *str)
{
	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	for (int i = 2; str[i]; i++)
	{
		if (str[i] != 'n')
			return (0);
	}
	return (1);
}

int	ft_echo(t_executor *node)
{
	int	i;
	int	newline;

	if (!node || !node->argv)
		return (1);
	i = 1;
	newline = 1;
	printf("[MINISHELL ECHO]: ");
	while (node->argv[i] && is_valid_n_flag(node->argv[i]))
	{
		newline = 0;
		i++;
	}
	while (node->argv[i])
	{
		printf("%s", node->argv[i]);
		if (node->argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	ft_pwd(t_executor *node)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd");
		node->error = "Failed to get current directory";
		return (1);
	}
	printf("[MINISHELL PWD]:");
	printf("%s\n", cwd);
	free(cwd);
	node->error = NULL;
	return (0);
}

int	ft_env(t_executor *exec_node)
{
	t_main	*prog;
	t_env	*tmp;

	prog = exec_node->program;
	tmp = prog->env;
	while (tmp)
	{
		if (tmp->full_str != NULL && tmp->control == 0)
			printf("%s\n", tmp->full_str);
		tmp = tmp->next;
	}
	printf("[MINISHELL ENV]:\n");
	return (0);
}

int	execute_builtin(t_executor *cmd)
{
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(cmd));
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
	    return (ft_cd(cmd));
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_pwd(cmd));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_export(cmd));
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
	    return (ft_unset(cmd));
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(cmd));
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
	    return (ft_exit(cmd));
	return (1);
}
