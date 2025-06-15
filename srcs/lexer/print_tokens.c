#include "../../includes/minishell.h"
#include "../../includes/minishell_structs.h"

/*int	main(void)
{
	char	buffer[1024];
	t_token	*tokens;
	t_token	*tmp;

	printf("minishell$ ");
	if (!fgets(buffer, sizeof(buffer), stdin))
	{
		perror("fgets");
		return (1);
	}
	buffer[strcspn(buffer, "\n")] = '\0';
	tokens = lexer(buffer);
	tmp = tokens;
	while (tmp)
	{
		printf("Token: %s, value: %d\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
	free_tokens(tokens);
	return (0);
}*/

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: %s, Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}
