/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 13:51:58 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/02 10:27:23 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"
#include "./include/parser.h"


void	print_token_lst(t_token **token_lst)
{
	while (*token_lst)
	{
		printf("%s est de type %d\n", (*token_lst)->value, (*token_lst)->type);
		*token_lst = (*token_lst)->next_token;
	}
}

int	main(int argc, char **argv)
{
	t_lexer		*lexer_res;
	t_parser	*parser_res;

	(void)argc;
	(void)argv;
	lexer_res = lexer("  ls   lss");
	print_token_lst(&lexer_res->token_lst);
	parser_res = parser(lexer);
	return (0);
}