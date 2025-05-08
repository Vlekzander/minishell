/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:57:43 by apierret          #+#    #+#             */
/*   Updated: 2025/05/08 15:58:15 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

int	is_operator(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '<' && str[1] == '<')
		return (1);
	if (str[0] == '>' && str[1] == '>')
		return (1);
	if (str[0] == '|' && str[1] == '|')
		return (1);
	if (str[0] == '&' && str[1] == '&')
		return (1);
	if (str[0] == '<')
		return (1);
	if (str[0] == '>')
		return (1);
	if (str[0] == '|')
		return (1);
	if (str[0] == '(')
		return (1);
	if (str[0] == ')')
		return (1);
	return (0);
}

int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}
