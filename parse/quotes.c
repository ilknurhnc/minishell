/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:14:34 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/01 04:35:55 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*empty_quotes(char *line)
{
	int		i;
	int		len;
	char	quote;

	i = 0;
	while (line[i])
	{
		len = 0;
		if (line[i] != 34 && line[i] != 39)
		{
			i++;
			continue ;
		}
		quote = line[i];
		while (line[++i] && line[i] != quote)
			len++;
		if (line[i] == quote)
			i++;
		if (len == 0)
		{
			line = if_loop(line, i);
			i = 0;
		}
	}
	return (line);
}

int	find_quotes(char *line, int *ptr_i, int *ptr_j)
{
	int		i;
	int		j;
	char	quote;

	i = *ptr_i;
	j = *ptr_j;
	quote = -99;
	if (line[i] == 34 || line[i] == 39)
	{
		j = i + 1;
		quote = line[i];
		i++;
		while (line[j] && line[j] != quote)
			j++;
	}
	else
	{
		j = i;
		while (line[j] && line[j] != 32 && line[j] != 39 && line[j] != 34)
			j++;
	}
	*ptr_i = i;
	*ptr_j = j;
	return ((int)quote);
}

static void	token_flags(char *line, t_token *new_token, int quote)
{
	new_token->space = 0;
	new_token->flag = quote;
	if (*line == 32 || (*line >= 9 && *line <= 13))
		new_token->space = 1;
	else if (*(line) && *(line + 1) && (*(line + 1) == 32 || (*(line + 1) >= 9
				&& *(line + 1) <= 13)) && (quote == 34 || quote == 39))
		new_token->space = 1;
}

void	tokenize_args(char *line, t_token **token)
{
	int		i;
	int		j;
	int		quote;
	t_token	*new_token;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (token && ft_lstlast(*token) && ft_lstlast(*token)->space == 0
			&& ft_lstlast(*token)->flag == -99)
			i--;
		while (line[i] == 32 || (line[i] >= 9 && line[i] <= 13))
			i++;
		quote = find_quotes(line, &i, &j);
		if (j - i > 0)
		{
			new_token = ft_lstnew(find_helper(line, &i, &j));
			ft_lstadd_back(token, new_token);
			token_flags(line + j, new_token, quote);
		}
		else if (i == j && (quote == 34 || quote == 39))
			i++;
	}
}
