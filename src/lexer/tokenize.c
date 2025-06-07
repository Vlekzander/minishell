/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:22:02 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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

t_error	tokenize(t_list **tokens, char **env, char *input)
{
	t_error	error;
	t_list	*tk_list;

	if (tokens == NULL || env == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	tk_list = NULL;
	error = process_input(&tk_list, input);
	if (error != ERR_NONE)
		return (error);
	error = expand_tokens(&tk_list, env);
	if (error != ERR_NONE)
		return (ft_lstclear(&tk_list, (void *) free_token), error);
	remove_tokens_quotes(tk_list);
	return (*tokens = tk_list, ERR_NONE);
}
