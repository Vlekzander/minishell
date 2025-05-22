/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:01:41 by apierret          #+#    #+#             */
/*   Updated: 2025/05/22 14:12:59 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "test_utils.h"

void		print_token_list(t_list *tokens)
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
