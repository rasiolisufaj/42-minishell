/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:59:55 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/20 20:59:55 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*elem;

	if (!del || !lst || !*lst)
		return ;
	while (lst && *lst)
	{
		elem = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = elem;
	}
}
