/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 16:43:45 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:26:04 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
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
	while (content[i] && (content[i] != '=' && content[i] != '+'))
		i++;
	return (i);
}

int	find_p(char *s)
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

char	*ft_strdup_noplus2(t_ms *ms, t_cmd *cmd, char *s)
{
	int		i;
	int		j;
	char	*dest;

	i = 0;
	j = 0;
	dest = malloc(sizeof(char) * (ft_strlen(s) - 1 + 1));
	if (!dest)
		prefree_minishell_cmd(ms, cmd);
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

char	*ft_strdup_noplus(t_ms *ms, t_cmd *cmd, char *s)
{
	char	*dest;
	int		i;

	i = 0;
	if (find_p(s) == 1)
		dest = ft_strdup_noplus2(ms, cmd, s);
	else
	{
		dest = malloc(sizeof(char) * ft_strlen(s) + 1);
		if (!dest)
			prefree_minishell_cmd(ms, cmd);
		while (s[i])
		{
			dest[i] = s[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (dest);
}
