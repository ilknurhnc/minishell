/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:57:59 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/03 18:47:23 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_path(char *dir, const char *cmd)
{
	char	*full_path;
	int		len;

	len = ft_strlen(dir) + ft_strlen((char *)cmd) + 2;
	full_path = malloc(len);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	if (dir[ft_strlen(dir) - 1] != '/')
		ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

char	*get_path_from_env(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->before_eq, "PATH") == 0)
		{
			if (env->after_eq && ft_strlen(env->after_eq) > 0)
				return (env->after_eq);
			else
				return (NULL);
		}
		env = env->next;
	}
	return (NULL);
}

char	*get_next_path_dir(char *path_str, int *start_pos)
{
	int		i;
	int		start;
	int		len;
	char	*token;

	i = *start_pos;
	if (!path_str || path_str[i] == '\0')
		return (NULL);
	start = i;
	while (path_str[i] != ':' && path_str[i] != '\0')
		i++;
	len = i - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strncpy(token, &path_str[start], len);
	token[len] = '\0';
	if (path_str[i] == ':')
		*start_pos = i + 1;
	else
		*start_pos = i;
	return (token);
}

char	*is_executable_path(char *command)
{
	if (!command || !*command)
		return (NULL);
	if (command[0] == '/')
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
	}
	else if (command[0] == '.' && (command[1] == '/' || (command[1] == '.'
				&& command[2] == '/')))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
	}
	return (NULL);
}

char	*find_command_path(t_main *program, char *command)
{
	char	*dir;
	char	*path_env;
	char	*full_path;
	int		pos;

	full_path = is_executable_path(command);
	if (full_path)
		return (full_path);
	path_env = get_path_from_env(program->env);
	if (!path_env)
		return (NULL);
	pos = 0;
	dir = get_next_path_dir(path_env, &pos);
	while (dir)
	{
		full_path = join_path(dir, command);
		free(dir);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		dir = get_next_path_dir(path_env, &pos);
	}
	return (NULL);
}
