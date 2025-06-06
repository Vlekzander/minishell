/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:22:29 by apierret          #+#    #+#             */
/*   Updated: 2025/06/06 10:31:38 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <bits/fcntl-linux.h>
#include <sys/stat.h>
#include "test_utils.h"

t_error		create_file(t_file file)
{
	int	ret;

	if (file.is_folder)
		ret = mkdir(file.name, file.perm);
	else
	{
		ret = open(file.name, O_CREAT | O_WRONLY, file.perm);
		if (ret != -1)
			close(ret);
	}
	if (ret == -1)
		return (ERR_ERRNO);
	return (ERR_NONE);
}

t_error		delete_file(t_file file)
{
	int	ret;
	chmod(file.name, 0777);
	if (file.is_folder)
		ret = rmdir(file.name);
	else
		ret = unlink(file.name);
	if (ret == -1)
		return (ERR_ERRNO);
	return (ERR_NONE);
}
