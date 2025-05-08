/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/05/08 16:16:46 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "lexer.h"
#include "lexer_internal.h"

static void	process_input(t_list **tokens, char *input, char *buf)
{
	char	quote;
	size_t	i;

	quote = 0;
	i = 0;
	while (*input != '\0')
	{
		if (quote == 0 && (is_operator(input) || is_separator(*input)))
		{
			i = 0;
			if (is_operator(input))
			{
				process_operator(tokens, &input, buf);
				continue ;
			}
			process_separator(tokens, buf);
			input++;
			continue ;
		}
		if (is_quote(*input) && (quote == 0 || quote == *input))
			quote = handle_quote(*input, quote);
		buf[i++] = *input;
		input++;
	}
	add_token(tokens, TK_WORD, buf);
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
	char	*buf;

	if (tokens == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	buf = ft_calloc(ft_strlen(input) +1, sizeof(char));
	if (buf == NULL)
		return (ERR_ALLOCATION);
	process_input(tokens, input, buf);
	remove_tokens_quotes(*tokens);
	return (free(buf), ERR_NONE);
}
