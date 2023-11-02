/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:08:42 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/02 14:04:06 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include <stdio.h>
# include "../libft/libft.h"
# include "ast.h"
# include "parser.h"

typedef enum e_type
{
	T_WORD,
	T_PIPE,
	T_EOF,
}		t_type;

typedef struct s_token
{
	t_type	type;
	char	*value;
	struct s_token	*next_token;
}		t_token;

typedef struct s_lexer
{
	char	*src;
	size_t	src_size;
	char	cur_c;
	size_t	cur_pos;
	t_token	*token_lst;
}		t_lexer;



//----------------------- lexer.c ------------------------
t_lexer	*init_lexer(char *s);
t_token	*init_token(char *value, t_type type);
void	advance(t_lexer *lexer, int i);
t_token	*parse_word(t_lexer *lexer);
int		is_wspace(char c);
t_token	*lexer_next_token(t_lexer * lexer);
t_lexer	*lexer(char *s);
void	token_add_back(t_token **lst, t_token *new);

#endif
