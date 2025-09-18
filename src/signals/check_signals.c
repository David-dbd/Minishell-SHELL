/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 02:47:16 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/18 05:41:37 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int check_signals(t_shell *data, char **input)
{
	if (g_signal[0] == SIG_INTERACTIVE)
	{
		if (g_signal[1] == SIGINT)
		{
			data->exit_code = EXIT_CTRL_C;
			g_signal[1] = VOID;
			return (RECIVED_SIGNAL);
		}
	}
	else if (g_signal[0] == SIG_HEREDOC)
	{
		if (g_signal[1] == SIGINT)
		{
			data->exit_code = EXIT_CTRL_C;
			g_signal[1] = VOID;
			return (RECIVED_SIGNAL);
		}
	}
	return (VOID);
}
