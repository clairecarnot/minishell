#include "../../include/lexer.h"
#include "../../include/parser.h"

/*
 * new_node:
 * Cree et initialise une structure AST du type indique
 */

t_ast	*new_node(t_ms *ms, t_node_type type)
{
	t_ast	*new_ast;

	new_ast = ft_calloc(1, sizeof(t_ast));
	if (!new_ast)
		return (NULL);
	new_ast->type = type;
	new_ast->right = NULL;
	new_ast->left = NULL;
	new_ast->args = NULL;
	new_ast->subsh = 0;
	new_ast->redirs = NULL;
	new_ast->dol = ms->cur_tok->dol;
	return (new_ast);
}

t_ast	*factor(t_ms *ms, t_token *cur_tok)
{
	t_ast	*node;

	if (!cur_tok || (cur_tok->type != T_WORD && (cur_tok->type < T_LESS && cur_tok->type > T_DGREAT)))
		return (NULL);
	node = new_node(ms, token_to_node(cur_tok->type));
	if (!node)
		return (NULL);
	while (cur_tok && (cur_tok->type == T_WORD || (cur_tok->type >= T_LESS && cur_tok->type <= T_DGREAT)))
	{
		if (is_redir(cur_tok->type))
		{
			if (!handle_red(ms, node))
				return (free_root_ast(node), NULL);// a verifier
		}
		else
		{
			if (!add_cmd_args(ms, node))
				return (free_root_ast(node), NULL);// a verifier
		}
	}
}

t_ast	*handle_par(t_ms *ms, t_token *cur_tok)
{
	t_ast	*node;

	node = NULL;
	eat_token(ms, T_LPAR);
	node = expr(ms, cur_tok);
	// if (!node) //protec
	eat_token(ms, T_RPAR);
	return (node);
}

t_ast	*term(t_ms *ms, t_token *cur_tok)
{
	t_ast	*node;
	t_ast	*middle_node;

	node = NULL;
	middle_node = NULL;
	if (cur_tok->type == T_LPAR)
	{
		node = handle_par(ms, cur_tok);
		// if (!node) //protec
	}
	else
	{
		node = factor(ms, cur_tok);
		// if (!node) //protec
		while (cur_tok->type == T_PIPE)
		{
			middle_node = new_node(ms, token_to_node(cur_tok->type));
			// if (!middle_node) //protec
			eat_token(ms, T_PIPE);
			middle_node->left = node;
			middle_node->right = factor(ms, cur_tok);
			// if (!middle_node->right) //protec
			node = middle_node;
		}
	}
	return (node);
}

t_ast	*expr(t_ms *ms, t_token *cur_tok)
{
	t_ast	*node;
	t_ast	*middle_node;

	middle_node = NULL;
	node = term(ms, cur_tok);
	// if (!node) //a proteger
	while (cur_tok->type == T_AND_IF || cur_tok->type == T_OR_IF)
	{
		middle_node = new_node(ms, token_to_node(cur_tok->type));
		// if (!middle_node)//protec
		eat_token(ms, cur_tok->type);
		middle_node->left = node;
		middle_node->right = term(ms, cur_tok);
		// if(!middle_node->right) //protec
		node = middle_node;
	}
	return (node);
}

void	parse(t_ms *ms)
{
	if (ms->cur_tok == T_EOF || !ms->cur_tok)
		return ;
	ms->root = expr(ms->cur_tok);
	if (ms->cur_tok->type != T_EOF)
		printf("syntax error\n");// a completer
}