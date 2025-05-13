/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_glob_pattern.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:12:50 by apierret          #+#    #+#             */
/*   Updated: 2025/05/12 12:17:39 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"

void	free_glob_pattern(t_glob_pattern *pattern)
{
	if (pattern == NULL)
		return ;
	ft_lstclear(&pattern->infixes, free);
	free(pattern->prefix);
	free(pattern->suffix);
	free(pattern);
}
