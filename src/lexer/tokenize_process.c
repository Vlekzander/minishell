/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 22:39:38 by apierret          #+#    #+#             */
/*   Updated: 2025/03/27 22:46:33 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "lexer.h"

void	add_token(t_list **list, t_token_type type, char *value)
{
	size_t	i;

	if (list == NULL || value == NULL)
		return ;
	if (ft_strlen(value) > 0)
		ft_lstadd_back(list, ft_lstnew(create_token(type, value)));
	i = 0;
	while (value[i] != '\0')
		value[i++] = '\0';
}

char	handle_quote(char c, char current)
{
	if (current == 0)
		return (c);
	if (current == c)
		return (0);
	return (current);
}

void	process_operator(t_list **tokens, char **input, char *buf)
{
	if (tokens == NULL || input == NULL || buf == NULL)
		return ;
	add_token(tokens, TK_WORD, buf);
	ft_strlcpy(buf, *input, 3);
	if (get_token_type(buf) == TK_WORD)
		buf[1] = '\0';
	*input += ft_strlen(buf);
	add_token(tokens, get_token_type(buf), buf);
}

void	process_separator(t_list **tokens, char *buf)
{
	if (tokens == NULL || buf == NULL)
		return ;
	add_token(tokens, TK_WORD, buf);
}
