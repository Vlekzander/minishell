/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strbuilder_append_char.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 13:28:45 by apierret          #+#    #+#             */
/*   Updated: 2025/07/14 17:02:07 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "data.h"

int	strbuilder_append_char(t_strbuilder *sb, char c)
{
	char	str[2];

	if (sb == NULL)
		return (0);
	str[0] = c;
	str[1] = '\0';
	return (strbuilder_append(sb, str));
}
