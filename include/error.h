/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:43:29 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 18:56:37 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

typedef enum e_error_code
{
	ERR_NONE,
	ERR_IMPLEMENTATION,
	ERR_SYNTAX,
	ERR_ALLOCATION,
	ERR_OPEN,
	ERR_CLOSE
}	t_error;

#endif