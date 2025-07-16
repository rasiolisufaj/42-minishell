/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:12:49 by raisufaj          #+#    #+#             */
/*   Updated: 2025/07/16 17:14:16 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

void	sig_heredoc(int status, int *sig)
{
	*sig = WTERMSIG(status);
	if (*sig == SIGINT)
		ft_putstr_fd("\n", 2);
}
