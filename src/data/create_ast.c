/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:27:43 by apierret          #+#    #+#             */
/*   Updated: 2025/06/16 16:39:58 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

t_ast	*create_ast(t_node_type	type)
{
	t_ast	*ast;

	ast = ft_calloc(1, sizeof(t_ast));
	if (ast == NULL)
		return (NULL);
	ast->type = type;
	return (ast);
}
