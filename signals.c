/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:23 by hbayram           #+#    #+#             */
/*   Updated: 2025/07/08 14:06:53 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_signal_exit = 0;

static void	handle_signal_case(void)
{
	if (g_signal_exit == 0 || g_signal_exit == 130)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_signal_exit == 2)
	{
		write(1, "\n", 1);
		rl_on_new_line();
	}
	else if (g_signal_exit == 1)
	{
		set_exit_status_code(130);
		exit(130);
	}
	else if (g_signal_exit == 5)
	{
		set_exit_status_code(130);
		write(1, "\n", 1);
		rl_on_new_line();
	}
}

void	signal_handler(int signal)
{
	(void)signal;
	handle_signal_case();
	set_exit_status_code(130);
	g_signal_exit = 130;
}

void	signal_init(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}
