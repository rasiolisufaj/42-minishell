/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:50:00 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/11 21:17:53 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

static char	*extract_word(char *str, int *start)
{
	int		end;
	char	*word;

	while (str[*start] && ft_isspace(str[*start]))
		(*start)++;
	if (!str[*start])
		return (NULL);
	end = *start;
	while (str[end] && !ft_isspace(str[end]))
		end++;
	word = ft_strndup(str + *start, end - *start);
	*start = end;
	return (word);
}

static t_token	*create_token(char *word)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = T_WORD;
	new_token->value = word;
	new_token->next = NULL;
	return (new_token);
}

t_token	*create_word_tokens(char *expanded_value)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	char	*word;
	int		pos;

	if (!expanded_value || !*expanded_value)
		return (NULL);
	head = NULL;
	current = NULL;
	pos = 0;
	word = extract_word(expanded_value, &pos);
	while (word)
	{
		new_token = create_token(word);
		if (!new_token)
			return (free(word), NULL);
		if (!head)
			head = new_token;
		else
			current->next = new_token;
		current = new_token;
		word = extract_word(expanded_value, &pos);
	}
	return (head);
}

static void	link_word_tokens(t_token **list, t_token **prev,
	t_token *tokens, t_token *next)
{
	t_token	*last_word;

	if (*prev)
		(*prev)->next = tokens;
	else
		*list = tokens;
	last_word = tokens;
	while (last_word->next)
		last_word = last_word->next;
	last_word->next = next;
	*prev = last_word;
}

void	word_splitting(t_token **list, t_token **prev,
	t_token **current, char *new_value)
{
	t_token	*word_tokens;
	t_token	*next_original;

	if (!new_value || !*new_value)
		return ;
	word_tokens = create_word_tokens(new_value);
	if (!word_tokens)
		return ;
	next_original = (*current)->next;
	free((*current)->value);
	free(*current);
	link_word_tokens(list, prev, word_tokens, next_original);
	*current = next_original;
	free(new_value);
}
