/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 04:19:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/04 23:56:55 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	loop_heredoc(t_shell *data, t_redir *redir, int *pipe_fd, char *delimiter)
{
	char	*tmp;
	char	*line;
	int		n_line;

	n_line = 0;
	while (42)
	{
		setup_signals_heredoc();
		if (isatty(fileno(stdin)))
			line = ic_readline("> ");
		else // esto es del test se puede borrar y debe borrarse con la
		{
			tmp = get_next_line(fileno(stdin));
			if (!tmp)
				break ;
			line = ft_strtrim(tmp, "\n");
			free(tmp);
		}
		if (!line)
		{
			ft_printf_fd(STDERR, ERR_HEREDOC_EOF, n_line, delimiter);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (check_signals(data, redir, line, pipe_fd))
			return (ERROR);
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
		n_line++;
	}
	return (OK);
}

int	get_heredoc(t_shell *data, t_redir *redir, char *delimiter)
{
	int		pipe_fd[2];

	if (pipe(pipe_fd) == ERROR)
	{
		exit_error(data, ERR_PIPE, EXIT_FAILURE);
		return (ERROR);
	}
	if (loop_heredoc(data, redir, pipe_fd, delimiter) == ERROR)
		return (setup_signals_interactive(), ERROR);
	setup_signals_interactive();
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
