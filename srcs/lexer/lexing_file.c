// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   lexing_file.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/05/30 15:20:18 by itaharbo          #+#    #+#             */
// /*   Updated: 2025/06/10 16:18:46 by raisufaj         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../includes/minishell_structs.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// int	is_space(char c)
// {
// 	return (c == 32 || c == 9);
// }

// int	is_operator(char c)
// {
// 	return (c == '>' || c == '<' || c == '&' || c == '|'
// 		|| c == '(' || c == ')');
// }

// int	ft_strlen(const char *s)
// {
// 	int	len;

// 	len = 0;
// 	while (s[len] != '\0')
// 		len++;
// 	return (len);
// }

// char	*ft_strjoin(const char *s1, const char *s2)
// {
// 	char	*str;
// 	size_t	len1;
// 	size_t	len2;
// 	size_t	i;

// 	if (!s1 || !s2)
// 		return (NULL);
// 	len1 = ft_strlen(s1);
// 	len2 = ft_strlen(s2);
// 	str = malloc(sizeof(char) * (len1 + len2 + 1));
// 	if (!str)
// 		return (NULL);
// 	i = -1;
// 	while (++i < len1)
// 		str[i] = s1[i];
// 	i = -1;
// 	while (++i < len2)
// 		str[len1 + i] = s2[i];
// 	str[len1 + len2] = '\0';
// 	return (str);
// }

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	char	*str;
// 	size_t	i;

// 	if (start >= (unsigned int)ft_strlen(s))
// 	{
// 		str = malloc(1);
// 		if (!str)
// 			return (NULL);
// 		str[0] = '\0';
// 		return (str);
// 	}
// 	if (start + len > (unsigned int)ft_strlen(s))
// 		len = ft_strlen(s) - start;
// 	str = malloc(sizeof(char) * (len + 1));
// 	if (!str)
// 		return (NULL);
// 	i = 0;
// 	while (i < len)
// 	{
// 		str[i] = s[start + i];
// 		i++;
// 	}
// 	str[i] = '\0';
// 	return (str);
// }

// char	*ft_strdup(const char *s)
// {
// 	char	*ptr;
// 	size_t	len;
// 	size_t	i;

// 	i = 0;
// 	len = ft_strlen(s) + 1;
// 	ptr = (char *)malloc(sizeof(char) * len);
// 	if (ptr == 0)
// 		return (0);
// 	while (s[i])
// 	{
// 		ptr[i] = s[i];
// 		i++;
// 	}
// 	ptr[i] = '\0';
// 	return (ptr);
// }

// void	pipe_token(const char *input, int *i, t_token *new_token)
// {
// 	if (input[*i + 1] == '|')
// 	{
// 		new_token->type = T_OR;
// 		new_token->value = ft_strdup("||");
// 		*i += 2;
// 	}
// 	else
// 	{
// 		new_token->type = T_PIPE;
// 		new_token->value = ft_strdup("|");
// 		(*i)++;
// 	}
// }

// void	redir_less_token(const char *input, int *i, t_token *new_token)
// {
// 	if (input[*i + 1] == '<')
// 	{
// 		new_token->type = T_HEREDOC;
// 		new_token->value = ft_strdup("<<");
// 		*i += 2;
// 	}
// 	else
// 	{
// 		new_token->type = T_REDIR_IN;
// 		new_token->value = ft_strdup("<");
// 		(*i)++;
// 	}
// }

// void	redir_great_token(const char *input, int *i, t_token *new_token)
// {
// 	if (input[*i + 1] == '>')
// 	{
// 		new_token->type = T_REDIR_APPEND;
// 		new_token->value = ft_strdup(">>");
// 		*i += 2;
// 	}
// 	else
// 	{
// 		new_token->type = T_REDIR_OUT;
// 		new_token->value = ft_strdup(">");
// 		(*i)++;
// 	}
// }

// void	parenthesis_token(const char *input, int *i, t_token *new_token)
// {
// 	if (input[*i] == '(')
// 	{
// 		new_token->type = T_PAREN_OPEN;
// 		new_token->value = ft_strdup("(");
// 		(*i)++;
// 	}
// 	else if (input[*i] == ')')
// 	{
// 		new_token->type = T_PAREN_CLOSE;
// 		new_token->value = ft_strdup(")");
// 		(*i)++;
// 	}
// }

// char	*concat_quote(const char *input, int *i, char *quoted)
// {
// 	int		start;
// 	char	*after;
// 	char	*joined;

// 	start = *i;
// 	while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i])
// 		&& input[*i] != '\'' && input[*i] != '"')
// 		(*i)++;
// 	if (*i > start)
// 	{
// 		after = ft_substr(input, start, *i - start);
// 		joined = malloc(sizeof(char) * (ft_strlen(quoted) + ft_strlen(after)
// 					+ 1));
// 		if (!joined)
// 		{
// 			free(after);
// 			free(quoted);
// 			exit(1);
// 		}
// 		joined[0] = '\0';
// 		strcat(joined, quoted);
// 		strcat(joined, after);
// 		free(after);
// 		free(quoted);
// 		return (joined);
// 	}
// 	return (quoted);
// }

// static char	*ft_strndup(const char *src, size_t n)
// {
// 	size_t	i;
// 	char	*dst;

// 	if (!src)
// 		return (NULL);
// 	dst = malloc(n + 1);
// 	if (!dst)
// 		return (NULL);
// 	i = 0;
// 	while (i < n)
// 	{
// 		dst[i] = src[i];
// 		i++;
// 	}
// 	dst[i] = '\0';
// 	return (dst);
// }

// void	word_token(const char *input, int *i, t_token *new_token)
// {
// 	char	*buffer;
// 	char	*tmp;
// 	char	*part;
// 	int		start;
// 	char	quote;

// 	buffer = malloc(1);
// 	if (!buffer)
// 		exit(1);
// 	buffer[0] = '\0';
// 	while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i]))
// 	{
// 		if (input[*i] == '\\' && input[*i + 1])
// 		{
// 			(*i)++;
// 			part = malloc(2);
// 			if (!part)
// 				exit(1);
// 			part[0] = input[*i];
// 			part[1] = '\0';
// 			tmp = ft_strjoin(buffer, part);
// 			free(buffer);
// 			free(part);
// 			buffer = tmp;
// 			(*i)++;
// 		}
// 		else if (input[*i] == '\'' || input[*i] == '"')
// 		{
// 			quote = input[*i];
// 			(*i)++;
// 			start = *i;
// 			while (input[*i] && input[*i] != quote)
// 			{
// 				if (quote == '"' && input[*i] == '\\' && input[*i + 1])
// 				{
// 					if (*i > start)
// 					{
// 						part = ft_strndup(input + start, (*i) - start);
// 						tmp = ft_strjoin(buffer, part);
// 						free(buffer);
// 						free(part);
// 						buffer = tmp;
// 					}
// 					(*i)++;
// 					part = malloc(2);
// 					if (!part)
// 						exit(1);
// 					part[0] = input[*i];
// 					part[1] = '\0';
// 					tmp = ft_strjoin(buffer, part);
// 					free(buffer);
// 					free(part);
// 					buffer = tmp;
// 					(*i)++;
// 					start = *i;
// 				}
// 				else
// 					(*i)++;
// 			}
// 			if (*i > start)
// 			{
// 				part = ft_strndup(input + start, (*i) - start);
// 				tmp = ft_strjoin(buffer, part);
// 				free(buffer);
// 				free(part);
// 				buffer = tmp;
// 			}
// 			if (input[*i] == quote)
// 				(*i)++;
// 		}
// 		else
// 		{
// 			start = *i;
// 			while (input[*i] && !is_space(input[*i]) && !is_operator(input[*i])
// 				&& input[*i] != '\'' && input[*i] != '"' && input[*i] != '\\')
// 				(*i)++;
// 			if (*i > start)
// 			{
// 				part = ft_strndup(input + start, (*i) - start);
// 				tmp = ft_strjoin(buffer, part);
// 				free(buffer);
// 				free(part);
// 				buffer = tmp;
// 			}
// 		}
// 	}
// 	new_token->type = T_WORD;
// 	new_token->value = buffer;
// 	if (!new_token->value)
// 		exit(1);
// }

// void	tokenizer(const char *input, int *i, t_token *new_token)
// {
// 	if (input[*i] == '|')
// 		pipe_token(input, i, new_token);
// 	else if (input[*i] == '<')
// 		redir_less_token(input, i, new_token);
// 	else if (input[*i] == '>')
// 		redir_great_token(input, i, new_token);
// 	else if (input[*i] == '&' && input[*i + 1] == '&')
// 	{
// 		new_token->type = T_AND;
// 		new_token->value = ft_strdup("&&");
// 		*i += 2;
// 	}
// 	else if (input[*i] == '&')
// 	{
// 		fprintf(stderr, "Error: '&' is not a valid token\n");
// 		exit(1);
// 	}
// 	else if (input[*i] == '(' || input[*i] == ')')
// 		parenthesis_token(input, i, new_token);
// 	else
// 		word_token(input, i, new_token);
// }

// void	head_or_tail(t_token **head, t_token **tail, t_token *new_token)
// {
// 	if (!*head)
// 	{
// 		*head = new_token;
// 		*tail = new_token;
// 	}
// 	else
// 	{
// 		(*tail)->next = new_token;
// 		*tail = new_token;
// 	}
// }

// t_token	*lexer(const char *input)
// {
// 	int		i;
// 	t_token	*new_token;
// 	t_token	*head;
// 	t_token	*tail;

// 	i = 0;
// 	head = NULL;
// 	tail = NULL;
// 	while (input[i] != '\0')
// 	{
// 		while (input[i] && is_space(input[i]))
// 			i++;
// 		if (input[i] == '\0')
// 			break ;
// 		new_token = malloc(sizeof(t_token));
// 		if (!new_token)
// 			exit(1);
// 		new_token->next = NULL;
// 		tokenizer(input, &i, new_token);
// 		head_or_tail(&head, &tail, new_token);
// 	}
// 	return (head);
// }

// void	free_tokens(t_token *token)
// {
// 	t_token	*tmp;

// 	while (token)
// 	{
// 		tmp = token;
// 		token = token->next;
// 		free(tmp->value);
// 		free(tmp);
// 	}
// }

// int	main(void)
// {
// 	char	buffer[1024];
// 	t_token	*tokens;
// 	t_token	*tmp;

// 	printf("minishell$ ");
// 	if (!fgets(buffer, sizeof(buffer), stdin))
// 	{
// 		perror("fgets");
// 		return (1);
// 	}
// 	buffer[strcspn(buffer, "\n")] = '\0';
// 	tokens = lexer(buffer);
// 	tmp = tokens;
// 	while (tmp)
// 	{
// 		printf("Token: %s, value: %d\n", tmp->value, tmp->type);
// 		tmp = tmp->next;
// 	}
// 	free_tokens(tokens);
// 	return (0);
// }
