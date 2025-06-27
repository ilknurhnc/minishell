/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 19:55:02 by ihancer           #+#    #+#             */
/*   Updated: 2025/06/27 15:46:13 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->before_eq, key) == 0)
			return (env->after_eq);
		env = env->next;
	}
	return (NULL);
}

static void update_cd(t_main *prog, char *oldpwd)
{
	char *newpwd;

	update_or_add_env(prog, "OLDPWD", oldpwd);
	free(oldpwd);
	newpwd = getcwd(NULL, 0);
	update_or_add_env(prog, "PWD", newpwd);
	free(newpwd);
}

int ft_cd(t_executor *node)
{
	t_main *prog;
	char *target;
	char *oldpwd;

	prog = node->program;
	oldpwd = getcwd(NULL, 0);
	if (!node->argv[1]) // cd
		target = get_env_value(prog->env, "HOME");
	else if (ft_strcmp(node->argv[1], "-") == 0) // cd -
	{
		target = get_env_value(prog->env, "OLDPWD");
		if (target)
			printf("%s\n", target);
	}
	else
		target = node->argv[1];
	if (!target || chdir(target) != 0)
	{
		write(2, "minishell: cd: ", 15);
		write(2, target, ft_strlen(target));
		write(2, ": No such file or directory\n", 28);
		set_exit_status_code(1);
		free(oldpwd);
		return (1);
	}
	if(node->argv[2]) // If there are more than one argument
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		set_exit_status_code(1);
		free(oldpwd);
		return (1);
	}
	update_cd(prog, oldpwd);
	return (0);
}
