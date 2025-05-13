/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:25:42 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 22:17:15 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int main(void) {
	int	failed;

	cmocka_set_message_output(-1);
	failed = 0;
	failed += tokenize_tests();
	failed += globbing_tests();
	return failed;
}
