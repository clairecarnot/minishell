#include "./include/general.h"

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
	else
		return ("UNKNOWN");
}

void	print_token_lst(t_token *token)
{
	t_token	*token_lst;

	token_lst = token;
	while (token_lst)
	{
		dprintf(2, "%s est de type %s\n", (token_lst)->value, tok_to_str(token_lst));
		if (token_lst->dol)
			print_lst(token_lst->dol->d);
		if (token_lst->dol)
			print_lst(token_lst->dol->c);
		token_lst = (token_lst)->next_token;
	}
}

void	print_redirs(t_redirs *args_enter)
{
	t_redirs	*args;

	args = args_enter;
	while (args)
	{
		printf("redirs filename = %s\n", (char *)args->filename);
		args = args->next_redir;
	}
}

void	print_lst(t_list *args_enter)
{
	t_list	*args;
	int		i = 0;

	args = args_enter;
	while (args)
	{
//		dprintf(2, "args content = %s\n", (char *)args->content);
		dprintf(2, "int = %d\n", args->n);
		// dprintf(2, "%d\n", i);
		i++;
		args = args->next;
	}
}

void	visit_node(t_ast *root)
{
	if (!root)
		return ;
	visit_node(root->left);
	dprintf(2, "node type : %s subshell : %d\n", node_to_str(root), root->subsh);
	dprintf(2, "ARGS\n");
	if (root->args)
		print_lst(root->args);
	dprintf(2, "REDIRS\n");
	if (root->redirs)
		print_redirs(root->redirs);
	dprintf(2, "ARGS DOLS\n");
	if (root->dol)
	{
		print_lst(root->dol->d);
		print_lst(root->dol->c);
	}
	dprintf(2, "REDIRS DOLS\n");
	if (root->redirs)
	{
		if (root->redirs->dol)
		{
			print_lst(root->redirs->dol->d);
			print_lst(root->redirs->dol->c);
		}
	}
	dprintf(2, "WILDS\n");
	if (root->wil)
	{
		if (root->wil)
		{
			print_lst(root->wil->w);
		}
	}
	visit_node(root->right);
//	printf("exiting node %s\n", node_to_str(root));
}

void	print_tree(t_ast *root, int space)//temporaire
{
	int i;

	if (!root)
		return ;
	space += 10;
	print_tree(root->right, space);
	printf("\n");
	for (i = 10; i < space; i++)
		printf(" ");
	if (!root->parent)
		printf("%s", node_to_str(root));
	else
		printf("%s par:%s", node_to_str(root), node_to_str(root->parent));
	if (root->type == CMD)
		printf("%s\n", (char *)root->args->content);
		// printf("%s ss:%d dol:%d\n", (char *)root->args->content, root->subsh, root->dol);

	print_tree(root->left, space);
}
