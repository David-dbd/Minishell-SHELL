/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens_debug.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:50:14 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 12:56:44 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

const char	*g_type_names[] = {
	"START",
	"WORD",
	"GROUP",
	"PIPE",
	"REDIR_INPUT",
	"REDIR_OUTPUT",
	"REDIR_APPEND",
	"REDIR_HEREDOC",
	"COMMAND",
	"SCAPE",
	"BUILT_IN",
	"SEMICOLON",
	"SINGLE_QUOTE",
	"DOUBLE_QUOTE",
	"CMD_SUBSTITUTION",
	"PAREN_OPEN",
	"PAREN_CLOSE",
	"WILDCARD",
	"EXPANSION",
	"ASIGNATION",
	"BACKGROUND",
	"AND",
	"OR",
	"EXPAN_VALUE",
	"EXPAN_CMD",
	"NO_SPACE",
	"LOCAL",
	"ENV",
	"DELETE",
	"EXP",
	"INDIFERENT",
	"PLUS_ASIGNATION",
	"TEMP_ASIGNATION",
	"FILENAME",
	"DELIMITER",
	"SUBSHELL",
	"SHELL",
	"TEMP_PLUS_ASIGNATION,"
};

void	print_tokens_debug(t_prompt *prompt)
{
	int	i;

	i = 0;
	printf("------------------------------------------------\n");
	while (i < prompt->n_tokens)
	{
		if (prompt->tokens[i].value)
			printf("Token [%d]: '%s' (type: %s)\n", i, prompt->tokens[i].value,
				g_type_names[prompt->tokens[i].type]);
		i++;
	}
}
