/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 19:01:01 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/11 20:55:21 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

int	check_quotes(char *value)
{
	if (ft_strchr(value, '"') != NULL
		|| ft_strchr(value, '\'') != NULL)
		return (1);
	return (0);
}

void	free_token_content(t_token *token, char *new_val,
		char *old_val)
{
	free(new_val);
	free(old_val);
	free(token);
}

void	update_token_links(t_token **head, t_token *prev,
	t_token *current, t_token *next)
{
	if (prev)
		prev->next = next;
	else
		*head = next;
	if (current)
		current = next;
}

void	handle_valid_token(t_token **prev, t_token **current,
	char *new_value, char *old_value)
{
	(*current)->value = new_value;
	free(old_value);
	*prev = *current;
	*current = (*current)->next;
}

int	should_word_split(char *new_value, char *old_value)
{
	int	i;

	if (!new_value || !*new_value || check_quotes(old_value))
		return (0);
	i = 0;
	while (new_value[i])
	{
		if (new_value[i] == ' ' || new_value[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}
