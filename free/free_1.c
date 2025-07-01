/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:15:45 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/01 04:18:01 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(t_main *program)
{
	t_env	*node;
	t_env	*temp;
	int		i;

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

void	free_token(t_main *program)
{
	t_token	*node;
	t_token	*temp;

	if (!program || !program->token)
		return ;
	node = program->token;
	while (node)
	{
		temp = node;
		node = node->next;
		if (temp->content)
		{
			free(temp->content);
			temp->content = NULL;
		}
		free(temp);
		temp = NULL;
	}
	program->token = NULL;
}

void	free_program(t_main *program, int key)
{
	free_token(program);
	if (key != 2)
		free_exec(program);
	else
		free(program->exec);
	if (key == 0 && program->control == 1)
		free_executer(program);
}

void	main_free(t_main program, char *line, int key)
{
	if (line)
		free(line);
	if (key == 1)
	{
		rl_clear_history();
		free_env(&program);
	}
	free_program(&program, key);
}

void	free_resources(t_main *program)
{
	free_token(program);
	free_exec(program);
	free_env(program);
	free_executer(program);
}
