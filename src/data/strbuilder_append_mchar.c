/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strbuilder_append_mchar.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:36:27 by apierret          #+#    #+#             */
/*   Updated: 2025/07/30 20:38:45 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"

int	strbuilder_append_mchar(t_strbuilder *sb, char c, size_t count)
{
	char	str[2];
	size_t	i;

	if (sb == NULL)
		return (0);
	str[0] = c;
	str[1] = '\0';
	i = 0;
	while (i < count)
	{
		if (!strbuilder_append(sb, str))
			return (0);
		i++;
	}
	return (1);
}
