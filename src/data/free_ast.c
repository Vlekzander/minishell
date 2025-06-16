/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:51:20 by apierret          #+#    #+#             */
/*   Updated: 2025/06/16 12:25:21 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"

void	free_ast(t_ast *ast)
{
	if (ast == NULL)
		return ;
	if (ast->type == NODE_COMMAND || ast->type == NODE_REDIR)
		ft_lstclear(&ast->redirs, (void (*)(void *)) free_redir);
	if (ast->type == NODE_COMMAND)
		ft_lstclear(&ast->command_args, free);
	else if (ast->type == NODE_PIPELINE)
		ft_lstclear(&ast->pipeline, (void (*)(void *)) free_ast);
	else if (ast->type == NODE_OR || ast->type == NODE_AND)
	{
		free_ast(ast->left);
		free_ast(ast->right);
	}
	else if (ast->type == NODE_GROUP)
		free_ast(ast->group);
	free(ast);
}
