/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:37:21 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/02 12:43:18 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/ast.h"


t_ast	*new_node(t_node_type type)
{
	t_ast	*new_ast;

	new_ast = ft_calloc(1, sizeof(t_ast));
	if (!new_ast)
		return (NULL);
	new_ast->type = type;
	new_ast->right = NULL;
	new_ast->left = NULL;
	new_ast->args = NULL;
	return (new_ast);
}

void	join_node(t_ast *root, t_ast *left, t_ast *right)
{
	if (!root)
		return ;
	root->left = left;
	root->right = right;
}

t_ast *read_words(t_parser *parser)
{
	t_ast	*new_ast;
	char	*args;
	
	new_ast = new_node(COMMAND);
	if (!new_ast)
		return (NULL);
	args = ft_strdup(parser->cur_token->value);
	if (!args)
		return (free(new_ast), NULL); //verif protection
	while (parser->cur_token->type == T_WORD)
	{
		
		ft_lstadd_back(&new_ast->args, ft_lstnew(args));
		eat_token(parser, T_WORD);
	}
	return (new_ast);
}

t_ast	*pipeline(t_parser *parser)
{
	t_ast	*left_node;
	t_ast	*right_node;
	t_ast	*center_node;

	left_node = NULL;
	right_node = NULL;
	center_node = NULL;	
	if (parser->cur_token->type == T_WORD)
	{
		left_node = read_words(parser);
		if (!left_node)
			return (NULL);
	}
	while (parser->cur_token->type == T_PIPE)//ex : ls | ls -l | ls 
	{
		center_node = new_node(PIPE);
		if (!center_node)
			return (NULL);// a proteger!
		eat_token(parser, T_PIPE);
		if (parser->cur_token->type == T_WORD)
		{
			right_node = read_words(parser);
			if (!right_node)
				return (NULL);
		}
		join_node(center_node, left_node, right_node);
		left_node = center_node;
	}
	return (left_node);
}

t_parser	*parse_grammar(t_parser *parser)
{
	if (parser->cur_token->type == T_EOF)
		return (NULL);
	
	parser->root = pipeline(parser);
	if (!parser->root)
		return (NULL);
	return (parser);
}
