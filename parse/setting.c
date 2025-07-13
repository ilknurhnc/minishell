/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:25:08 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/06 18:54:00 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	create_new_node(t_main *program, t_token *node)
{
	t_exec	*new_node;
	int		flag;

	flag = 0;
	if (node && node->space == 1)
	{
		new_node = ft_lstnew_exec(ft_strdup(node->content));
		new_node->rank = node->rank;
		new_node->space = node->space;
		ft_execadd_back(&program->exec, new_node);
		node->tick = 1;
		node = node->next;
	}
	if (node && node->rank != 4)
	{
		new_node = ft_lstnew_exec(ft_strdup(node->content));
		new_node->rank = node->rank;
		new_node->space = node->space;
		ft_execadd_back(&program->exec, new_node);
		node->tick = 1;
		node = node->next;
	}
	if (node != NULL)
		flag = 1;
	return (flag);
}

void	if_first(t_main *program, char *add, int first)
{
	t_exec	*new;

	if (first == 1)
	{
		new = ft_lstnew_exec(ft_strdup(add));
		new->rank = 4;
		ft_execadd_back(&program->exec, new);
	}
	free(add);
}

void	set_helper(t_token *node, char **add, int *flag)
{
	char	*str;

	str = ft_strjoin(ft_strdup(*add), ft_strdup(node->content));
	free(*add);
	node->tick = 1;
	if (node->next && node->space == 1 && node->next->rank == 4)
	{
		*add = ft_strdup(str);
		free(str);
		if (node->space == 1 && node->next)
			*flag = 1;
		return ;
	}
	else
		*add = ft_strdup(str);
	free(str);
	if (node->space == 1 && node->next)
		*flag = 1;
}

void	setting_node(t_main *program)
{
	t_token	*node;
	char	*add;
	int		first;
	int		new_flag;
	int		flag;

	flag = 0;
	new_flag = 0;
	first = 0;
	node = program->token->next;
	while (node && node->tick == 1)
		node = node->next;
	add = ft_strjoin(ft_strdup(""), ft_strdup(""));
	while (node && node->rank == 4 && (node->space == 0 || new_flag == 1)
		&& flag == 0)
	{
		set_helper(node, &add, &flag);
		node = node->next;
		if (node && node->space == 1)
			new_flag = 1;
		first = 1;
	}
	if_first(program, add, first);
	if (create_new_node(program, node) == 1)
		setting_node(program);
}
