/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/07/01 14:05:30 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_error	tokenize(t_list **tokens, char *input)
{
	t_error	err;
	t_list	*tk_list;

	if (tokens == NULL || input == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	tk_list = NULL;
	err = process_input(&tk_list, input);
	if (err.id != ERR_NONE)
		return (err);
	return (*tokens = tk_list, error(ERR_NONE, NULL));
}
