/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:51:20 by apierret          #+#    #+#             */
/*   Updated: 2025/04/01 16:28:07 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"

static void	free_command(t_command *command)
{
	int	i;

	if (command == NULL)
		return ;
	free(command->path);
	if (command->args != NULL)
	{
		i = 0;
		while (command->args[i] != NULL)
			free(command->args[i++]);
		free(command->args);
	}
	if (command->redir.in != NULL)
		free(command->redir.in);
	if (command->redir.out != NULL)
		free(command->redir.out);
	if (command->redir.heredoc != NULL)
		free(command->redir.heredoc);
	free(command);
}

void	free_ast(t_ast *ast)
{
	if (ast == NULL)
		return ;
	if (ast->type == NODE_COMMAND)
		free_command(ast->command);
	else if (ast->type == NODE_PIPELINE)
		ft_lstclear(&ast->pipeline, (void (*)(void *)) free_command);
	else if (ast->type == NODE_OR || ast->type == NODE_AND)
	{
		free_ast(ast->left);
		free_ast(ast->right);
	}
	else if (ast->type == NODE_SUBSHELL)
		free_ast(ast->child);
	free(ast);
}
