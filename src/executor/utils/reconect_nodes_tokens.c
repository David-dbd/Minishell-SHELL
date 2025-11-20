/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reconect_nodes_tokens.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:40:47 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/20 19:40:51 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	reconect_nodes_tokens(t_shell *data, t_node *node, t_token *tokens)
{
	int	i;

	if (!node)
		return ;
	i = 0;
	while (i < data->prompt.n_tokens)
	{
		if (tokens[i].hash == node->token_hash)
		{
			node->token = &tokens[i];
			node->token->id = i;
			break ; //ya encontro el token correcto, así que hay que seguir con los demás
		}
		i++;
	}
	reconect_nodes_tokens(data, node->left, tokens);
	reconect_nodes_tokens(data, node->right, tokens);
}
