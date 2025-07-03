/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:44:12 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/03 16:49:53 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	space_control(char *s)
{
	int	i;

	i = 0;
	if (!s[0])
		return (0);
	while (s[i])
	{
		if (s[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

t_env	*a_lstnew(char *before, char *after)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
	{
		perror("malloc failed");
		exit(1);
	}
	new->control = 0;
	new->full_str = malloc(sizeof(before) + sizeof(after) + 2);
	new->before_eq = before;
	new->after_eq = after;
	new->next = NULL;
	return (new);
}

void	ft_envadd_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_exec	*ft_lstnew_exec(void *content)
{
	t_exec	*node;

	node = malloc(sizeof(t_exec));
	if (!node)
		return (0);
	node->content = content;
	node->space = 0;
	node->tick = 0;
	node->next = NULL;
	return (node);
}

void	ft_execadd_back(t_exec **lst, t_exec *new)
{
	t_exec	*temp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}
