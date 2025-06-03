/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:25:24 by ihancer           #+#    #+#             */
/*   Updated: 2025/06/03 12:55:07 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			set_env(prog, prog->env);
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