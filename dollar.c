#include "minishell.h"

char *get_value_from_env(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->before_eq, key) == 0)
		{
			if (env->after_eq)
				return env->after_eq;
			else
				return NULL;
		}
		env = env->next;
	}
	return NULL;
}

char	*dollar_handle(char *dollar, t_env *env)
{
	char	*new_dollar;
	char	*new;

	if (ft_strcmp(dollar, "0") == 0)
	{
		new_dollar = "minishell";
		new = ft_strdup(new_dollar);
	}
	else
	{
		new_dollar = get_value_from_env(env, dollar);
		if (new_dollar == NULL)
		{
			free(dollar);
			return (NULL);
		}
		new = ft_strdup(new_dollar);
	}
	free(dollar);
	return (new);
}

char	*find_dollar(char *content, int index, t_main *program)
{
	char	*dollar;
	char	*str;
	char	*yeter;
	char	*allahim;
	int		i;

	i = index + 1;
	if (isdigit(content[i]))
		i++;
	else
		while (ft_isalnum(content[i]) == 1)
			i++;
	dollar = ft_substr(content, index + 1, i - index - 1);
	allahim = ft_strdup(dollar);
	free(dollar);
	dollar = dollar_handle(allahim, program->env);
	str = ft_strdup(&content[i]);
	if (dollar == NULL)
	{
		free(dollar);
		return (str);
	}
	yeter = ft_strjoin(dollar, str);
	return (yeter);
}

void	dollar_control(t_token *token)
{
	char	*before_dollar;
	char	*after_dollar;
	t_token *head;
	int		index;
	int		new;

	head = token;
	after_dollar = NULL;
	token = token->next;
	while (token)
	{
		index = ft_our_strchr(token->content, 36);
		while (index >= 0 && token->flag != 39)
		{
			new = index;
			before_dollar = ft_substr(token->content, 0, index);
			after_dollar = find_dollar(token->content, index, head->program);
			free(token->content);
			token->content = ft_strjoin(before_dollar, after_dollar);
			if (ft_our_strchr(&token->content[index], 36) <= 0)
				break ;
			index = ft_our_strchr(&token->content[index], 36) + new;
		}
		token = token->next;
	}
}
