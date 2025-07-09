/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prefix_suffix_str.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 23:55:24 by apierret          #+#    #+#             */
/*   Updated: 2025/07/10 00:03:30 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "utils.h"

char	*prefix_suffix_str(char *str, char *prefix, char *suffix)
{
	size_t	len;
	char	*new_str;

	len = 0;
	if (str != NULL)
		len += ft_strlen(str);
	if (prefix != NULL)
		len += ft_strlen(prefix);
	if (suffix != NULL)
		len += ft_strlen(suffix);
	new_str = ft_calloc(len +1, sizeof(char));
	if (new_str == NULL)
		return (NULL);
	ft_strlcat(new_str, prefix, len +1);
	ft_strlcat(new_str, str, len +1);
	ft_strlcat(new_str, suffix, len +1);
	return (new_str);
}
