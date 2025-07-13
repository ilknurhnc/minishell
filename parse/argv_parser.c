/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 01:45:18 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/08 14:08:17 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	design_argv(t_executor **node, t_exec *current, int i)
{
	int	h_count;

	h_count = 0;
	if (current && current->rank == 3)
	{
		set_heredoc(current, node[i], h_count);
		h_count++;
		if (!node[i]->heredoc_delimiters)
		{
			node[i]->heredoc_delimiters = malloc(sizeof(char *) * 100);
			if (!node[i]->heredoc_delimiters)
			{
				free_resources(current->program);
				exit(1);
			}
		}
		node[i]->heredoc_delimiters[h_count] = NULL;
		return (1);
	}
	if (current && current->rank != 4 && current->rank != 1)
	{
		set_redirect(current, node[i]);
		return (1);
	}
	return (0);
}

t_exec	*set_argv(t_executor **node, t_exec *start, int i)
{
	t_exec	*current;
	int		j;

	current = start;
	j = 0;
	node[i]->argv = malloc(sizeof(char *) * 100);
	if (!node[i]->argv)
		cannot_open(node[i]->program);
	ft_memset(node[i]->argv, 0, sizeof(char *) * 100);
	while (current && current->rank && current->rank != 1)
	{
		if (current->rank == 4 && current->content)
			node[i]->argv[j++] = ft_strdup(current->content);
		if (design_argv(node, current, i) == 1)
		{
			current = current->next->next;
			continue ;
		}
		current = current->next;
	}
	if (current && current->rank == 1)
		current = current->next;
	node[i]->argv[j] = NULL;
	return (current);
}

void	prep_exec(t_main *program)
{
	t_exec		*current;
	t_executor	**node;
	int			i;
	int			count;

	current = program->exec;
	count = 0;
	setting_node(program);
	pipe_count(program->exec);
	i = program->exec->pipe + 1;
	node = malloc(sizeof(t_executor *) * (i + 1));
	if (!node)
		cannot_open(program);
	while (count < i)
	{
		init_exec(program, node, count);
		current = set_argv(node, current, count);
		count++;
	}
	node[count] = NULL;
	program->executer = node;
	program->control = 1;
	main_execute(node[0], STDIN_FILENO);
}
