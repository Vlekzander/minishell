/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 23:40:48 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 23:41:42 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRS_H
# define REDIRS_H
# include "error.h"

t_error	open_file(int *fd, char *path, int write, int append);

#endif
