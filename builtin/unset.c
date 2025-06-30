/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:38:37 by ihancer           #+#    #+#             */
/*   Updated: 2025/06/30 16:45:06 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier_unset(char *str)
{
	int		i;

	if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	remove_var(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->before_eq, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->before_eq);
			free(current->after_eq);
			free(current->full_str);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(t_executor *node)
{
	t_main	*prog;
	int		i;

	prog = node->program;
	i = 1;
	while (node->argv[i])
	{
		if (is_valid_identifier_unset(node->argv[i]))
			remove_var(&prog->env, node->argv[i]);
		else
		{
			printf("minishell: unset: `%s`: not a valid identifier\n",
				node->argv[i]);
		}
		i++;
	}
	fill_array(prog, ft_lstsize_env(prog->env));
	return (0);
}
