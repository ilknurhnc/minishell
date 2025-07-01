/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:16:00 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/01 04:25:01 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_exec(t_main *program)
{
	t_exec	*node;
	t_exec	*temp;

	node = program->exec;
	while (node)
	{
		temp = node;
		node = node->next;
		free(temp->content);
		free(temp);
	}
	program->token = NULL;
}

void	free_executer_argv(t_main *program, int i)
{
	int	j;

	j = 0;
	if (program->executer[i]->argv)
	{
		while (program->executer[i]->argv[j])
		{
			free(program->executer[i]->argv[j]);
			program->executer[i]->argv[j] = NULL;
			j++;
		}
		free(program->executer[i]->argv);
		program->executer[i]->argv = NULL;
	}
}

void	free_executer_files(t_main *program, int i)
{
	if (program->executer[i]->infile)
	{
		free(program->executer[i]->infile);
		program->executer[i]->infile = NULL;
	}
	if (program->executer[i]->outfile)
	{
		free(program->executer[i]->outfile);
		program->executer[i]->outfile = NULL;
	}
	if (program->executer[i]->append)
	{
		free(program->executer[i]->append);
		program->executer[i]->append = NULL;
	}
	if (program->executer[i]->error)
	{
		free(program->executer[i]->error);
		program->executer[i]->error = NULL;
	}
}

void	free_executer_heredoc(t_main *program, int i)
{
	int	k;

	k = 0;
	if (program->executer[i]->heredoc_delimiters)
	{
		while (program->executer[i]->heredoc_delimiters[k])
		{
			free(program->executer[i]->heredoc_delimiters[k]);
			program->executer[i]->heredoc_delimiters[k] = NULL;
			k++;
		}
		free(program->executer[i]->heredoc_delimiters);
		program->executer[i]->heredoc_delimiters = NULL;
	}
}

void	free_executer(t_main *program)
{
	int	i;

	i = 0;
	if (!program || !program->executer)
		return ;
	while (program->executer[i])
	{
		free_executer_argv(program, i);
		free_executer_files(program, i);
		free_executer_heredoc(program, i);
		free(program->executer[i]);
		program->executer[i] = NULL;
		i++;
	}
	free(program->executer);
	program->executer = NULL;
}
