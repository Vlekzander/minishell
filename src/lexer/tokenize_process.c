/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 22:39:38 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 17:06:48 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "lexer.h"

t_error	add_token(t_list **list, t_token_type type, char *value)
{
	t_token	*token;
	t_list	*node;

	if (list == NULL || value == NULL)
		return (ERR_IMPLEMENTATION);
	if (ft_strlen(value) > 0)
	{
		token = create_token(type, value);
		if (token == NULL)
			return (ERR_ALLOCATION);
		node = ft_lstnew(token);
		if (node == NULL)
			return (ERR_ALLOCATION);
		ft_lstadd_back(list, node);
	}
	ft_bzero(value, ft_strlen(value));
	return (ERR_NONE);
}

t_error	process_operator(t_list **tokens, char **input, char *buf)
{
	t_error	error;

	if (tokens == NULL || input == NULL || buf == NULL)
		return (ERR_IMPLEMENTATION);
	error = add_token(tokens, TK_WORD, buf);
	if (error != ERR_NONE)
		return (error);
	ft_strlcpy(buf, *input, 3);
	if (get_token_type(buf) == TK_WORD)
		buf[1] = '\0';
	*input += ft_strlen(buf);
	error = add_token(tokens, get_token_type(buf), buf);
	if (error != ERR_NONE)
		return (error);
	return (ERR_NONE);
}

t_error	process_separator(t_list **tokens, char *buf)
{
	t_error	error;

	if (tokens == NULL || buf == NULL)
		return (ERR_IMPLEMENTATION);
	error = add_token(tokens, TK_WORD, buf);
	if (error != ERR_NONE)
		return (error);
	return (ERR_NONE);
}
