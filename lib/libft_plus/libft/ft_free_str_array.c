/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_str_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:11:06 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/17 16:39:20 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

void	ft_free_str_array(char ***array_ptr)
{
	int		i;
	char	**array;

	if (!array_ptr || !*array_ptr)
		return ;
	array = *array_ptr;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	*array_ptr = NULL;
}
