/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prelexer_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:19:15 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/23 16:52:07 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

// case : <>      <>  eof 
int	check_after_lessgreat(char *line, size_t i)
{
	while (line[i] && i <= ft_strlen(line))
	{
		if (line[i] == '<' && line[i + 1] == '>')
			return (printf("minishell: \
syntax error near unexpected token `<>'\n"), 1);
		i++;
	}
	return (0);
}

// <>
//case : <>         eof
int	check_redir_beforelex(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] && i <= ft_strlen(line))
	{
		if (line[i] == '<' && line[i + 1] && line[i + 1] == '>')
		{
			while (line[i] == ' ')
				i++;
			if (!line[i + 2])
				return (printf("minishell: \
syntax error near unexpected token `newline'\n"), 1);
			else if (check_after_lessgreat(line, i + 2))
				return (1);
		}
		if (line[i] == '<' && line[i + 1] && line[i + 1] == '|')
			return (printf("minishell: \
syntax error near unexpected token `|'\n"), 1);
		if (line[i] == '<' && line[i + 1] && line[i + 1] == '&')
			return (printf("minishell: \
syntax error near unexpected token `|'\n"), 1);
		i++;
	}
	return (0);
}

int	check_quotes(char *str)
{
	int		qflag;
	int		i;
	char	c;

	qflag = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] && (str[i] == '\'' || str[i] == '\"'))
		{
			qflag = 1;
			c = str[i];
			i++;
			while (str[i] && str[i] != c)
				i++;
			if (str[i] && str[i] == c)
				qflag = 0;
			else if (!str[i])
				break ;
		}
	}
	return (qflag);
}

int	check_error_prelexer(char *line)
{
	if (check_quotes(line) != 0)
		return (printf("quotes error\n"), 1);
	if (check_redir_beforelex(line))
		return (1);
	return (0);
}
