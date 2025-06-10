/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:01:21 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/20 21:01:21 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*elem;
	t_list	*new;

	if (!lst || !f)
		return (NULL);
	elem = NULL;
	while (lst)
	{
		new = ft_lstnew((*f)(lst->content));
		if (!new)
		{
			while (elem)
			{
				new = elem->next;
				(*del)(elem->content);
				free(new);
				elem = new;
			}
			lst = NULL;
			return (NULL);
		}
		ft_lstadd_back(&elem, new);
		lst = lst->next;
	}
	return (elem);
}
