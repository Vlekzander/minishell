/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/05/08 19:17:51 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "lexer_internal.h"

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
