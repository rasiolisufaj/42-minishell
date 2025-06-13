/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 19:21:34 by itaharbo          #+#    #+#             */
/*   Updated: 2025/06/12 19:23:07 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	print_tokens(const char *label, t_token *tok)
{
    printf("%s\n", label);
    for (; tok; tok = tok->next)
        printf("  [%s] \"%s\"\n",
            tok->type == T_WORD ? "WORD" : "REDIR",
            tok->value);
}

int	main(void)
{
    char    buffer[1024];
    t_token *tokens;
	t_token	*tokens_head;
    t_node  *cmd;

    printf("Entrez une commande à tester> ");
    if (!fgets(buffer, sizeof buffer, stdin))
        return 1;
    buffer[strcspn(buffer, "\n")] = '\0';

    tokens = lexer(buffer);
    tokens_head = tokens;

    print_tokens("Tokens avant parse_cmd:", tokens);

    cmd = parse_cmd(&tokens);
    if (!cmd) 
	{
        fprintf(stderr, "parse_cmd a échoué\n");
        free_tokens(tokens_head);
        return 1;
    }

    print_tokens("Tokens dans node->tokens:", cmd->tokens);

    free_tokens(tokens_head);
    free_tokens(cmd->tokens);
    free(cmd);
    return 0;
}
