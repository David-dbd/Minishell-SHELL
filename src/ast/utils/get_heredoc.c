/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 04:19:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/11 07:30:45 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	loop_heredoc(t_shell *data, int *pipe_fd, char *delimiter)
{
	char	*line;
	int		n_line;

	n_line = 0;
	while(1)
	{
		setup_signals_heredoc();
		line = ic_readline("> ");
		if (!line)
		{
			ft_printf_fd(STDERR, ERR_HEREDOC_EOF, EXIT_FAIL, n_line, delimiter);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (check_signals(data, line, pipe_fd))
			return (ERROR);
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		n_line++;
	}
}

int	get_heredoc(t_shell *data, char *delimiter)
{
	int		pipe_fd[2];

	if (pipe(pipe_fd) == ERROR)
	{
		exit_error(data, ERR_PIPE, EXIT_FAILURE);
		return (ERROR);
	}
	if (loop_heredoc(data, pipe_fd, delimiter) == ERROR)
		return (ERROR);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
