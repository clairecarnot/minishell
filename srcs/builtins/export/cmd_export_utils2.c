/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 16:43:45 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/12 16:03:29 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../libft/libft.h"

int	ft_slen(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

int	slen_equal(char *content)
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
	while (s[i] && s[i] != '=')
	{
		if (s[i] == '+')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strdup_noplus2(t_ms *ms, char *s)
{
	int		i;
	int		j;
	char	*dest;

	i = 0;
	j = 0;
	dest = malloc(sizeof(char) * (ft_strlen(s) - 1 + 1));// c'est verifie
	if (!dest)
	{
		ms->exit_code = 134;
		free_minishell(ms, 1);
	}
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

char	*ft_strdup_noplus(t_ms *ms, char *s)
{
	char	*dest;
	int		i;

	i = 0;
	if (find_plus(s) == 1)
		dest = ft_strdup_noplus2(ms, s);
	else
	{
		dest = malloc(sizeof(char) * ft_strlen(s) + 1);// c'est verifie
		if (!dest)
		{
			ms->exit_code = 134;
			free_minishell(ms, 1);
		}
		while (s[i])
		{
			dest[i] = s[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (dest);
}
