/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:43:29 by apierret          #+#    #+#             */
/*   Updated: 2025/04/03 15:05:43 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

typedef enum e_error_code
{
	ERR_NONE,
	ERR_IMPLEMENTATION,
	ERR_ALLOCATION,
	ERR_SYNTAX
}	t_error;

void	print_error(char *source, t_error error);

#endif