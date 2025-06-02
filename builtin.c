/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 00:01:30 by ilknurhance       #+#    #+#             */
/*   Updated: 2025/06/02 18:10:26 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (tmp->full_str != NULL)
			printf("%s\n", tmp->full_str);
		tmp = tmp->next;
	}
	printf("[MINISHELL ENV]:\n");
	return (0);
}

int	ft_lstsize_env(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	print_export_format(t_env *env)
{
	while (env)
	{
		if (env->after_eq)
			printf("declare -x %s=\"%s\"\n", env->before_eq, env->after_eq);
		else
			printf("declare -x %s\n", env->before_eq);
		env = env->next;
	}
}


void	update_or_add_env(t_main *prog, char *key, char *value)
{
	t_env	*tmp;
	char	*tmp1;
	char	*tmp2;
	t_env	*new;

	tmp = prog->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->before_eq, key) == 0)
		{
			free(tmp->after_eq);
			tmp->after_eq = ft_strdup(value);
			free(tmp->full_str);
			tmp1 = ft_strjoin(ft_strdup(key), ft_strdup("="));
			tmp2 = ft_strjoin(tmp1, ft_strdup(value));
			tmp->full_str = tmp2;
			fill_array(prog, ft_lstsize_env(prog->env));
			return ;
		}
		tmp = tmp->next;
	}
	new = a_lstnew(ft_strdup(key), ft_strdup(value));
	ft_envadd_back(&prog->env, new);
	tmp1 = ft_strjoin(ft_strdup(key), ft_strdup("="));
	new->full_str = ft_strjoin(tmp1, ft_strdup(value));
	set_env(prog, prog->env);
}

int	ft_export(t_executor *node)
{
	t_main	*prog;
	char	*equal_pos;
	int		key_len;
	char	*key;
	char	*value;
	int		i;

	prog = node->program;
	if (!node->argv[1])
	{
		print_export_format(prog->env);
		return (0);
	}
	i = 1;
	while (node->argv[i])
	{
		if (!is_valid_identifier(node->argv[i]))
		{
			printf("minishell: export: '%s': not a valid identifier\n",
				node->argv[i]);
			i++;
			continue ;
		}
		equal_pos = ft_strchr(node->argv[i], '=');
		if (equal_pos)
		{
			key_len = equal_pos - node->argv[i];
			key = ft_substr(node->argv[i], 0, key_len);
			value = ft_substr(equal_pos + 1, 0, strlen(equal_pos + 1));
			update_or_add_env(prog, key, value);
			i++;
			continue ;
		}
		else
		{
			update_or_add_env(prog, node->argv[i], "");
			i++;
		}
	}
	return (0);
}

int	execute_builtin(t_executor *cmd)
{
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(cmd));
	// else if (ft_strcmp(cmd->argv[0], "cd") == 0)
	//     return (ft_cd(cmd));
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_pwd(cmd));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_export(cmd));
	// else if (ft_strcmp(cmd->argv[0], "unset") == 0)
	//     return (ft_unset(cmd));
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(cmd));
	// else if (ft_strcmp(cmd->argv[0], "exit") == 0)
	//     return (ft_exit(cmd));
	return (1);
}
