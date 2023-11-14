/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prelexer_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:19:15 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/14 17:01:23 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

int	check_after_lessgreat(char *line, size_t i)
{
	while (line[i] && i <= ft_strlen(line) )
	{
		if (line[i] == '<' && line[i + 1] == '>')// case : <>      <>  eof 
			return(printf("minishell: syntax error near unexpected token `<>'\n"), 1);
		else if (line[i] == '>')// case: <>    >  eof
			return(printf("minishell: syntax error near unexpected token `>'\n"), 1);
		else if (line[i] == '<')// case: <>    <  eof
			return(printf("minishell: syntax error near unexpected token `<'\n"), 1);
		i++;
	}
	return (0);
}

int	check_redir_beforelex(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] && i <= ft_strlen(line))
	{
		if (line[i] == '<' && line[i + 1] && line[i + 1] == '>')// <>
		{
			while (line[i] == ' ')
				i++;
			if (!line[i + 2])//case : <>eof
				return (printf("minishell: syntax error near unexpected token `newline'\n"), 1);
			else if (check_after_lessgreat(line, i + 2))
				return (1);
			else
				return (0);
		}
		i++;
	}
	return (0);
}

int	count_quotes(char *line)
{
	int	i;
	int	flag_single;// 39 = '
	int	flag_double;// 34 = "

	i = 0;
	flag_single = 0;
	flag_double = 0;
	while (line[i])
	{
		if (line[i] && line[i] == '\'')
		{
			flag_single += 1;
			i++;
			while (line[i] && flag_single == 1)
			{
				if (line[i] == '\'')
					flag_single -= 1;
				else
					i++;
			}
		}
		if (line[i] && line[i] == '\"')
		{
			flag_double += 1;
			i++;
			while (line[i] && flag_double == 1)
			{
				if (line[i] == '\"')
					flag_double -= 1;
				else
					i++;
			}
		}
		i++;
	}
	if (flag_single != 0 || flag_double != 0)
		return (printf("minishell: quotes error\n"), 1);//error
	return (0);
}

int	check_error_prelexer(char *line)
{
	if (count_quotes(line))
		return (1);
	if (check_redir_beforelex(line))
		return (1);
	// if (check_par(line))// a checker apres init lexer
	// 	return (1);
	return (0);
}