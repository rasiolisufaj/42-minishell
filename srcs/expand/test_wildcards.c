/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_wildcards.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 16:50:50 by itaharbo          #+#    #+#             */
/*   Updated: 2025/06/24 16:55:32 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/minishell_structs.h"

// Stub de handle_joker : renvoie toujours NULL pour l'instant
/*t_token *handle_joker(const char *pattern)
{
    (void)pattern;
    return NULL;
}*/

// Prototype de ta fonction
void wildcards_cmd(t_node *node);
void expand_wildcards(t_node *node);

// Helper pour créer un token WORD
t_token *new_token(const char *value)
{
    t_token *tok = malloc(sizeof *tok);
    if (!tok) { perror("malloc"); exit(1); }
    tok->type  = T_WORD;
    tok->value = strdup(value);
    if (!tok->value) { perror("strdup"); exit(1); }
    tok->next  = NULL;
    return tok;
}

// Affiche la liste de tokens (type et valeur)
void print_tokens(const char *label, t_token *tok)
{
    printf("%s\n", label);
    while (tok)
    {
        printf("  [%s] \"%s\"\n",
               tok->type == T_WORD ? "WORD" : "OTHER",
               tok->value);
        tok = tok->next;
    }
}

int main(void)
{
    // Construisons un petit N_CMD avec quelques valeurs, incluant des *
    t_node node;
    node.type      = N_CMD;
    node.left      = node.right = NULL;
    node.redir_list = NULL;

    // Chaînons manuellement quelques tokens : "foo", "bar*", "*baz", "qux"
    t_token *head = NULL, *tail = NULL;
    const char *tests[] = { "foo", "bar*", "*baz", "qux", NULL };
    for (int i = 0; tests[i]; i++)
    {
        t_token *tok = new_token(tests[i]);
        if (!head) head = tail = tok;
        else       tail->next = tok, tail = tok;
    }
    node.tokens = head;

    // Affichage avant expansion
    print_tokens("Avant wildcards:", node.tokens);

    // Appel de ta passe de wildcard (qui ne fera rien pour l'instant)
    expand_wildcards(&node);

    // Affichage après expansion
    print_tokens("Après wildcards:", node.tokens);

    // Cleanup
    t_token *cur = node.tokens;
    while (cur)
    {
        t_token *n = cur->next;
        free(cur->value);
        free(cur);
        cur = n;
    }
    return 0;
}
