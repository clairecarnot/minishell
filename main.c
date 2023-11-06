/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 13:51:58 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/06 17:24:27 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"
#include "./include/parser.h"
#include "./include/struct.h"

char	*tok_to_str(t_token *token)
{
	if (token->type == T_WORD)
		return ("T_WORD");
	if (token->type == T_PIPE)
		return ("T_PIPE");
	if (token->type == T_AND_IF)
		return ("T_AND_IF");
	if (token->type == T_OR_IF)
		return ("T_OR_IF");
	if (token->type == T_LPAR)
		return ("T_LPAR");
	if (token->type == T_RPAR)
		return ("T_RPAR");
	if (token->type == T_LESS)
		return ("T_LESS");
	if (token->type == T_GREAT)
		return ("T_GREAT");
	if (token->type == T_DLESS)
		return ("T_DLESS");
	if (token->type == T_DGREAT)
		return ("T_DGREAT");	
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

void	print_lst(t_list *args_enter)
{
	t_list	*args;
	
	args = args_enter;
	while (args)
	{
		printf("args content = %s\n", (char *)args->content);
		args = args->next;
	}
}

void	visit_node(t_ast *root)
{
	if (!root)
		return ;
	visit_node(root->left);
	printf("node type : %s\n", node_to_str(root));
	if (root->type == COMMAND)
		print_lst(root->args);
	visit_node(root->right);
}

t_ms	*init_ms(void)
{
	t_ms	*minishell;

	minishell = ft_calloc(1, sizeof(t_ms));
	if (!minishell)
		return (NULL);
	minishell->lexer = NULL;
	minishell->parser = NULL;
	return (minishell);
}

int	main(int argc, char **argv)
{
	t_ms	*minishell;

	
	(void)argc;
	(void)argv;
	minishell = init_ms();
	if (!minishell)
		return (1);
	lexer(minishell, "bla <,< bla  | ,  || ls -l (nom_de_commande && command) hey  >>  hey ");
	if (!minishell->lexer)
		return (free_minishell(minishell, 1), 1);
	print_token_lst(minishell->lexer->token_lst);
	// parser(minishell, minishell->lexer);
	// if (!minishell->parser)
	// 	return (free_minishell(minishell, 1), 1);
	// visit_node(minishell->parser->root);
	free_minishell(minishell, 0);
	return (0);
}
