/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:55:03 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/14 06:55:05 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	num;
	size_t	max;

	max = (size_t)-1;
	if (count > 0 && max / count < size)
		return (NULL);
	num = count * size;
	ptr = (void *)malloc(num);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, num);
	return (ptr);
}
