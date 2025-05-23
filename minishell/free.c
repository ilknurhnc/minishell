/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:26:05 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/23 17:27:08 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_main *program)
{
	t_env	*node;
	t_env *temp;
	int i;

	i = 0;
	node = program->env;
	while (node)
	{
		temp = node;
		node = node->next;
		free(temp->before_eq);
		free(temp->after_eq);
		free(temp->full_str);
		free(temp);
	}
	program->token = NULL;
	program->env = NULL;

	if (program->env_str)
	{
		i = 0;
		while (program->env_str[i])
			free(program->env_str[i++]);
		free(program->env_str);
		program->env_str = NULL;
	}
}

void free_exec(t_main *program)
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

void	free_executer(t_main *program)
{
	int i = 0;

	if (!program->executer)
		return;
	while (program->executer[i])
	{	
		if (program->executer[i]->argv)
		{
			int j = 0;
			while (program->executer[i]->argv[j] != NULL)
			{
				free(program->executer[i]->argv[j]);
				program->executer[i]->argv[j] = NULL;
				j++;
			}
			free(program->executer[i]->argv);
			program->executer[i]->argv = NULL;
		}
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

		if (program->executer[i]->heredoc_delimiters)
		{
			int k = 0;
			while (program->executer[i]->heredoc_delimiters[k] != NULL)
			{
				free(program->executer[i]->heredoc_delimiters[k]);
				program->executer[i]->heredoc_delimiters[k] = NULL;
				k++;
			}
			free(program->executer[i]->heredoc_delimiters);
			program->executer[i]->heredoc_delimiters = NULL;
		}
		free(program->executer[i]);
		program->executer[i] = NULL;
		i++;
	}
	free(program->executer);
	program->executer = NULL;
}

void	free_program(t_main *program)
{
	t_token	*node;
	t_token	*temp;
	
	node = program->token;
	while (node)
	{
		temp = node;
		node = node->next;
		free(temp->content);
		free(temp);
	}
	program->token = NULL;
	free_env(program);
	free_exec(program);
	free_executer(program);
}

void	main_free(t_main program, char *line, int key)
{
	if (line)
		free(line);
	free_program(&program);
	if (key == 1)
		rl_clear_history();	
}
