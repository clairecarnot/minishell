#ifndef LEXER_H
# define LEXER_H


# include "../libft/libft.h"
# include "struct.h"
# include "parser.h"
# include "export.h"
# include <stddef.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <errno.h>
# include <limits.h>

//----------------------- main.c ------------------------
char	*tok_to_str(t_token *token);
char	*node_to_str(t_ast *node);
void	print_token_lst(t_token *token);
void	print_lst(t_list *args_enter);
void	print_redirs(t_redirs *args_enter);
void	visit_node(t_ast *root);
t_ms	*init_ms(char **env);
char	*display_prompt();

//----------------------- end.c ------------------------
void	ft_lstfree(t_list **lst);
void	token_lst_free(t_token **lst);
void	redirs_free(t_redirs **lst);
void	free_root_ast(t_ast *root);
void	free_minishell(t_ms *minishell, int exit_status);

//----------------------- lexer.c ------------------------
t_token	*lexer_next_token_2(t_ms *minishell, t_lexer *lexer);
t_token	*lexer_next_token(t_ms *minishell, t_lexer * lexer);
void	token_add_back(t_token **lst, t_token *new);
int		lexer(t_ms *minishell, char *s);

//--------------------- lexer_utils.c ----------------------
void	advance(t_lexer *lexer);
void	advance_ntimes(t_lexer *lexer, int i);
int		is_wspace(char c);
char	peek_next(t_lexer *lexer);
int		ft_ischar(int c, int quotes);

//--------------------- lexer_parse_word.c ----------------------
t_token	*parse_following_quotes(t_ms *ms, t_lexer *lexer, int qtype);
t_token	*parse_quotes_word(t_ms *ms, t_lexer *lexer, int qtype);
t_token	*parse_word(t_ms *minishell, t_lexer *lexer, int i);

//--------------------- lexer_init.c ----------------------
t_lexer	*init_lexer(char *s);
t_token	*init_token(t_ms *minishell, char *value, t_type type);

//--------------------- lexer_error.c ----------------------
int		check_par(t_ms *ms);
int		alone_and_after_op(t_ms *ms);
int		consecutive_op(t_ms *ms);
int		error_in_lexer(t_ms *ms);

//--------------------- lexer_error_redir.c ----------------------
int		check_redir3(t_token *tok_lst);
int		check_redir2(t_token *tok_lst);
int		check_redir1(t_token *tok_lst);
int		check_redir(t_ms *ms);

//--------------------- prelexer_error.c ----------------------
// int		count_quotes(char *line);
int		count_squotes(char *line);
int		count_dquotes(char *line);
int		check_error_prelexer(char *line);



#endif
