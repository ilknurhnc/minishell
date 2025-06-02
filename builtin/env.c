/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 09:21:49 by hbayram           #+#    #+#             */
/*   Updated: 2025/06/02 19:50:29 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_lstsize_env(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

void	get_env(t_env **envp, char **env)
{
	int		i;
	int		index;
	t_env	*new;

	i = -1;
	*envp = NULL;
	while (env[++i])
	{
		index = of_strchr(env[i], 61);
		if (index == -1)
			continue ;
		new = a_lstnew(ft_substr(env[i], 0, index), ft_substr(env[i], index + 1,
					ft_strlen(env[i]) - index - 1));
		ft_envadd_back(envp, new);
	}
}

void	fill_array(t_main *program, int i)
{
	t_main	*temp;
	t_env	*env;
	int		index;

	index = 0;
	env = program->env;
	temp = program;
	temp->env_str = (char **)malloc(sizeof(char *) * (i + 1));
	if (!temp->env_str)
		return ;
	while (i > index && env)
	{
		temp->env_str[index] = ft_strdup(env->full_str);
		index++;
		env = env->next;
	}
	temp->env_str[index] = NULL;
}

void	set_env(t_main *program, t_env *env)
{
	char	*str;
	char	*new;
	int		i;
	t_env	*temp;

	i = 0;
	temp = env;
	while (temp)
	{
		new = ft_strdup(temp->before_eq);
		str = ft_strjoin(new, ft_strdup("="));
		new = ft_strdup(temp->after_eq);
		temp->full_str = ft_strjoin(str, new);
		temp = temp->next;
	} 
	temp = env;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	fill_array(program, i);
}
