/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 02:02:04 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/18 04:42:18 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

volatile sig_atomic_t	g_signal[2];

void	setup_signals_interactive(void)
{
	g_signal[0] = SIG_INTERACTIVE;
	signal(SIGINT, handle_sigint_interative);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_child(void)
{
	g_signal[0] = SIG_CHILD;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void setup_signals_heredoc(void)
{
	g_signal[0] = SIG_HEREDOC;
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
