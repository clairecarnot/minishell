/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 13:51:58 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/15 11:14:03 by mapoirie         ###   ########.fr       */
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
	if (token->type == T_NEWLINE)
		return ("T_NEWLINE");
	if (token->type == T_EOF)
		return ("T_EOF");
	else
		return ("UNKNOWN");
}

char	*node_to_str(t_ast *node)
{
	if (node->type == CMD)
		return ("CMD");
	if (node->type == PIPE)
		return ("PIPE");
	if (node->type == AND_IF)
		return ("AND_IF");
	if (node->type == OR_IF)
		return ("OR_IF");
	if (node->type == LPAR)
		return ("LPAR");
	if (node->type == RPAR)
		return ("RPAR");
	if (node->type == LESS)
		return ("LESS");
	if (node->type == GREAT)
		return ("GREAT");
	if (node->type == DLESS)
		return ("DLESS");
	if (node->type == DGREAT)
		return ("DGREAT");
	if (node->type == NEWLINE)
		return ("NEWLINE");
	if (node->type == END)
		return ("END");
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

void	print_redirs(t_redirs *args_enter)
{
	t_redirs	*args;
	
	args = args_enter;
	while (args)
	{
		printf("redirs = %s\n", (char *)args->filename);
		args = args->next_redir;
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
	printf("node type : %s subshell : %d\n", node_to_str(root), root->subsh);
	fflush(stdout);
	if (root->args)
		print_lst(root->args);
	if (root->redirs)
		print_redirs(root->redirs);
	visit_node(root->right);
//	printf("exiting node %s\n", node_to_str(root));
}

t_ms	*init_ms(void)
{
	t_ms	*minishell;

	minishell = ft_calloc(1, sizeof(t_ms));
	if (!minishell)
		return (NULL);
	minishell->lexer = NULL;
	// minishell->parser = NULL;
	minishell->cur_tok = NULL;
	return (minishell);
}

char	*display_prompt()
{
	char	*line;

	line = readline("minishell$ ");
	add_history(line);
	return (line);
}

void		btree_apply_by_level(t_ast *root);

int	main(int argc, char **argv)
{
	t_ms	*minishell;
	
	(void)argc;
	(void)argv;
	minishell = init_ms();
	if (!minishell)
		return (1);

	while (1)
	{
		minishell->line = display_prompt();
		if (!check_error_prelexer(minishell->line) && !lexer(minishell, minishell->line))// if no error
		{	
			if (!minishell->lexer)
				return (free_minishell(minishell, 1), 1);
			print_token_lst(minishell->lexer->token_lst);
			// minishell->cur_tok = minishell->lexer->token_lst;
			// parse(minishell);
			// visit_node(minishell->root);	
		}
	}
	// parser(minishell, minishell->lexer);
	// if (!minishell->parser)
	// 	return (free_minishell(minishell, 1), 1);
	visit_node(minishell->root);
	// btree_apply_by_level(minishell->root);
	free_minishell(minishell, 0);
	return (0);
}
