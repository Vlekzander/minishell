/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:25:42 by apierret          #+#    #+#             */
/*   Updated: 2025/05/14 19:14:59 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "test.h"

int main(void) {
	t_test_result	results;

	cmocka_set_message_output(-1);
	results = execute_tests();
	return (results.failed);
}
