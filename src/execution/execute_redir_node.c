/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/06/24 18:34:08 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

t_error	execute_redir_node(t_ast *node, t_hash_table *env)
{
	if (node == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	return (error(ERR_IMPLEMENTATION, NULL));
}
