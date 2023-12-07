/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 16:43:45 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/07 12:33:00 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../libft/libft.h"

int	ft_strlen_equal(char *content)
{
	int	i;

	i = 0;
	while (content[i] && (content[i] != '=' && content[i] != '+'))// a verifier si marche bien
		i++;
	// dprintf(2, "i = %d\n", i);
	return (i);
}

int	find_plus(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '+')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strdup_noplus2(char *s)
{
	int		i;
	int		j;
	char	*dest;

	i = 0;
	j = 0;
	dest = malloc(sizeof(char) * (ft_strlen(s) - 1) + 1);//malloc -1 car on ne va pas copier le +
	if (!dest)
		return (0x0);
	while (s[i])
	{
		if (s[i] != '+')
			dest[j++] = s[i++];
		else
			i++;
	}
	dest[j] = '\0';
	return (dest);
}

char	*ft_strdup_noplus(char *s)
{
	char	*dest;
	int		i;

	i = 0;
	if (find_plus(s) == 1)
		dest = ft_strdup_noplus2(s);
	else
	{
		dest = malloc(sizeof(char) * ft_strlen(s) + 1);
		if (!dest)
			return (0x0);
		// dprintf(2, "no find plus\n");
		while (((char *)s)[i])
		{
			dest[i] = ((char *)s)[i];
			i++;
		}
		dest[i] = '\0';
	}
	// dprintf(2, "dest = %s\n", dest);
	return (dest);
}
