/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/07/06 21:15:00 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "execution.h"
#include "redirs.h"

t_error	execute_redir_node(t_ast *node, t_hash_table *env)
{
	t_error	err;

	if (node == NULL || env == NULL || node->type != NODE_REDIR)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_redirs(node->redirs, env);
	if (err.id != ERR_NONE)
		return (node->exit_code = 1, err);
	err = handle_redirs(node->redirs, -1, -1);
	if (err.id != ERR_NONE)
		return (node->exit_code = 1, err);
	return (error(ERR_NONE, NULL));
}
