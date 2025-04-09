/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:23:08 by apierret          #+#    #+#             */
/*   Updated: 2025/04/09 13:48:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <munit.h>

#include "parser.h"
#include "utils.h"

t_error	parse_ast(t_ast **ast, t_list *tokens)
{
	if (ast == NULL || tokens == NULL)
		return (ERR_IMPLEMENTATION);
	return (ERR_NONE);
}
