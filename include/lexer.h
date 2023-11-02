/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:08:42 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/02 18:12:04 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include <stdio.h>
# include "../libft/libft.h"
# include "struct.h"
# include "parser.h"

//----------------------- main.c ------------------------
char	*tok_to_str(t_token *token);
char	*node_to_str(t_ast *node);
void	print_token_lst(t_token *token);
void	print_lst(t_list *args_enter);
void	visit_node(t_ast *root);
t_ms	*init_ms(void);
int	main(int argc, char **argv);

//----------------------- end.c ------------------------
void	ft_lstfree(t_list **lst);
void	token_lst_free(t_token **lst);
void	free_root_ast(t_ast *root);
void	free_minishell(t_ms *minishell, int exit_status);

//----------------------- lexer.c ------------------------
t_lexer	*init_lexer(char *s);
t_token	*init_token(t_ms *minishell, char *value, t_type type);
void	advance(t_lexer *lexer, int i);
t_token	*parse_word(t_ms *minishell, t_lexer *lexer);
int		is_wspace(char c);
t_token	*lexer_next_token(t_ms *minishell, t_lexer * lexer);
void	lexer(t_ms *minishell, char *s);
void	token_add_back(t_token **lst, t_token *new);

#endif
