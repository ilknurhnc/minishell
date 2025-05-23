/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:30 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/23 17:09:23 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token(t_token *list)
{
	while (list)
	{
		printf("%s\n", list->content);
		printf("%d\n", list->space);
		printf("%d\n", list->flag);
		printf("RANK %d\n\n", list->rank);
		list = list->next;
	}
}
void	print_exec(t_exec *exec)
{
	while (exec)
	{
		if (exec->content)
			printf("%s\n", exec->content);
		if(exec->rank)
			printf("RANK %d\n", exec->rank);
		exec = exec->next;
	}
}

int	parsing(char *line, t_main *program)
{
	char	*linenew;

	if (quote_control(line) != 0)
	{
		return 1;
	}
	linenew = empty_quotes(line);
	tokenize_args(linenew, &program->token);
	find_keys(&program->token);
	dollar_control(program->token);
	if (set_rank(program->token) == 1)
	{
		return 1;
	}
	//print_token(program->token->next);
	return 0;
}

// void print_exec_list(t_exec *cmd)
// {
//     while (cmd)
//     {
//         printf("NODE: content  =   %s, rank = %d, space = %d\n",
//                cmd->content ? cmd->content : "(null)",
//                cmd->rank, cmd->space);
//         cmd = cmd->next;
//     }
// }

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_main	program;

	(void)av;
	if (ac != 1)
		exit(61);
	signal_init();
	while (1)
	{
		ft_init(&program, env);
		line = readline("ilknur&&eslem<3 ");
		
		if (line == NULL) // Eğer Ctrl-D ile EOF alırsak,
			//readline() NULL döndürecektir
		{
			printf("exit\n");
			break ;
		}
		else if(space_control(line) == 0)
			continue;
		else if (ft_strlen(line) > 0)
		{ 
			add_history(line);
			if (parsing(line, &program) == 0)
				prep_exec(&program);
		}
		main_free(program, line, 0);
	}
	main_free(program, line, 1);
	return (0);
}
