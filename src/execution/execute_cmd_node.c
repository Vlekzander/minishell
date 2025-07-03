/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:32:53 by apierret          #+#    #+#             */
/*   Updated: 2025/07/03 14:28:05 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "env.h"
#include "execution.h"
#include "expand.h"
#include "lexer.h"
#include "utils.h"

static t_error	prepare_envp(char ***envp, t_hash_table *env)
{
	char	**arr;
	char	*str;
	t_error	err;

	err = get_env(&str, env);
	if (err.id != ERR_NONE)
		return (err);
	arr = ft_split(str, '\n');
	if (arr == NULL)
		return (free(str), err);
	free(str);
	return (*envp = arr, error(ERR_NONE, NULL));
}

t_error	execute_cmd_node(t_ast *node, t_hash_table *env)
{
	char	*executable;
	char	**args;
	char	**envp;
	t_error	err;

	if (node == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = expand_list(&node->command_args, env);
	if (err.id != ERR_NONE)
		return (err);
	ft_lstiter(node->command_args, (void *) remove_str_quotes);
	err = find_executable(&executable, node->command_args->content, env);
	if (err.id != ERR_NONE)
		return (err);
	err = prepare_arguments(&args, node->command_args);
	if (err.id != ERR_NONE)
		return (free(executable), err);
	err = prepare_envp(&envp, env);
	if (err.id != ERR_NONE)
		return (free(executable), free_ddarray((void **) args), err);
	free(executable);
	free_ddarray((void **) args);
	free_ddarray((void **) envp);
	return (error(ERR_NONE, NULL));
}
