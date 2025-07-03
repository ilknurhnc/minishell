/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:04:18 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/03 17:09:09 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalnum(int c)
{
	if (c == 0)
		return (0);
	return (ft_isalpha(c) || ft_isdigit(c));
}

int	ft_atoi(const char *str)
{
	int	sign;
	int	result;
	int	i;

	sign = 1;
	result = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = sign * (-1);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

static int	bas_num(long n)
{
	int	digit_num;

	digit_num = 0;
	if (n < 0)
	{
		digit_num += 1;
		n *= -1;
	}
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		digit_num++;
	}
	return (digit_num);
}

char	*ft_itoa(int n)
{
	int		digit_num;
	char	*s;
	long	num;

	num = n;
	digit_num = bas_num(n);
	s = malloc((digit_num + 1) * sizeof(char));
	if (s == NULL)
		return (NULL);
	s[digit_num] = '\0';
	if (n == 0)
		s[0] = '0';
	else if (n < 0)
	{
		s[0] = '-';
		num = num * -1;
	}
	while (num > 0)
	{
		s[--digit_num] = (num % 10) + '0';
		num = num / 10;
	}
	return (s);
}

char	*ft_strnstr(const char *src, const char *to_find, size_t n)
{
	size_t	j;
	size_t	i;

	i = 0;
	j = 0;
	if (*(to_find) == '\0')
		return ((char *)src);
	while (src[i] != '\0')
	{
		while (src[i + j] == to_find[j] && (j + i) < n)
		{
			j++;
			if (to_find[j] == '\0')
				return ((char *)&src[i]);
		}
		j = 0;
		i++;
	}
	return (0);
}
