/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   override_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:33:58 by apierret          #+#    #+#             */
/*   Updated: 2025/06/17 22:43:58 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "expand.h"
#include "utils.h"

static t_error	calculate_len(size_t *len, char *base, t_list *vars, char **env)
{
	size_t	size;
	t_vref	*var;
	char	*var_content;
	t_error	error;

	if (len == NULL || base == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	size = ft_strlen(base);
	while (vars != NULL)
	{
		var = vars->content;
		error = ERR_IMPLEMENTATION;
		if (error != ERR_NONE)
			return (error);
		size += ft_strlen(var_content);
		size -= ft_strlen(var->str);
		vars = vars->next;
	}
	return (*len = size, ERR_NONE);
}

static void	str_append_n(char *dst, const char *src, size_t n, size_t dstsize)
{
	size_t	dest_len;
	size_t	i;

	if (dst == NULL || src == NULL || n == 0 || dstsize == 0)
		return ;
	dest_len = ft_strlen(dst);
	i = 0;
	while (i + dest_len < dstsize - 1 && src[i] != '\0' && i < n)
	{
		dst[i + dest_len] = src[i];
		i++;
	}
	dst[i + dest_len] = '\0';
}

static t_error	process_override(char **strs, size_t len, t_list *vars,
		char **env)
{
	t_vref	*var;
	char	*var_content;
	size_t	i;
	t_error	error;

	(void) env;
	i = 0;
	while (vars != NULL)
	{
		var = vars->content;
		error = ERR_IMPLEMENTATION;
		if (error != ERR_NONE)
			return (free(strs[1]), error);
		str_append_n(strs[1], strs[0] + i, var->index - i, len + 1);
		i = var->index;
		ft_strlcat(strs[1], var_content, len +1);
		i += ft_strlen(var->str);
		vars = vars->next;
	}
	ft_strlcat(strs[1], strs[0] + i, len +1);
	return (ERR_NONE);
}

t_error	override_vars(char **output, char *base, t_list *vars, char **env)
{
	char		*strs[2];
	size_t		len;
	t_error		error;

	if (output == NULL || base == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	if (vars == NULL)
		return (*output = base, ERR_NONE);
	error = calculate_len(&len, base, vars, env);
	if (error != ERR_NONE)
		return (error);
	strs[1] = ft_calloc(len +1, sizeof(char));
	if (strs[1] == NULL)
		return (ERR_ALLOCATION);
	strs[0] = base;
	error = process_override(strs, len, vars, env);
	if (error != ERR_NONE)
		return (free(strs[1]), error);
	return (*output = strs[1], ERR_NONE);
}
