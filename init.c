/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:16:34 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/25 19:44:09 by hbayram          ###   ########.fr       */
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
}

void	env_init(t_main *program, char **env)
{
	t_env envp;

	program->env = &envp;
	get_env(&program->env, env);
	set_env(program, program->env);
}

void exec_init(t_main *program)
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
}

void	ft_init(t_main *program, char **env)
{
	program->control = 0;
	token_init(program);
	env_init(program, env);
	exec_init(program);
}

