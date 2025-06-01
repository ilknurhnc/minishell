/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:23 by hbayram           #+#    #+#             */
/*   Updated: 2025/02/26 13:41:15 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int signal)
{
	(void)signal;
	printf("\n");
	rl_on_new_line();       // Readline kullanıyorsanız
	rl_replace_line("", 0); // Mevcut satırı temizle
	rl_redisplay();         // Yeni promptu yeniden göster
}

void	signal_init(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}
