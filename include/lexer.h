/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:08:42 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/15 13:54:10 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H


# include "../libft/libft.h"
# include "struct.h"
# include "parser.h"
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

//----------------------- main.c ------------------------
char	*tok_to_str(t_token *token);
char	*node_to_str(t_ast *node);
void	print_token_lst(t_token *token);
void	print_lst(t_list *args_enter);
void	print_redirs(t_redirs *args_enter);
void	visit_node(t_ast *root);
t_ms	*init_ms(void);
char	*display_prompt();
int	main(int argc, char **argv);

//----------------------- end.c ------------------------
void	ft_lstfree(t_list **lst);
void	token_lst_free(t_token **lst);
void	redirs_free(t_redirs **lst);
void	free_root_ast(t_ast *root);
void	free_minishell(t_ms *minishell, int exit_status);

//----------------------- lexer.c ------------------------
t_token	*parse_word(t_ms *minishell, t_lexer *lexer);
t_token	*lexer_next_token(t_ms *minishell, t_lexer * lexer);
void	token_add_back(t_token **lst, t_token *new);
int		lexer(t_ms *minishell, char *s);

//--------------------- lexer_utils.c ----------------------
void	advance(t_lexer *lexer);
void	advance_ntimes(t_lexer *lexer, int i);
int		is_wspace(char c);
char	peek_next(t_lexer *lexer);
int		ft_ischar(int c, int quotes);

//--------------------- lexer_init.c ----------------------
t_lexer	*init_lexer(char *s);
t_token	*init_token(t_ms *minishell, char *value, t_type type);

//--------------------- lexer_error.c ----------------------
int		check_par(t_ms *ms);

int		error_in_lexer(t_ms *ms);

//--------------------- prelexer_error.c ----------------------
int		count_quotes(char *line);
int		check_error_prelexer(char *line);

#endif
