/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_empty.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 21:50:25 by apierret          #+#    #+#             */
/*   Updated: 2025/07/01 21:50:49 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"

int	str_empty(void *content)
{
	char	*str;

	str = content;
	return (ft_strlen(str) == 0);
}
