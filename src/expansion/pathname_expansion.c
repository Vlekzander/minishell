/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:51:44 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 14:53:29 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expansion.h"
#include "utils.h"

static t_error	filter_files(t_list **filtered, t_pattern *pattern)
{
	t_list	*files;
	t_error	err;

	if (filtered == NULL || pattern == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = scan_dir(&files, ".");
	if (err.id != ERR_NONE)
		return (err);
	err = globbing(filtered, files, pattern);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&files, free), err);
	return (ft_lstclear(&files, free), error(ERR_NONE, NULL));
}

t_error	pathname_expansion(t_list **lst, char *str, char *mask, char *glob_ign)
{
	t_pattern	*pattern;
	t_list		*filtered;
	t_error		err;

	if (lst == NULL || str == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (glob_ign == NULL)
		glob_ign = ".*";
	err = extract_pattern(&pattern, str, mask);
	if (err.id != ERR_NONE)
		return (err);
	if (pattern == NULL)
		return (*lst = NULL, error(ERR_NONE, NULL));
	else
	{
		err = filter_files(&filtered, pattern);
		if (err.id != ERR_NONE)
			return (free_pattern(pattern), err);
	}
	free_pattern(pattern);
	return (*lst = filtered, error(ERR_NONE, NULL));
}
