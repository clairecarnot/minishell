/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:09:54 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/08 09:52:57 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "../libft/libft.h"
# include "lexer.h"

typedef struct s_ast	t_ast;

typedef enum e_node_type
{
	PIPE,//0
	COMMAND,//1
}			t_node_type;

struct	s_ast
{
	t_node_type	type;
	t_ast		*right;
	t_ast		*left;
	t_list		*args;
};

typedef enum e_type
{
	T_WORD,
	T_PIPE,
	T_AND_IF,
	T_OR_IF,
	T_LPAR,
	T_RPAR,
	T_LESS,
	T_GREAT,
	T_DLESS,
	T_DGREAT,
	T_NEWLINE;
	T_EOF,
}		t_type;

typedef struct s_token
{
	t_type	type;
	char	*value;
	struct s_token	*next_token;
	size_t	tok_nb;
}		t_token;

typedef struct s_lexer
{
	char	*src;
	size_t	src_size;
	char	cur_c;
	size_t	cur_pos;
	size_t	next_pos;
	t_token	*token_lst;
	size_t	tok_count;
}		t_lexer;

typedef struct	s_parser
{
	t_lexer	*lexer;
	t_ast	*root;
	t_token	*cur_token;
}				t_parser;

typedef struct	s_ms
{
	t_lexer		*lexer;
	t_parser	*parser;
	char		*line;
}				t_ms;

#endif 