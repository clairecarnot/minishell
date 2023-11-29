/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:27:50 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/27 15:29:03 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/export.h"
#include "../libft/libft.h"

/*
Error if the content is empty ou est un special char
Check that the 1rst character of the variable name is a letter (upper or lower)
Check that there's not a special character in the entire name (before the '=')
*/
int	error_exp(char *content)
{
	//the message error = bash: export: `8hey=hey': not a valid identifier
	int	i;

	i = -1;
	if (!content[0])
		return (printf("ominishell: export: `': not a valid identifier\n"), 1);
	if (content[0] && content[0] == '-' && content[1])
		return (printf("oominishell: export: %c%c: invalid option\n", content[0], content[1]), 1);
	if (content[0] && (content[0] < 'A' || (content[0] > 'Z' && content[0] < 'a') || \
	content[0] > 'z') && content[0] != '_')
		return (printf("ooominishell: export: `%s': not a valid identifier\n", content), 1);
	while (content[++i] != '=' && content[i])
	{
		if ((content[i] < '0' || (content[i] > '9' && content[i] < 'A') || \
		(content[i] > 'Z' && content[i] < 'a') || content[i] > 'z') && content[i] != '_')
			return (printf("oooominishell: export: `%s': not a valid identifier\n", content), 1);
	}
	return (0);
}

/*
Check si le character avant '=' est un ' '
*/
int	error_exp_spaces(char *content)
{
	int	i;

	i = 0;
	while (content[i] && content[i++] != '=')
	if (content[i - 1] == ' ')
		return (printf("minishell: export: `%s': not a valid identifier\n", content), 1);
	return (0);
}

/*
Cas ou il n'y a pas de '=', nom de variable seulement ajoutee a exp dans l'ordre ASCII
*/
int	has_equal(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (content[i] == '=')
			return (1);
		i++;
	}
	return (0);
}
/*
Ajoute les quotes autour du contenu de la variable poour l'ajout a exp
*/
char	*add_qvar(char *content)
{
	int		i;
	int		j;
	char	*cpy_content;
	
	i = 0;
	j = 0;
	cpy_content = malloc(ft_strlen(content) + 3 * sizeof(char));
	if (!cpy_content)
		return (NULL);
	while (content[i] && content[i] != '=')
		cpy_content[j++] = content[i++];
	i++;
	cpy_content[j++] = '=';
	cpy_content[j++] = '\"';
	while (content[i])
		cpy_content[j++] = content[i++];
	cpy_content[j++] = '\"';
	cpy_content[j] = '\0';
	return (cpy_content);
}
