/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 19:21:34 by itaharbo          #+#    #+#             */
/*   Updated: 2025/07/04 20:54:30 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

// ─── Construction de la liste t_env à partir de envp ──────────────────────────

t_env *build_env(char **envp)
{
    t_env *head = NULL, *tail = NULL;
    for (int i = 0; envp[i]; i++)
    {
        char *eq = strchr(envp[i], '=');
        if (!eq) continue;
        t_env *node = malloc(sizeof *node);
        if (!node) { perror("malloc"); exit(1); }
        node->key   = strndup(envp[i], eq - envp[i]);
        node->value = strdup(eq + 1);
        node->next  = NULL;
        if (!head) head = tail = node;
        else       tail->next = node, tail = node;
    }
    return head;
}

void free_env(t_env *env)
{
    while (env)
    {
        t_env *n = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = n;
    }
}

// ─── Helpers d’affichage ──────────────────────────────────────────────────────

const char *token_type_to_str(t_token_type type)
{
    switch (type)
    {
        case T_WORD:         return "WORD";
        case T_PIPE:         return "PIPE";
        case T_REDIR_IN:     return "REDIR_IN";
        case T_REDIR_OUT:    return "REDIR_OUT";
        case T_REDIR_APPEND: return "APPEND";
        case T_HEREDOC:      return "HEREDOC";
        case T_AND:          return "AND";
        case T_OR:           return "OR";
        case T_PAREN_OPEN:   return "PAREN_OPEN";
        case T_PAREN_CLOSE:  return "PAREN_CLOSE";
        default:             return "UNKNOWN";
    }
}

void print_tokens(const char *label, t_token *tok)
{
    printf("%s\n", label);
    while (tok)
    {
        printf("  [%s] \"%s\"\n",
               token_type_to_str(tok->type),
               tok->value);
        tok = tok->next;
    }
}

void print_token_list(const char *label, t_token *list)
{
    if (!list) return;
    printf("    %s:", label);
    for (; list; list = list->next)
        printf(" [%s]", list->value);
    printf("\n");
}

void print_ast(t_node *node, int indent)
{
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");
    switch (node->type)
    {
        case N_CMD:
            printf("CMD\n");
            print_token_list("ARGS",   node->tokens);
            print_token_list("REDIRS", node->redir_list);
            break;
        case N_PIPE:  printf("PIPE\n");         break;
        case N_AND:   printf("AND\n");          break;
        case N_OR:    printf("OR\n");           break;
        case N_GROUP: printf("GROUP ( )\n");    break;
    }
    print_ast(node->left,  indent + 1);
    print_ast(node->right, indent + 1);
}

// ─── main ────────────────────────────────────────────────────────────────────

int main(int argc, char **argv, char **envp)
{
	argc = 0; // Pour éviter les warnings
	argv = NULL; // Pour éviter les warnings
    // 1) Construire l’environnement
    (void)argc;
    (void)argv;
    t_env *env_list = build_env(envp);
    int    last_status = 0;

    // 2) Lecture de la ligne
    char buffer[1024];
    printf("minishell$ ");
    if (!fgets(buffer, sizeof buffer, stdin))
    {
        free_env(env_list);
        return 0;
    }
    buffer[strcspn(buffer, "\n")] = '\0';

    // 3) Parsing + expansion variables + wildcard
    t_node *ast = parsing_file(buffer, env_list, last_status);
    if (!ast)
    {
        free_env(env_list);
        return 1;
    }

    // 4) Affichage pour débogage
    printf("\nTokens finaux et AST complet :\n");
    print_ast(ast, 0);

    // 5) Cleanup
    free_env(env_list);
    free_node(ast);
    return 0;
}
