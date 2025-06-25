/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:38:42 by itaharbo          #+#    #+#             */
/*   Updated: 2025/06/24 16:52:37 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

/*t_token	*handle_joker(char *str)
{
	
}*/
t_token *handle_joker(const char *pattern)
{
    (void)pattern;
    return NULL;
}

void	wildcards_cmd(t_node *node)
{
	t_token	*head;
	t_token	*tail;
	t_token	*current;
	t_token	*expanded;
	t_token	*next;

	head = NULL;
	tail = NULL;
	current = node->tokens;
	while (current)
	{
		next = current->next;
		if (ft_strchr(current->value, '*'))
		{
				expanded = handle_joker(current->value);
				if (expanded)
				{
					if (!head)
					{
						head = expanded;
						tail = expanded;
					}
					else
						tail->next = expanded;
					while (tail->next)
						tail = tail->next;
					free(current->value);
					free(current);
				}
				else
				{
					current->next = NULL;
					if (!head)
					{
						head = current;
						tail = current;
					}
					else
					{
						tail->next = current;
						tail = current;
					}
				}
		}
		else
		{
			current->next = NULL;
			if (!head)
			{
				head = current;
				tail = current;
			}
			else
			{
				tail->next = current;
				tail = current;
			}
		}
		current = next;
	}
	node->tokens = head;
}

void	expand_wildcards(t_node *node)
{
	if (!node)
		return ;
	if (node->type != N_CMD)
	{
		expand_wildcards(node->left);
		expand_wildcards(node->right);
	}
	else
		wildcards_cmd(node);
}
