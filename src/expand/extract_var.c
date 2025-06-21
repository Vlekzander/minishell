/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:38:03 by apierret          #+#    #+#             */
/*   Updated: 2025/06/21 16:04:10 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"
#include "utils.h"

static t_error	scan_next_var(char *str, int *index, int *len, int ignore_quote)
{
	char	quote;
	int		i;

	if (str == NULL || index == NULL || len == NULL)
		return (ERR_IMPLEMENTATION);
	*index = -1;
	*len = 0;
	quote = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if ((quote != '\'' || ignore_quote) && str[i] == '$')
			*index = i;
		if (is_quote(str[i]) && (quote == 0 || quote == str[i]))
			quote = handle_quote(str[i], quote);
		if (*index != -1 && *len == 1 && str[i] == '?')
			(*len)++;
		if (*index != -1 && ((*len == 1 && !ft_isalpha(str[i]) && str[i] != '_')
				|| (*len > 1 && !ft_isalnum(str[i]) && str[i] != '_')))
			break ;
		if (*index != -1)
			(*len)++;
		i++;
	}
	return (ERR_NONE);
}

t_error	extract_var(t_vref **varpos, char *str, int ignore_quote)
{
	t_error	error;
	t_vref	*var;
	int		len;

	if (varpos == NULL || str == NULL)
		return (ERR_IMPLEMENTATION);
	var = create_vref();
	if (var == NULL)
		return (*varpos = NULL, ERR_ALLOCATION);
	len = 0;
	error = scan_next_var(str, &var->index, &len, ignore_quote);
	if (error != ERR_NONE)
		return (error);
	if (var->index != -1 && len > 1)
		var->str = ft_strndup(str + var->index, len);
	if (var->index != -1 && len > 1 && var->str == NULL)
		return (*varpos = NULL, ERR_ALLOCATION);
	return (*varpos = var, ERR_NONE);
}
