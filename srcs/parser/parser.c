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

int	is_redirect(t_token *cur_tok)
{
	if (!cur_tok || (cur_tok->type < T_LESS || cur_tok->type > T_DGREAT))
		return (0);
	if (!cur_tok->next_token || cur_tok->next_token->type != T_WORD)
		return (0);
	return (1);
}

t_ast	*factor(t_ms *ms)
{
	t_ast	*node;

	if (!ms->cur_tok || (ms->cur_tok->type != T_WORD && (ms->cur_tok->type < T_LESS || ms->cur_tok->type > T_DGREAT)))
		return (NULL);
	node = new_node(ms, CMD);
	if (!node)
		return (NULL);
	while (ms->cur_tok && ms->cur_tok->type != T_EOF && (ms->cur_tok->type == T_WORD || is_redirect(ms->cur_tok)))
	{
		if (ms->cur_tok && ms->cur_tok->type == T_WORD)
		{
			if (!add_cmd_args(ms, node))
				return (free_root_ast(node), NULL);
		}
		if (is_redirect(ms->cur_tok))
		{
			if (!handle_red(ms, node))
				return (free_root_ast(node), NULL);
		}
	}
	return (node);
}

t_ast	*handle_par(t_ms *ms)
{
	t_ast	*node;

	node = NULL;
	eat_token(ms, T_LPAR);
	node = expr(ms);
	if (!node)
		return (NULL);
	eat_token(ms, T_RPAR);
	return (node);
}

t_ast	*term(t_ms *ms)
{
	t_ast	*node;
	t_ast	*middle_node;

	node = NULL;
	middle_node = NULL;
	if (ms->cur_tok && ms->cur_tok->type == T_LPAR)
	{
		node = handle_par(ms);
		if (!node)
			return (NULL);
	}
	else if (ms->cur_tok)
	{
		node = factor(ms);
		if (!node)
			return (NULL);
		while (ms->cur_tok && ms->cur_tok->type == T_PIPE)
		{
			middle_node = new_node(ms, PIPE);
			if (!middle_node)
				return (free_root_ast(node), NULL);
			eat_token(ms, T_PIPE);
			middle_node->right = factor(ms);
			if (!middle_node->right)
				return (free(middle_node), free_root_ast(node), NULL);
			middle_node->left = node;
			node = middle_node;
		}
	}
	return (node);
}

t_ast	*expr(t_ms *ms)
{
	t_ast	*node;
	t_ast	*middle_node;

	node = NULL;
	middle_node = NULL;
	node = term(ms);
	if (!node)
		return (NULL);
	while (ms->cur_tok->type == T_AND_IF || ms->cur_tok->type == T_OR_IF)
	{
		middle_node = new_node(ms, token_to_node(ms->cur_tok->type));
		if (!middle_node)
			return (free_root_ast(node), NULL);
		eat_token(ms, ms->cur_tok->type);
		middle_node->right = term(ms);
		if (!middle_node->right)
			return (free(middle_node), free_root_ast(node), NULL);
		middle_node->left = node;
		node = middle_node;
	}
	return (node);
}

//int	parse(t_ms *ms)
void	parse(t_ms *ms)
{
	if (!ms->cur_tok || ms->cur_tok->type == T_EOF)
		return ;
//		return (1);
	ms->root = expr(ms);
	if (!ms->root)
		return ;
//		return (0);
	if (ms->cur_tok->type != T_EOF)
	{
		printf("minishell: syntax error near unexpected token `%s'\n", ms->cur_tok->value);
		//error number = 2
		//return (0);
	}
//	return (1);
}