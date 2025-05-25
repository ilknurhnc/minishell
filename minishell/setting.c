/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:25:08 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/25 19:16:05 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void setting_str(t_main *program)
{
	t_exec *new;
	t_token *node;
	char *str;
	char *add;

	node = program->token->next;
	while (node && node->tick == 1)
		node = node->next;
	add = ft_strjoin(ft_strdup(""), ft_strdup(""));
	while(node)
	{
		str = ft_strjoin(ft_strdup(add), ft_strdup(node->content));
		free(add);
		if(node->next && node->space == 1 && node->next->rank == 4)
			add = ft_strjoin(ft_strdup(str), ft_strdup(" "));
		else
			add = ft_strdup(str);
		free(str);
		
		node = node->next;
	}
	new = ft_lstnew_exec(ft_strdup(add));
	new->rank = 4;
	ft_execadd_back(&program->exec, new);
	free(add);
}

void setting_sign(t_main *program)
{
	t_exec *new;
	t_exec *new_node;
	t_token *node;
	char *str;
	char *add;
	int first;
	int new_flag =0;

	first = 0;
	node = program->token->next;
	while (node && node->tick == 1)
		node = node->next;
	add = ft_strjoin(ft_strdup(""), ft_strdup(""));
	while(node && node->rank == 4 && (node->space == 0 || new_flag == 1))
	{
		new_flag = 0;
		str = ft_strjoin(ft_strdup(add), ft_strdup(node->content));
		free(add);
		if(node->next && node->space == 1 && node->next->rank == 4)
			add = ft_strjoin(ft_strdup(str), ft_strdup(" "));
		else
			add = ft_strdup(str);
		free(str);
		node->tick = 1;
		node = node->next;
		if (node && node->space == 1)
			new_flag = 1;
		first = 1;
	}
	if(first == 1)
	{
		new = ft_lstnew_exec(ft_strdup(add));
		new->rank = 4;
		ft_execadd_back(&program->exec, new);
	}
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
	free(add);
	if(node != NULL)
		setting_sign(program);
}
