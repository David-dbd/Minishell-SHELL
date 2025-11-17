/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_int_matrix.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:22:37 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/17 16:39:09 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

void	ft_free_int_matrix(int ***matrix_ptr, int rows)
{
	int	i;
	int	**matrix;

	if (!matrix_ptr)
		return ;
	matrix = *matrix_ptr;
	if (!matrix || rows <= 0)
		return ;
	i = 0;
	while (i < rows)
	{
		free(matrix[i]);
		matrix[i] = NULL;
		i++;
	}
	free(matrix);
	*matrix_ptr = NULL;
}
