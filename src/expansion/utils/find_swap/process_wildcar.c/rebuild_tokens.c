/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rebuild_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:43:47 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/01 19:11:07 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

void	rebuild_tokens(t_shell *data, t_token *token, char **dirs, int n_dirs)
{
	int	i;
	int	id;
	int	original_size;

	i = 0;
	id = 0;
	original_size = data->prompt.n_tokens;
	while (i < n_dirs)
	{
		add_token(data, &data->prompt, dirs[i], WORD);
		i++;
	}
	reorder_tokens(data, token, original_size, dirs);
}
