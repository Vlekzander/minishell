/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/05/08 18:15:42 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "lexer.h"
#include "lexer_internal.h"

static t_error	process_input(t_list **tokens, char *input)
{
	char	*buf;
	char	quote;
	t_error	error;
	size_t	i;

	if (tokens == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	buf = ft_calloc(ft_strlen(input) +1, sizeof(char));
	if (buf == NULL)
		return (ERR_ALLOCATION);
	quote = 0;
	i = 0;
	while (*input != '\0')
	{
		if (quote == 0 && (is_operator(input) || is_separator(*input)))
		{
			i = 0;
			if (is_operator(input))
			{
				error = process_operator(tokens, &input, buf);
				if (error != ERR_NONE)
					return (free(buf), ft_lstclear(tokens, (void *) free_token), error);
				continue ;
			}
			error = process_separator(tokens, buf);
			if (error != ERR_NONE)
				return (free(buf), ft_lstclear(tokens, (void *) free_token), error);
			input++;
			continue ;
		}
		if (is_quote(*input) && (quote == 0 || quote == *input))
			quote = handle_quote(*input, quote);
		buf[i++] = *input;
		input++;
	}
	error = add_token(tokens, TK_WORD, buf);
	return (free(buf), error);
}

static void	remove_tokens_quotes(t_list *tokens)
{
	t_token	*token;

	if (tokens == NULL)
		return ;
	while (tokens != NULL)
	{
		token = (t_token *) tokens->content;
		if (token != NULL)
			remove_str_quotes(token->value);
		tokens = tokens->next;
	}
}

t_error	tokenize(t_list **tokens, char *input)
{
	t_error	error;

	if (tokens == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	error = process_input(tokens, input);
	if (error != ERR_NONE)
		return (error);
	remove_tokens_quotes(*tokens);
	return (ERR_NONE);
}
