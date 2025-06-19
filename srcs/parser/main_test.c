/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 19:21:34 by itaharbo          #+#    #+#             */
/*   Updated: 2025/06/19 22:40:16 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *token_type_to_str(t_token_type type)
{
    if (type == T_WORD) return "WORD";
    if (type == T_PIPE) return "PIPE";
    if (type == T_REDIR_IN) return "REDIR_IN";
    if (type == T_REDIR_OUT) return "REDIR_OUT";
    if (type == T_REDIR_APPEND) return "APPEND";
    if (type == T_HEREDOC) return "HEREDOC";
    if (type == T_AND) return "AND";
    if (type == T_OR) return "OR";
    if (type == T_PAREN_OPEN) return "PAREN_OPEN";
    if (type == T_PAREN_CLOSE) return "PAREN_CLOSE";
    return "UNKNOWN";
}

void	print_tokens(const char *label, t_token *tok)
{
    printf("%s\n", label);
    while (tok)
    {
        printf("  [%s] \"%s\"\n", token_type_to_str(tok->type), tok->value);
        tok = tok->next;
    }
}

void	print_token_list(const char *label, t_token *list)
{
	if (!list)
		return ;
	printf("    %s:", label);
	while (list)
	{
		printf(" [%s]", list->value);
		list = list->next;
	}
	printf("\n");
}

void	print_ast(t_node *node, int indent)
{
	if (!node)
		return ;

	// Indentation
	for (int i = 0; i < indent; i++)
		printf("  ");

	// Affichage selon le type de nœud
	if (node->type == N_CMD)
	{
		printf("CMD\n");
		print_token_list("ARGS", node->tokens);
		print_token_list("REDIRS", node->redir_list);
	}
	else if (node->type == N_PIPE)
		printf("PIPE\n");
	else if (node->type == N_AND)
		printf("AND\n");
	else if (node->type == N_OR)
		printf("OR\n");
	else if (node->type == N_GROUP)
		printf("GROUP ( )\n");

	// Récursivement afficher les sous-nœuds
	if (node->left)
		print_ast(node->left, indent + 1);
	if (node->right)
		print_ast(node->right, indent + 1);
}

int	main(void)
{
	char	buffer[1024];
	t_token	*tokens;
	t_token	*tokens_head;
	t_node	*ast;

	printf("Entrez une commande à tester> ");
	if (!fgets(buffer, sizeof buffer, stdin))
		return (1);
	buffer[strcspn(buffer, "\n")] = '\0';

	tokens = lexer(buffer);
	tokens_head = tokens;

	printf("\nTokens lus :\n");
	print_tokens(">", tokens);

	ast = parse_and_or(&tokens);
	if (!ast)
	{
		fprintf(stderr, "Erreur dans parse_and_or\n");
		free_tokens(tokens_head);
		return (1);
	}

	printf("\nAST généré :\n");
	print_ast(ast, 0);

	free_tokens(tokens_head);
	free_node(ast);
	return (0);
}
