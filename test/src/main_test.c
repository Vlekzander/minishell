/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:01:59 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 14:02:17 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int main(void) {
	t_test_result	results;

	cmocka_set_message_output(-1);
	results = execute_tests();
	return (results.failed);
}
