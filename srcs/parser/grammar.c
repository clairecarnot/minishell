/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:37:21 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/02 17:23:25 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/struct.h"


t_ast	*new_node(t_ms *minishell, t_node_type type)
{
	t_ast	*new_ast;

	if (minishell->parser->cur_token->tok_nb == 0)
	{
		minishell->parser->root->type = type;
		minishell->parser->root->right = NULL;
		minishell->parser->root->left = NULL;
		minishell->parser->root->args = NULL;
		return (minishell->parser->root);
	}
	else
	{
		new_ast = ft_calloc(1, sizeof(t_ast));
		if (!new_ast)
			free_minishell(minishell, 1);
		new_ast->type = type;
		new_ast->right = NULL;
		new_ast->left = NULL;
		new_ast->args = NULL;
		return (new_ast);
	}
}

void	join_node(t_ast *root, t_ast *left, t_ast *right)
{
	if (!root)
		return ;
	root->left = left;
	root->right = right;
}

t_ast *read_words(t_ms *minishell, t_parser *parser)
{
	t_ast	*new_ast;
	char	*cmd;
	t_list	*new_arg;
	
	new_ast = new_node(minishell, COMMAND);
	while (parser->cur_token->type == T_WORD)
	{
		if (minishell->parser->cur_token->tok_nb == 3)
			cmd = NULL;
		else
			cmd = ft_strdup(parser->cur_token->value);
		if (!cmd)
			(free(new_ast), free_minishell(minishell, 1));
		new_arg = ft_lstnew(cmd);
		if (!new_arg)
			free_minishell(minishell, 1);
		ft_lstadd_back(&new_ast->args, new_arg);
		eat_token(parser, T_WORD);
	}
	return (new_ast);
}

t_ast	*pipeline(t_ms *minishell, t_parser *parser)
{
	t_ast	*left_node;
	t_ast	*right_node;
	t_ast	*center_node;

	left_node = NULL;
	right_node = NULL;
	center_node = NULL;	
	if (parser->cur_token->type == T_WORD)
	{
		left_node = read_words(minishell, parser);
		if (!left_node)
			return (NULL);
	}
	while (parser->cur_token->type == T_PIPE)//ex : ls | ls -l | ls 
	{
		center_node = new_node(minishell, PIPE);
		if (!center_node)
			return (NULL);// a proteger!
		eat_token(parser, T_PIPE);
		if (parser->cur_token->type == T_WORD)
		{
			right_node = read_words(minishell, parser);
			if (!right_node)
				return (NULL);
		}
		join_node(center_node, left_node, right_node);
		left_node = center_node;
	}
	return (left_node);
}

void	parse_grammar(t_ms *minishell, t_parser *parser)
{
	if (parser->cur_token->type == T_EOF)
		return ;
	parser->root = ft_calloc(1, sizeof(t_ast));
	if (!parser->root)
		free_minishell(minishell, 1);
	parser->root = pipeline(minishell, parser);
	if (!parser->root)
		return ;
}
