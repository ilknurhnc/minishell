/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:44:31 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/03 16:47:56 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1)
	{
		s1 = malloc(1 * sizeof(char));
		*s1 = '\0';
	}
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (*(s1 + ++i))
		*(str + i) = *(s1 + i);
	j = -1;
	while (*(s2 + ++j))
		*(str + i + j) = *(s2 + j);
	*(str + i + j) = '\0';
	free(s1);
	free(s2);
	return (str);
}

t_token	*ft_lstnew(void *content)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (0);
	node->content = content;
	node->rank = 0;
	node->tick = 0;
	node->flag = -99;
	node->next = NULL;
	return (node);
}

t_token	*ft_lstlast(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstdelone(t_token *lst, void (*del)(void *))
{
	if (!del)
		return ;
	if (lst)
	{
		(*del)(lst->content);
		free(lst);
	}
}

void	ft_lstadd(t_token *node, t_token *new)
{
	t_token	*temp;

	if (!node || !new)
		return ;
	temp = node->next;
	node->next = new;
	new->next = temp;
}
