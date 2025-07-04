/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:25:24 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/04 13:24:51 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	add_env(t_main *prog, char *key, char *value)
{
	t_env	*new;
	char	*tmp;

	new = a_lstnew(ft_strdup(key), ft_strdup(value));
	if (ft_strlen(value) == 0)
		new->control = 1;
	ft_envadd_back(&prog->env, new);
	tmp = ft_strjoin(ft_strdup(key), ft_strdup("="));
	free(new->full_str);
	new->full_str = ft_strjoin(tmp, ft_strdup(value));
}

void	update_or_add_env(t_main *prog, char *key, char *value)
{
	t_env	*tmp;
	char	*tmp1;
	char	*tmp2;

	tmp = prog->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->before_eq, key) == 0)
		{
			if (ft_strlen(value) == 0)
				return ;
			free(tmp->after_eq);
			tmp->after_eq = ft_strdup(value);
			free(tmp->full_str);
			tmp1 = ft_strjoin(ft_strdup(key), ft_strdup("="));
			tmp2 = ft_strjoin(tmp1, ft_strdup(value));
			tmp->full_str = tmp2;
			tmp->control = 0;
			fill_array(prog, ft_lstsize_env(prog->env));
			return ;
		}
		tmp = tmp->next;
	}
	add_env(prog, key, value);
}

void	add_empty_value(t_main *prog, char *key, char *value)
{
	t_env	*tmp;
	char	*tmp1;
	char	*tmp2;

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
			tmp->control = 0;
			fill_array(prog, ft_lstsize_env(prog->env));
			return ;
		}
		tmp = tmp->next;
	}
	add_env(prog, key, value);
}

int	handle_export(t_main *prog, t_executor *node, int i)
{
	char	*equal_pos;
	char	*key;
	char	*value;
	int		key_len;

	equal_pos = ft_strchr(node->argv[i], '=');
	if (equal_pos)
	{
		key_len = equal_pos - node->argv[i];
		key = ft_substr(node->argv[i], 0, key_len);
		value = ft_substr(equal_pos + 1, 0, strlen(equal_pos + 1));
		if (ft_strlen(value) == 0)
			add_empty_value(prog, key, value);
		update_or_add_env(prog, key, value);
		set_env(prog, prog->env);
		i++;
		free(value);
		free(key);
		return (1);
	}
	else
		update_or_add_env(prog, node->argv[i], "");
	return (0);
}

int	ft_export(t_executor *node)
{
	t_main	*prog;
	int		i;
	int		flag;

	flag = 0;
	prog = node->program;
	if (!node->argv[1])
		if (print_export_format(prog->env) == 0)
			return (0);
	i = 1;
	while (node->argv[i])
	{
		if (!is_valid_identifier(node->argv[i]))
		{
			print_export_error(node->argv[i++], &flag);
			continue ;
		}
		if (handle_export(prog, node, i) == 1)
		{
			i++;
			continue ;
		}
		i++;
	}
	return (flag);
}
