/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/08/05 14:19:28 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "redirs.h"

t_error	execute_redir_node(t_ast *node, t_hash_table *env)
{
	t_error	err;
	t_ret	ret;

	if (node == NULL || env == NULL || node->type != NODE_REDIR)
		return (error(ERR_IMPLEMENTATION, NULL));
	ret = (t_ret){0};
	ret.type = RET_VALUE;
	err = prepare_redirs(node->redirs, env);
	if (err.id != ERR_NONE)
		return (ret.redir_error = 1, node->exit_code = get_exit_code(ret, err),
			err);
	err = handle_redirs(node->redirs, -1, -1);
	if (err.id != ERR_NONE)
		return (ret.redir_error = 1, node->exit_code = get_exit_code(ret, err),
			err);
	return (error(ERR_NONE, NULL));
}
