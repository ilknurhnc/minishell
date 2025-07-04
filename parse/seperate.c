/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   seperate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:24:37 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/04 15:15:40 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	seperate_two_helper(t_token *token, t_token *temp, char *content,
		int len)
{
	t_token	*new;

	new = ft_lstnew(ft_substr(token->content, len, ft_strlen(token->content)
				- len));
	token->next = new;
	new->next = temp;
	free(token->content);
	token->content = ft_strdup(content);
	new->space = token->space;
	token->space = 0;
}

void	separate_two(t_token *token, char *content, int len, int location)
{
	t_token	*temp;
	t_token	*new;
	char	*str;
	char	*tmp;

	temp = token->next;
	if (location == 0)
		seperate_two_helper(token, temp, content, len);
	else
	{
		new = ft_lstnew(ft_strdup(content));
		token->next = new;
		new->next = temp;
		tmp = ft_substr(token->content, 0, ft_strlen(token->content) - len);
		str = ft_strdup(tmp);
		free(tmp);
		free(token->content);
		token->content = str;
		new->space = token->space;
		token->space = 0;
	}
}

void	separate_three(t_token *token, char *content, int len, int location)
{
	t_token	*temp;
	t_token	*last;
	t_token	*new;
	char	*str;
	int		str_len;

	str = ft_strdup(token->content);
	str_len = ft_strlen(str);
	temp = token->next;
	new = ft_lstnew(ft_strdup(content));
	last = ft_lstnew(ft_substr(str, location + len, str_len - location - len));
	token->next = new;
	new->next = last;
	last->next = temp;
	free(token->content);
	token->content = ft_substr(str, 0, location);
	free(str);
	new->space = 0;
	last->space = token->space;
	token->space = 0;
}

void	arrange_tokens(t_token *token, char *content, int len)
{
	int	i;

	i = 0;
	i = ft_strstr(token->content, content);
	if (i == 0 && !(i + len == (int)ft_strlen(token->content)))
		separate_two(token, content, len, i);
	else if (i != 0 && i + len == (int)ft_strlen(token->content))
	{
		separate_two(token, content, len, i);
		token = token->next;
	}
	else if (i >= 0 && !(i + len == (int)ft_strlen(token->content)))
	{
		separate_three(token, content, len, i);
		token = token->next;
	}
}

void	set_keys(t_token *temp)
{
	if (temp->content != NULL && ft_strstr(temp->content, "<<") != -1)
		arrange_tokens(temp, "<<", 2);
	if ((temp->content != NULL && ft_strstr(temp->content, ">>") != -1))
		arrange_tokens(temp, ">>", 2);
	if (temp->content != NULL && ft_strchr(temp->content, '>'))
	{
		if (ft_strstr(temp->content, ">>") != -1)
			arrange_tokens(temp, ">>", 2);
		else
			arrange_tokens(temp, ">", 1);
	}
	if (temp->content != NULL && ft_strchr(temp->content, '<'))
	{
		if (ft_strstr(temp->content, "<<") != -1)
			arrange_tokens(temp, "<<", 2);
		else
			arrange_tokens(temp, "<", 1);
	}
	if (temp->content != NULL && ft_strchr(temp->content, '|'))
		arrange_tokens(temp, "|", 1);
}
