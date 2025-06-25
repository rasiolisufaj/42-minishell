/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaharbo <itaharbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 02:56:14 by itaharbo          #+#    #+#             */
/*   Updated: 2025/06/24 09:35:25 by itaharbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Fonction simplifiée pour récupérer une valeur d'environnement
char *get_env_value(t_env *env, char *name) {
    if (strcmp(name, "USER") == 0)
        return "idrisstaharbouche";
    if (strcmp(name, "HOME") == 0)
        return "/Users/idrisstaharbouche";
    if (strcmp(name, "US") == 0)
        return "";
    return NULL;
}

// Création d'un token pour les tests
t_token *create_test_token(char *value) {
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->type = T_WORD;
    token->value = strdup(value);
    token->next = NULL;
    return token;
}

// Déclaration de la fonction à tester
char	*expand_token(char *value, t_env *env, int last_status);

int main() {
    t_env *env = NULL; // Environnement simplifié, la fonction get_env_value est mocké
	int	last_status = 127; // Dernier statut de commande
    // Définir les cas de test
    char *test_cases[] = {
        "hello world",                 // Texte simple
        "$USER",                       // Variable simple
        "'$USER'",                     // Variable dans guillemets simples
        "\"$USER\"",                   // Variable dans guillemets doubles
        "\"'$USER'\"",                 // Guillemets imbriqués (1)
        "'\"$USER\"'",                 // Guillemets imbriqués (2)
        "\"'\"$USER\"'\"",             // Cas complexe (1)
        "'\"'$USER'\"'",               // Cas complexe (2)
        "$US\"ER\"",                   // Expansion partielle (1)
        "hel\"lo$USER\"",              // Expansion partielle (2)
        "\"$US\"\"ER\"",               // Guillemets adjacents (1)
        "\"$HOME\"'$USER'",            // Guillemets adjacents (2)
        NULL
    };

    printf("===== TEST D'EXPANSION DE VARIABLES =====\n\n");

    for (int i = 0; test_cases[i] != NULL; i++) {
        t_token *token = create_test_token(test_cases[i]);
        char *expanded = expand_token(token->value, env, last_status);
        
        printf("Test %2d: %s\n", i + 1, test_cases[i]);
        printf("  Résultat: %s\n\n", expanded);
        
        free(expanded);
        free(token->value);
        free(token);
    }

    return 0;
}
