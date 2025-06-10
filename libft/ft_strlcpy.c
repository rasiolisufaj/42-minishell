/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raisufaj <raisufaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:18:50 by raisufaj          #+#    #+#             */
/*   Updated: 2024/11/13 12:36:30 by raisufaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	if (!size)
		return (ft_strlen(src));
	i = 0;
	while (src[i] && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen(src));
}

/*#include <string.h>
#include <stdio.h>
#define ft_strlcpy strlcpy //needs to be to the top for testing with real fx!

int	main(void)
{
    char src[] = "Hello, World!";
    char dest[6];

    size_t requiredLength = ft_strlcpy(dest, src, sizeof(dest));
	size_t realFX = strlcpy(dest, src, sizeof(dest));
    printf("Destination ====> %s \n", dest);
    printf("Required Buffer Size (Source Length) =====> %zu \n", requiredLength);
    printf("Real FX Required Buffer Size (Source Length) =====> %zu \n", realFX);
    return (0);
}
*/