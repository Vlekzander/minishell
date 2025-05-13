/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:25:42 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 23:39:26 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "test.h"

static void	add_results(t_test_result *dest, t_test_result src)
{
	if (dest == NULL)
		return ;
	dest->total += src.total;
	dest->successful += src.successful;
	dest->failed += src.failed;
}
int main(void) {
	t_test_result	global = {0};

	cmocka_set_message_output(-1);
	add_results(&global, tokenize_tests());
	add_results(&global, globbing_tests());
	add_results(&global, extract_pattern_tests());
	add_results(&global, parse_ast_tests());
	printf(GLOBAL_RESULt, global.successful, global.total);
	return (global.failed);
}
