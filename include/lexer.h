/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:08:42 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/01 14:19:51 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
#define LEXER_H

typedef struct s_token;

typedef struct s_lexer
{
	char	*src;
	size_t	src_size;
	char	cur_c;
	t_token	*token_lst;
}		t_lexer;

typedef enum e_type
{
	T_WORD,
	T_EOF,
}		t_type;

typedef struct s_token
{
	t_type	type;
	char	*value;
	struct s_token	*next_token;
}		t_token;

#endif
