/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_cond.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:05:02 by apierret          #+#    #+#             */
/*   Updated: 2025/07/09 18:09:31 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "parser.h"

int	redir_cond(t_ast *node, t_token *token, t_token_type redir)
{
	return (((node != NULL && node->type != NODE_GROUP) || node == NULL)
		&& ((token != NULL && is_redirection(token) && redir == TK_NONE)
			|| (token != NULL && token->type == TK_WORD && redir != TK_NONE)));
}
