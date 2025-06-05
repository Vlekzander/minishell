/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:32:39 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 20:33:40 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "test.h"

void	print_token_list(t_list *tokens)
{
	t_token *token;

	printf("[ ");
	while (tokens != NULL)
	{
		token = tokens->content;
		printf("(%d) %s, ", token->type, token->value);
		tokens = tokens->next;
	}
	printf("(null) ]\n");
}
