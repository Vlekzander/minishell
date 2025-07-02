/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 01:41:37 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "execution.h"
#include "expand.h"

t_error	execute_cmd_node(t_ast *node, t_hash_table *env)
{
	t_error	err;

	if (node == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = expand_list(&node->command_args, env);
	return (err);
}
