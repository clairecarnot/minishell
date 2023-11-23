#include "./include/lexer.h"
#include "./include/parser.h"
#include "./include/struct.h"
#include "./include/env.h"
#include "./include/signals.h"

char	*tok_to_str(t_token *token)//temporaire
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

char	*node_to_str(t_ast *node)//temporaire
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

void	print_token_lst(t_token *token)//temporaire
{
	t_token	*token_lst;

	token_lst = token;
	while (token_lst)
	{
		dprintf(1, "%s est de type %s\n", (token_lst)->value, tok_to_str(token_lst));
		token_lst = (token_lst)->next_token;
	}
}

void	print_redirs(t_redirs *args_enter)//temporaire
{
	t_redirs	*args;

	args = args_enter;
	while (args)
	{
		printf("redirs = %s\n", (char *)args->filename);
		args = args->next_redir;
	}
}

void	print_lst(t_list *args_enter)//temporaire
{
	t_list	*args;

	args = args_enter;
	while (args)
	{
//		printf("args content = %s\n", (char *)args->content);
		printf("%s\n", (char *)args->content);
		args = args->next;
	}
}

void	visit_node(t_ast *root)//temporaire
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

t_ms	*init_ms(char **env)
{
	t_ms	*minishell;

	minishell = ft_calloc(1, sizeof(t_ms));
	if (!minishell)
		return (NULL);
	minishell->lexer = NULL;
	minishell->cur_tok = NULL;
	minishell->root = NULL;
	minishell->env = NULL;
	minishell->exp = NULL;
	minishell->wkdir = NULL;
	minishell->old_wkdir = NULL;
	minishell->cur_node = NULL;
	if (init_env(minishell, env))
		free_minishell(minishell, 1);
	if (init_workdir(minishell))
		free_minishell(minishell, 1);
	if (init_exp(minishell))
		free_minishell(minishell, 1);
	return (minishell);
}

char	*display_prompt()
{
	char	*line;

	line = readline("minishell$ ");
	if (!line)
		return (NULL);
	add_history(line);
	return (line);
}

void	print_tree(t_ast *root, int space)
{
	int i;

	if (!root)
		return ;
	space += 10;
	print_tree(root->right, space);
	printf("\n");
	for (i = 10; i < space; i++)
		printf(" ");
	printf("%s ", node_to_str(root));
	if (root->type == CMD)
		printf("%s ss:%d\n", (char *)root->args->content, root->subsh);

	print_tree(root->left, space);
}

int	main(int argc, char **argv, char **env)
{
	t_ms	*minishell;

	(void)argc;
	(void)argv;
	minishell = init_ms(env);
	if (!minishell)
		return (1);
//	while (1)
//	{
//		preprompt_signals();
		minishell->line = display_prompt();
		if (!minishell->line)
			return (free_minishell(minishell, 0), 0);//verifier protec
//		postprompt_signals();
		if (!check_error_prelexer(minishell->line) && !lexer(minishell, minishell->line))// if no error
		{	
			if (!minishell->lexer)
				return (free_minishell(minishell, 1), 1);
			print_token_lst(minishell->lexer->token_lst);
			minishell->cur_tok = minishell->lexer->token_lst;
			parse(minishell);
			//print_tree(minishell->root, 0);
			//faire une fonction qui clean le parser pour la prochaine ligne
			//visit_node(minishell->root);
			// exec_export(minishell);
			g_exit_code = execute(ms);
		}
//	}
	visit_node(minishell->root);
	free_minishell(minishell, 0);
	return (0);
}
