/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:34:19 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/08 14:08:43 by hbayram          ###   ########.fr       */
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
			set_keys(temp);
		}
		temp = temp->next;
	}
}

char	*find_helper(char *line, int *ptr_i, int *ptr_j)
{
	int		i;
	int		j;
	char	*content;

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

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*d;

	i = 0;
	d = (unsigned char *)s;
	while (n > i)
	{
		d[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

int	cannot_open(t_main *program)
{
	free_token(program);
	free_exec(program);
	free_env(program);
	exit(1);
}
