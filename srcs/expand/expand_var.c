/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:28:38 by itaharbo          #+#    #+#             */
/*   Updated: 2025/06/24 16:54:18 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

static void	append_string(char **input, char *str)
{
	char	*tmp;

	if (!str || !*input)
		return ;
	tmp = ft_strjoin(*input, str);
	if (!tmp)
	{
		fprintf(stderr, "Error: Memory allocation failed in append_string\n");
		free(*input);
		*input = NULL;
		return ;
	}
	free(*input);
	*input = tmp;
}

char	*get_env_value(t_env *env, char *var_name)
{
	t_env	*current;

	if (!env || !var_name || !*var_name)
		return (NULL);
	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*consume_single_quote(char *value, int *i)
{
	char	*part;
	int		start;

	(*i)++;
	start = *i;
	while (value[*i] && value[*i] != '\'')
		(*i)++;
	part = ft_strndup(value + start, *i - start);
	if (!part)
		return (fprintf(stderr,
			"Error: Memory allocation failed in consume_single_quote\n"), NULL);
	if (value[*i] == '\'')
		(*i)++;
	return (part);
}

static char *expand_variable(char *value, int *i, t_env *env, int last_status)
{
	char	*part;
	char	*env_value;
	int		start;
	
	(*i)++;
	start = *i;
	if (value[*i] == '?')
	{
		part = ft_itoa(last_status);
		if (!part)
			return (fprintf(stderr,
				"Error: Memory allocation failed in expand_variable\n"), NULL);
		(*i)++;
		return (part);
	}
	while (value[*i] && (value[*i] == '_' || ft_isalnum(value[*i])))
		(*i)++;
	part = ft_strndup(value + start, *i - start);
	if (!part)
		return (fprintf(stderr,
			"Error: Memory allocation failed in expand_variable\n"), NULL);
	env_value = get_env_value(env, part);
	free(part);
	if (!env_value || !*env_value)
	{
		part = ft_strdup("");
		if (!part)
			return (fprintf(stderr,
				"Error: Memory allocation failed in expand_variable\n"), NULL);
		return (part);
	}
	part = ft_strdup(env_value);
	if (!part)
		return (fprintf(stderr,
			"Error: Memory allocation failed in expand_variable\n"), NULL);
	return (part);
}

static char	*consume_double_quote(char *value, int *i, t_env *env, int last_status)
{
	char	*part;
	char	*result;
	int		start;

	result = ft_strdup("");
	if (!result)
		return (fprintf(stderr,
			"Error: Memory allocation failed in consume_double_quote\n"), NULL);
	(*i)++;
	while (value[*i] && value[*i] != '"')
	{
		if (value[*i] == '$')
		{
			part = expand_variable(value, i, env, last_status);
			if (!part)
				return (free(result), NULL);
			append_string(&result, part);
			free(part);
			if (!result)
				return (fprintf(stderr,
					"Error: Memory allocation failed in consume_double_quote\n"), NULL);
		}
		else
		{
			start = *i;
			while (value[*i] && value[*i] != '"' && value[*i] != '$')
				(*i)++;
			if (start < *i)
			{
				part = ft_strndup(value + start, *i - start);
				if (!part)
					return (free(result), fprintf(stderr,
						"Error: Memory allocation failed in consume_double_quote\n"), NULL);
				append_string(&result, part);
				free(part);
				if (!result)
					return (fprintf(stderr,
						"Error: Memory allocation failed in consume_double_quote\n"), NULL);
			}
		}
	}
	if (value[*i] == '"')
		(*i)++;
	return (result);
}

static char	*consume_unquoted(char *value, int *i)
{
	char	*part;
	int		start;

	start = *i;
	while (value[*i] && value[*i] != '\'' && value[*i] != '"'
		&& value[*i] != '$')
		(*i)++;
	part = ft_strndup(value + start, *i - start);
	if (!part)
		return (fprintf(stderr,
			"Error: Memory allocation failed in consume_unquoted\n"), NULL);
	return (part);
}

char	*expand_token(char *value, t_env *env, int last_status)
{
	char	*result;
	char	*part;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	part = NULL;
	i = 0;
	while (value[i])
	{
		if (value[i] == '\'')
		{
			part = consume_single_quote(value, &i);
			if (!part)
				return (free(result), NULL);
		}	
		else if (value[i] == '"')
		{
			part = consume_double_quote(value, &i, env, last_status);
			if (!part)
				return (free(result), NULL);
		}
		else if (value[i] == '$')
		{
			part = expand_variable(value, &i, env, last_status);
			if (!part)
				return (free(result), NULL);
		}
		else
		{
			part = consume_unquoted(value, &i);
			if (!part)
				return (free(result), NULL);
		}
		append_string(&result, part);
		free(part);
		if (!result)
			return (fprintf(stderr,
				"Error: Memory allocation failed in expand_token\n"), NULL);
	}
	return (result);
}

void	expand_ast(t_node *node, t_env *env, int last_status)
{
	t_token	*current;
	char	*old_value;
	char	*new_value;

	if (!node)
		return ;
	if (node->type != N_CMD)
	{
		expand_ast(node->left, env, last_status);
		expand_ast(node->right, env, last_status);
	}
	else if (node->type == N_CMD)
	{
		current = node->tokens;
		while (current)
		{
			old_value = current->value;
			new_value = expand_token(old_value, env, last_status);
			if (!new_value)
			{
				fprintf(stderr, "Error: Memory allocation failed in expand_ast\n");
				return ;
			}
			current->value = new_value;
			free(old_value);
			current = current->next;
		}
		current = node->redir_list;
		while (current)
		{
			old_value = current->value;
			new_value = expand_token(old_value, env, last_status);
			if (!new_value)
			{
				fprintf(stderr, "Error: Memory allocation failed in expand_ast\n");
				return ;
			}
			current->value = new_value;
			free(old_value);
			current = current->next;
		}
	}
}
