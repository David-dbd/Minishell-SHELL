/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 04:19:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/16 21:33:22 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	save_heredoc_line(t_shell *data, t_redir *redir, char *line)
{
	t_list	*heredoc_line;

	heredoc_line = ft_lstnew(line);
	if (!heredoc_line)
	{
		free(line);
		return (exit_error(data, ERR_MALLOC, EXIT_FAILURE)); // leak no liberamos memoria anterior del arbol
	}
	ft_lstadd_back(&redir->heredoc_lines, heredoc_line);
	return (SUCCESS);
}

int	loop_heredoc(t_shell *data, t_redir *redir, char *delimiter)
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
			// return (ft_printf_fd(STDERR, ERR_STDIN), ERROR);
			tmp = get_next_line(fileno(stdin));
			if (!tmp)
				break ;
			line = ft_strtrim(tmp, "\n");
			free(tmp);
		}
		if (!line)
			return (ft_printf_fd(STDERR, ERR_HEREDOC_EOF, n_line, delimiter));
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (check_signals(data, redir, line))
			return (ERROR);
		save_heredoc_line(data, redir, line);
		n_line++;
	}
	return (OK);
}

int	get_heredoc(t_shell *data, t_redir *redir, char *delimiter, int expansion)
{
	t_list *heredoc_lines;

	if (expansion == TRUE)
		redir->expand = TRUE;
	if (loop_heredoc(data, redir, delimiter) == ERROR)
		return (setup_signals_interactive(), ERROR);
	setup_signals_interactive();
	return (SUCCESS);
}
