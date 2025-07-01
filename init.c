/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:16:34 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/01 04:32:01 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_init(t_main *program)
{
	program->token = malloc(sizeof(t_token));
	if (!program->token)
	{
		perror("Malloc failed");
		exit(1);
	}
	program->token->content = NULL;
	program->token->rank = 0;
	program->token->dollar = 0;
	program->token->flag = 0;
	program->token->tick = 0;
	program->token->space = 0;
	program->token->next = NULL;
	program->token->program = program;
}

void	env_init(t_main *program, char **env)
{
	t_env	*envp;

	envp = NULL;
	get_env(&envp, env);
	program->env = envp;
	set_env(program, program->env);
}

void	exec_init(t_main *program)
{
	program->exec = malloc(sizeof(t_exec));
	if (!program->exec)
	{
		perror("Malloc failed");
		exit(1);
	}
	program->exec->content = NULL;
	program->exec->next = NULL;
	program->exec->rank = 4;
	program->exec->program = program;
}

void	ft_init(t_main *program)
{
	program->control = 0;
	program->exit_status = 0;
	token_init(program);
	exec_init(program);
}

void	init_exec(t_main *program, t_executor **node, int count)
{
	node[count] = malloc(sizeof(t_executor));
	if (!node[count])
	{
		while (--count >= 0)
			free(node[count]);
		free_executer(program);
		return ;
	}
	node[count]->infile = NULL;
	node[count]->outfile = NULL;
	node[count]->heredoc_file = -1;
	node[count]->append = NULL;
	node[count]->pipe = program->exec->pipe;
	node[count]->heredoc_delimiters = NULL;
	node[count]->program = program;
	node[count]->error = NULL;
	if (count > 0)
		node[count - 1]->next = node[count];
	node[count]->next = NULL;
}
