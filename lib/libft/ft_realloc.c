/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:29:21 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/03 18:41:43 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

void	*ft_realloc(void *old_ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (!old_ptr)
		return (ft_calloc(1, new_size));
	if (new_size == 0)
	{
		free(old_ptr);
		return (NULL);
	}
	new_ptr = ft_calloc(1, new_size);
	if (!new_ptr)
		return (NULL);
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	ft_memcpy(new_ptr, old_ptr, copy_size);
	free(old_ptr);
	old_ptr = NULL;
	return (new_ptr);
}
