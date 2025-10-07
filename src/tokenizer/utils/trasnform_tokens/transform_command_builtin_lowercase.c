/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_command_builtin_lowercase.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:40:49 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/06 20:00:11 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	normalize_token_to_lower(t_shell *data, t_token *token, char *og_value)
{
	int		i;
	char	*lc_value;

	i = 0;
	lc_value = ft_calloc(ft_strlen(og_value) + 1, sizeof(char));
	if (!lc_value)
		return ((void)exit_error(data, ERR_MALLOC, EXIT_FAIL));
	while(og_value[i] != '\0')
	{
		lc_value[i] = ft_tolower(og_value[i]);
		i++;
	}
	lc_value[i] = '\0';
	free(og_value);
	token->value = lc_value;
}

void	transform_command_built_lowercase(t_shell *d, t_prompt *p, t_token *t)
{
	int	i;

	i = 0;
	while  (i < p->n_tokens)
	{
		if (is_cmd_builtin_type(t[i].type))
			normalize_token_to_lower(d, &t[i], t->value);
		i++;
	}
}
