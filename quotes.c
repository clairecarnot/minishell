/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:05:30 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/15 17:14:03 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	check_quotes(char *str)
{
	int	qflag;
	int	i;
	char	c;

	qflag = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			qflag = 1;
			c = str[i];
			while (str[i] && str[i] != c)
				i++;
			if (str[i] && str[i] == c)
				qflag = 0;
		}
	}
	return (qflag);
}

int	main(int argc, char **argv)
{
	(void)argc;
	printf("flag = %d\n", check_quotes(argv[1]));
	return (0);
}
