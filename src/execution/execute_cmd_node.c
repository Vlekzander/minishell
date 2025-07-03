/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 12:01:37 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "execution.h"
#include "expand.h"
#include "lexer.h"
#include "utils.h"

t_error	execute_cmd_node(t_ast *node, t_hash_table *env)
{
	t_error	err;
	char	*executable;
	char	**arr;

	if (node == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = expand_list(&node->command_args, env);
	if (err.id != ERR_NONE)
		return (err);
	ft_lstiter(node->command_args, (void *) remove_str_quotes);
	err = find_executable(&executable, node->command_args->content, env);
	if (err.id != ERR_NONE)
		return (err);
	err = prepare_arguments(&arr, node->command_args);
	if (err.id != ERR_NONE)
		return (free(executable), err);
	free(executable);
	free_ddarray((void **) arr);
	return (error(ERR_NONE, NULL));
}
