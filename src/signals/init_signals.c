/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:37:17 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/09 16:37:19 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include ".h"


/*Here we simply set or initialize the sigaction sa struct with 0 to clean it from
posible trash data it might have. Then we simply set the members we are going to use.
SA_RESTART will make the functions that do syscalls pick it up from were they
left it. If we didn't use it, then those functions would not pick it up and would
instead return an error. sa_handler simply sets the actual function's name we're
going to use for signal-event behaviour. SIGACTION simply returns either 0 or -1
depending on the STATE OF THE SIGNAL*/

int init_signals(void)
{
	struct sigaction sa;

    ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signal_handler;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT) == ERROR)
	{
        ft_printf_fd(STDR,"Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
    if (sigaction(SIGQUIT) == ERROR)
	{
		ft_printf_fd(STDR,"Error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return (0);
}