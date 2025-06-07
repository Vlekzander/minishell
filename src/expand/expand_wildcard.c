/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:32:08 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:41:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expand.h"
#include "utils.h"

t_error	expand_wildcard(t_list **filtered_files, char *base)
{
	t_pattern	*pattern;
	t_list		*files;
	t_list		*filtered;
	t_error		error;

	if (filtered_files == NULL || base == NULL)
		return (ERR_IMPLEMENTATION);
	filtered = NULL;
	error = extract_pattern(&pattern, base);
	if (error != ERR_NONE)
		return (error);
	if (pattern != NULL)
	{
		error = scan_dir(&files, ".");
		if (error != ERR_NONE)
			return (free_pattern(pattern), error);
		error = globbing(&filtered, files, pattern);
		if (error != ERR_NONE)
			return (free_pattern(pattern), ft_lstclear(&files, free), error);
		ft_lstclear(&files, free);
	}
	return (*filtered_files = filtered, free_pattern(pattern), ERR_NONE);
}
