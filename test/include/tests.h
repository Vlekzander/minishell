/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:34:27 by apierret          #+#    #+#             */
/*   Updated: 2025/04/02 16:35:27 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "munit.h"

#ifndef TESTS_H
# define TESTS_H

typedef struct s_case
{
	char* name;
	union
	{
		struct
		{
			char* input;
			char* expected_tokens[10];
		};
	};
} t_case;

t_case	*find_case(t_case cases[], const char *name);

MunitResult	tokenize_basic_tests(const MunitParameter params[], void* data);

#endif
