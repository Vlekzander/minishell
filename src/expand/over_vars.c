/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   over_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:15:26 by apierret          #+#    #+#             */
/*   Updated: 2025/06/22 20:44:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "expand.h"
#include "utils.h"

static t_error	calculate_len(size_t *len, char *base, t_list *vars,
					t_hash_table *env)
{
	size_t	size;
	t_vref	*var;
	char	*var_content;
	t_error	err;

	if (len == NULL || base == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	size = ft_strlen(base);
	while (vars != NULL)
	{
		var = vars->content;
		err = get_var(&var_content, env, var->str +1);
		if (err.id != ERR_NONE)
			return (err);
		size += ft_strlen(var_content);
		size -= ft_strlen(var->str);
		vars = vars->next;
	}
	return (*len = size, error(ERR_NONE, NULL));
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
		t_hash_table *env)
{
	t_vref	*var;
	char	*var_content;
	size_t	i;
	t_error	err;

	if (strs == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	i = 0;
	while (vars != NULL)
	{
		var = vars->content;
		err = get_var(&var_content, env, var->str +1);
		if (err.id != ERR_NONE)
			return (free(strs[1]), err);
		str_append_n(strs[1], strs[0] + i, var->index - i, len + 1);
		i = var->index;
		ft_strlcat(strs[1], var_content, len +1);
		i += ft_strlen(var->str);
		vars = vars->next;
	}
	ft_strlcat(strs[1], strs[0] + i, len +1);
	return (error(ERR_NONE, NULL));
}

t_error	over_vars(char **output, char *base, t_list *vars, t_hash_table *env)
{
	char		*strs[2];
	size_t		len;
	t_error		err;

	if (output == NULL || base == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (vars == NULL)
		return (*output = base, error(ERR_NONE, NULL));
	err = calculate_len(&len, base, vars, env);
	if (err.id != ERR_NONE)
		return (err);
	strs[1] = ft_calloc(len +1, sizeof(char));
	if (strs[1] == NULL)
		return (error(ERR_ALLOCATION, NULL));
	strs[0] = base;
	err = process_override(strs, len, vars, env);
	if (err.id != ERR_NONE)
		return (free(strs[1]), err);
	return (*output = strs[1], error(ERR_NONE, NULL));
}
