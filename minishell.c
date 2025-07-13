/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:30 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/06 18:44:57 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parsing(char *line, t_main *program)
{
	char	*linenew;

	if (quote_control(line) != 0)
	{
		return (1);
	}
	linenew = empty_quotes(line);
	tokenize_args(linenew, &program->token);
	find_keys(&program->token);
	dollar_control(program->token);
	if (set_rank(program->token) == 1)
	{
		return (1);
	}
	return (0);
}

int	ft_readline(t_main *program, char **line)
{
	*line = readline("minishell> ");
	if (*line == NULL)
	{
		printf("exit\n");
		free_token(program);
		return (0);
	}
	else if (space_control(*line) == 0)
	{
		if (ft_strlen(*line) > 0)
			add_history(*line);
		free(*line);
		free_program(program, 2);
		return (1);
	}
	else if (ft_strlen(*line) > 0)
	{
		add_history(*line);
		if (parsing(*line, program) == 0)
			prep_exec(program);
	}
	return (2);
}

int	main(int ac, char **av, char **env)
{
	int		i;
	char	*line;
	t_main	program;

	(void)av;
	line = NULL;
	if (ac != 1)
		exit(1);
	signal_init();
	env_init(&program, env);
	while (1)
	{
		ft_init(&program);
		i = ft_readline(&program, &line);
		if (i == 0)
			break ;
		if (i == 1)
			continue ;
		main_free(program, line, 0);
	}
	main_free(program, line, 1);
	return (0);
}
