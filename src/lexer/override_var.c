/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   override_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:33:58 by apierret          #+#    #+#             */
/*   Updated: 2025/06/03 00:17:04 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "lexer_internal.h"
#include "utils.h"

t_error	override_var(char **output, char *base, t_varpos *var, char **env)
{
	char	*str_one;
	char	*str_two;
	char	*var_content;
	t_error	error;

	if (output == NULL || base == NULL || var == NULL || var->str == NULL
		|| env == NULL)
		return (ERR_IMPLEMENTATION);
	str_one = ft_strndup(base, var->index);
	if (str_one == NULL)
		return (ERR_ALLOCATION);
	error = get_var(&var_content, env, var->str +1);
	if (error != ERR_NONE)
		return (free(str_one), error);
	if (var_content == NULL)
		var_content = "";
	str_two = ft_strjoin(str_one, var_content);
	if (str_two == NULL)
		return (free(str_one), ERR_ALLOCATION);
	free(str_one);
	str_one = ft_strjoin(str_two, base + var->index + ft_strlen(var->str));
	if (str_one == NULL)
		return (ERR_ALLOCATION);
	free(str_two);
	return (*output = str_one, ERR_NONE);
}
