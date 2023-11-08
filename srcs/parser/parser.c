/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/02 17:30:54 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

// t_parser	*init_parser(t_lexer *lexer)
// {
// 	t_parser	*parser;

// 	parser = ft_calloc(1, sizeof(t_parser));
// 	if (!parser)
// 		return (NULL);
// 	parser->lexer = lexer;
// 	parser->root = NULL;
// 	parser->cur_token = lexer->token_lst;
// 	return (parser);
// }

t_ast	*new_node(t_ms *minishell, t_node_type type)
{
	t_ast	*new_ast;

		new_ast = ft_calloc(1, sizeof(t_ast));
		if (!new_ast)
			free_minishell(minishell, 1);
		new_ast->type = type;
		new_ast->right = NULL;
		new_ast->left = NULL;
		new_ast->args = NULL;
		new_ast->subsh = 0;
		new_ast->redirs = NULL;
		return (new_ast);
}

t_ast	*handle_par(t_ms *ms)
{
	t_ast	*new_ast;

	while (ms->cur_tok && ms->cur_tok->type != T_RPAR)
	{
		if (ms->cur_tok->type == T_WORD || ms->cur_tok->type == T_LPAR)
			add_subtree(handle_cmd(ms), &new_ast);
		else if (ms->cur_tok->type >= T_PIPE && ms->cur_tok->type <= T_OR_IF)
			add_subtree(handle_op(ms), &new_ast);
	}
	if (ms->cur_tok->type != T_RPAR)
		printf("error"); //exit
	eat_token(ms, T_RPAR);
	new_ast->subsh = 1;
	return (new_ast);
}

int	is_redir(int type)
{
	if (type == T_LESS || type == T_GREAT || type == T_DLESS || type == T_DGREAT)
		return (1);
	return (0);
}

t_ast	*handle_cmd(t_ms *ms)
{
	t_ast	*new_ast;
	char	*cmd;
	t_list	*new_arg;
	t_redirs	*new_redir;

	if (ms->cur_tok->type == T_LPAR)
		return (handle_par(ms));
	new_ast = new_node(ms, token_to_node(ms->cur_tok->type));
	if (!new_ast)
		return (NULL); //protect?
	while (ms->cur_tok && ms->cur_tok->type != T_PIPE && ms->cur_tok->type != T_AND_IF
			&& ms->cur_tok->type != T_OR_IF && ms->cur_tok->type != T_EOF)
	{
		printf("ms->cur_tok->type = %s\n", tok_to_str(ms->cur_tok));
		if (is_redir(ms->cur_tok->type))
		{
			// printf("REDIR\n");
			if (ms->cur_tok->next_token != T_WORD)
				return (printf("error\n"), NULL); //a modif
			new_redir = redirs_new(ms->cur_tok);
			if (!new_ast->redirs)
				return (NULL); //protec
			redirs_add_back(&new_ast->redirs, new_redir);
			eat_token(ms, ms->cur_tok->type);
		}
		else
		{
			cmd = ft_strdup(ms->cur_tok->value);
			if (!cmd)
				(free(new_ast), free_minishell(ms, 1));  //verif protec
			new_arg = ft_lstnew(cmd);
			if (!new_arg)
				free_minishell(ms, 1); //verif protec
			ft_lstadd_back(&new_ast->args, new_arg);
			eat_token(ms, T_WORD);
		}
	}
	// printf("leaving cmd\n");
	return (new_ast);	
}

t_ast	*handle_op(t_ms *ms)
{
	t_ast	*new_ast;

	new_ast = new_node(ms, token_to_node(ms->cur_tok->type));
	if (!new_ast)
		return (NULL); //protect
	eat_token(ms, ms->cur_tok->type);
	//protection sur le next token?
	new_ast->right = handle_cmd(ms);
	return (new_ast);
}

void	add_subtree(t_ast *node, t_ast	**root)
{
	if (!node) // !root ?
		return ;
	node->left = *root;
	*root = node;
}

void	parse(t_ms *ms)
{
	while (ms->cur_tok && ms->cur_tok->type != T_EOF)
	{
		if (ms->cur_tok->type == T_WORD || ms->cur_tok->type == T_LPAR)
			add_subtree(handle_cmd(ms), &ms->root);
		else if (ms->cur_tok->type >= T_PIPE && ms->cur_tok->type <= T_OR_IF)
			add_subtree(handle_op(ms), &ms->root);
	}
}
