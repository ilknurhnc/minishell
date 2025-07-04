/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:39 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/04 15:32:36 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_rank(t_token *token)
{
	t_token	*node;

	node = token;
	node = node->next;
	while (node != NULL)
	{
		if (node->content != NULL && ft_strchr(node->content, '|')
			&& node->flag == -99)
			node->rank = 1;
		else if (node->content != NULL && ft_strcmp(node->content, "<") == 0)
			node->rank = 2;
		else if (node->content != NULL && ft_strcmp(node->content, "<<") == 0)
			node->rank = 3;
		else if (node->content != NULL && ft_strcmp(node->content, ">>") == 0)
			node->rank = 5;
		else if (node->content != NULL && ft_strcmp(node->content, ">") == 0)
			node->rank = 6;
		else
			node->rank = 4;
		node = node->next;
	}
	if (pipe_control(token) == 0)
		return (0);
	return (1);
}

int	pipe_control(t_token *token)
{
	t_token	*node;

	node = token->next;
	if (node != NULL && (node->rank == 1 || ft_lstlast(node)->rank != 4))
		return (printf("error, unexpected token\n"), 1);
	while (node != NULL && node->next != NULL)
	{
		if (node->next && node->next->rank != 4)
		{
			if ((node->rank != 4 && node->rank != 1)
				|| (node->next->next != NULL && node->next->next->rank != 4
					&& node->next->rank != 1))
				return (printf("error, unexpected token\n"), 1);
			if (node->rank == 1 && node->next && node->next->rank == 1)
				return (printf("error, near |\n"), 1);
		}
		node = node->next;
	}
	node = token->next;
	return (0);
}

int	quote_control(char *line)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == 34 && flag == 0)
			flag = 34;
		else if (line[i] == 39 && flag == 0)
			flag = 39;
		else if (line[i] == flag && flag != 0)
			flag = 0;
		i++;
	}
	if (flag != 0)
		return (printf("Error! Waiting for expression.\n"));
	return (0);
}
