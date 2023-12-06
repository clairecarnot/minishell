#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "parser.h"
# include "struct.h"
# include "builtins.h"

//----------------------- old_parser.c -----------------------
// void		parse(t_ms *ms);
// void		add_subtree(t_ast *node, t_ast	**root);
// t_ast		*new_node(t_ms *ms, t_node_type type);

//----------------------- parser.c -----------------------
t_ast	*new_node(t_ms *ms, t_node_type type);
int		is_redirect(t_ms *ms, t_token *cur_tok);
t_ast	*factor(t_ms *ms);
t_ast	*handle_par(t_ms *ms);
t_ast	*term(t_ms *ms);
t_ast	*expr(t_ms *ms);
int		parse(t_ms *ms);

//----------------------- handle_cmds.c ------------------
// t_ast		*handle_cmd(t_ms *ms);
t_list		*add_cmd_args(t_ms *ms, t_ast *new_ast);

//----------------------- handle_ops.c -------------------
// t_ast		*handle_op(t_ms *ms);

//----------------------- handle_par.c -------------------
// t_ast		*handle_par(t_ms *ms);

//----------------------- handle_pipes.c -----------------
// t_ast		*handle_pipe(t_ms *ms);

//----------------------- handle_redirs.c ----------------
// int			is_redir(int type);
t_redirs	*handle_red(t_ms *ms, t_ast *new_ast);
void		redirs_add_back(t_redirs **lst, t_redirs *new);
t_redirs	*redirs_new(t_ms *ms, t_token *token, int type);

//----------------------- utils.c ------------------------
int			token_to_node(int type);
int			eat_token(t_ms *ms, t_type type);

void	print_tree(t_ast *root, int space);//temp

#endif