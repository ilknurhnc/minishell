/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:34:19 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/01 04:36:09 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*if_loop(char *line, int i)
{
	char	*first;
	char	*new;

	first = ft_substr(line, 0, i - 2);
	new = ft_substr(line, i, ft_strlen(&line[i]));
	line = my_join(line, first, new);
	return (line);
}

void	find_keys(t_token **token)
{
	t_token	*temp;

	temp = (*token)->next;
	while (temp)
	{
		if (temp->flag == -99)
		{
			new_func(temp);
		}
		temp = temp->next;
	}
}

char	*find_helper(char *line, int *ptr_i, int *ptr_j)
{
	int		i;
	int		j;
	char *content;

	i = *ptr_i;
	j = *ptr_j;
	content = ft_substr(line, i, j - i);
	if (j - i > 0)
	{
		i = j;
		if (line[i] && line[i] != '\0')
			i++;
	}
	*ptr_i = i;
	*ptr_j = j;
	return (content);
}