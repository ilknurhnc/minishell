/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:44:45 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/03 17:09:57 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_our_strchr(char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c && (ft_isalnum(s[i + 1]) || s[i + 1] == '?'
				|| s[i + 1] == '_'))
			return (i);
		i++;
	}
	return (-1);
}

int	of_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	c = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	if (s[i] == '\0' && c == s[i])
		return (i);
	return (-1);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	c = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == '\0' && c == s[i])
		return ((char *)&s[i]);
	return (0);
}

char	*ft_strtrim(char *s1, char *set)
{
	int		len;
	char	*s;

	while (*s1 && ft_strchr(set, *s1))
		s1++;
	len = ft_strlen((char *)s1);
	while (len && s1[len - 1] && ft_strchr(set, s1[len]))
		len--;
	s = ft_substr(s1, 0, len + 1);
	return (s);
}

char	*my_join(char *line, char *s1, char *s2)
{
	int	i;
	int	j;

	if (!s1)
	{
		s1 = malloc(1 * sizeof(char));
		*s1 = '\0';
	}
	i = -1;
	while (*(s1 + ++i))
		*(line + i) = *(s1 + i);
	j = -1;
	while (*(s2 + ++j))
		*(line + i + j) = *(s2 + j);
	*(line + i + j) = '\0';
	free(s1);
	free(s2);
	return (line);
}
