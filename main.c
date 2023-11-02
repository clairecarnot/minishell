/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 13:51:58 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/02 14:23:57 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"
#include "./include/parser.h"
#include "./include/ast.h"

char	*tok_to_str(t_token *token)
{
	if (token->type == T_WORD)
		return ("T_WORD");
	if (token->type == T_PIPE)
		return ("T_PIPE");
	if (token->type == T_EOF)
		return ("T_EOF");
	else
		return ("UNKNOWN");
}

char*node_to_str(t_ast *node)
{
	if (node->type == PIPE)
		return ("PIPE");
	if (node->type == COMMAND)
		return ("COMMAND");
	else
		return ("UNKNOWN");
}

void	print_token_lst(t_token *token)
{
	t_token	*token_lst;
	
	token_lst = token;
	while (token_lst)
	{
		printf("%s est de type %s\n", (token_lst)->value, tok_to_str(token_lst));
		token_lst = (token_lst)->next_token;
	}
}

void	print_lst(t_list **args)
{
	while (*args)
	{
		printf("args content = %s\n", (char *)(*args)->content);
		*args = (*args)->next;
	}
}

void	visit_node(t_ast *root)
{
	if (!root)
		return ;
	visit_node(root->left);
	printf("node type : %s\n", node_to_str(root));
	if (root->type == COMMAND)
		print_lst(&root->args);
	visit_node(root->right);
}

int	main(int argc, char **argv)
{
	t_lexer		*lexer_res;
	t_parser	*parser_res;

	(void)argc;
	(void)argv;
	lexer_res = lexer("  ls |   lss l a"); //a proteger
	print_token_lst(lexer_res->token_lst);
	parser_res = parser(lexer_res); //a proteger
	visit_node(parser_res->root);
	return (0);
}