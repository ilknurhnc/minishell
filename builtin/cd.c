/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 19:55:02 by ihancer           #+#    #+#             */
/*   Updated: 2025/07/13 11:43:55 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->before_eq, key) == 0)
			return (env->after_eq);
		env = env->next;
	}
	return (NULL);
}

static void	update_cd(t_main *prog, char *oldpwd)
{
	char	*newpwd;

	update_or_add_env(prog, "OLDPWD", oldpwd);
	free(oldpwd);
	newpwd = getcwd(NULL, 0);
	update_or_add_env(prog, "PWD", newpwd);
	free(newpwd);
}

static int	cd_error(char *target, char *msg, char *oldpwd)
{
	write(2, "minishell: cd: ", 15);
	if (target)
		write(2, target, ft_strlen(target));
	write(2, msg, ft_strlen(msg));
	set_exit_status_code(1);
	free(oldpwd);
	return (1);
}

int	ft_cd(t_executor *node)
{
	t_main	*prog;
	char	*target;
	char	*oldpwd;

	prog = node->program;
	oldpwd = getcwd(NULL, 0);
	if (node->argv[2] != NULL)
		return (cd_error(NULL, "too many arguments\n", oldpwd));
	if (!node->argv[1] || ft_strcmp(node->argv[1], "~") == 0)
		target = get_env_value(prog->env, "HOME");
	else if (ft_strcmp(node->argv[1], "-") == 0)
	{
		target = get_env_value(prog->env, "OLDPWD");
		if (target)
			printf("%s\n", target);
	}
	else
		target = node->argv[1];
	if (!target)
		return (cd_error(target, ": HOME not set\n", oldpwd));
	if (chdir(target) != 0)
		return (cd_error(target, ": No such file or directory\n", oldpwd));
	update_cd(prog, oldpwd);
	return (0);
}
