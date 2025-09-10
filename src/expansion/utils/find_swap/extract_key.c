/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_key.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:18:26 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/10 21:55:58 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

int	extract_key(t_shell *data, t_token *token, char **key_to_find, int phase)
{
	int	j;
	int	k;
	int	found;
	int	match;

	j = 0;
	k = 0;
	found = FALSE;
	match = copy_key(token->value, key_to_find);
	if (!match)
		return (FAILURE);
	found = find_key_in_lst(data, token, *key_to_find);
	if (found == ERROR)
		return (ERROR);
	if (!found && phase == 2)
	{
		token->value = ft_strdup("");// //cambiar el value por una str vacia
		// si no se haya CREO QUE ESTO ESTA MAL porque reemplaza todo el valu
		if (!token->value) // por ""y solo necesito que reemplaze la expansion
		{
			exit_error(data, ERR_MALLOC, EXIT_USE);
			return (ERROR);
		}
	}
	return (SUCCESS);
}
