/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:47:58 by ihancer           #+#    #+#             */
/*   Updated: 2025/06/30 21:37:27 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_export_error(char *arg)
{
	write(2, "minishell: export: ", 18);
	write(2, arg, ft_strlen(arg));
	write(2, ": not a valid identifier\n", 24);
}

int	is_valid_identifier(char *str)
{
	int		i;

	if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	print_export_format(t_env *env)
{
	while (env)
	{
		if (env->control == 0)
			printf("declare -x %s=\"%s\"\n", env->before_eq, env->after_eq);
		else
			printf("declare -x %s\n", env->before_eq);
		env = env->next;
	}
}

int	env_print_error(char *msg, int code)
{
	printf("%s\n", msg);
	return (code);
}
