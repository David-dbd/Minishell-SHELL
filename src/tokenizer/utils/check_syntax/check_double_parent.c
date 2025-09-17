/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_double_parent.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:14:29 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/09/17 19:15:28 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static void	put_space_between(t_shell *d, char **str, char **tmp, char **space)
{
	*space = ft_strjoin(*str, " ");
	free (*str);
	if (!*space)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	*tmp = *space;
}

static char	*msg_construction(t_shell *d, t_token *tokens, int start, int len)
{
	char	*str;
	char	*tmp;
	char	*space;

	tmp = ft_strdup("");
	if (!tmp)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	while (start < len)
	{
		str = ft_strjoin(tmp, tokens[start].value);
		if (!str)
		{
			if (tmp)
				free (tmp);
			exit_error(d, ERR_MALLOC, EXIT_FAILURE);
		}
		free (tmp);
		tmp = NULL;
		if (start + 1 < len)
			put_space_between(d, &str, &tmp, &space);
		else
			tmp = str;
		start++;
	}
	return (str);
}

static int	ignore_till_paren_close(t_prompt *prompt, t_token *tokens, int i)
{
	while (i + 1 < prompt->n_tokens && !(tokens[i].type == PAREN_CLOSE
			&& tokens[i + 1].type == PAREN_CLOSE))
		i++;
	return (i);
}

int	check_double_parent(t_shell *data, t_token *tokens, t_prompt *p)
{
	char	*str;
	int		i;
	int		start;

	i = 0;
	while (i < p->n_tokens)
	{
		if (tokens[i].type == PAREN_OPEN && tokens[i + 1].type == PAREN_OPEN)
		{
			i += 2;
			start = i;
			i = ignore_till_paren_close(p, tokens, i);
			if (i + 1 < p->n_tokens && tokens[i].type == PAREN_CLOSE
				&& tokens[i + 1].type == PAREN_CLOSE)
			{
				str = msg_construction(data, tokens, start, i);
				if (!str)
					exit_error(data, ERR_MALLOC, EXIT_FAILURE);
				syntax_error(data, ERR_DOUBLE_PARENT, EXIT_USE, str, str);
				free (str);
				return (SYNTAX_ERROR);
			}
		}
		i++;
	}
	return (SUCCESS);
}
