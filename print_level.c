#include "./include/lexer.h"
#include "./include/parser.h"
#include "./include/struct.h"
#include <stdlib.h>

static int	btree_level_count(t_ast *root)
{
	int left;
	int right;

	if (!root)
		return (0);
	left = btree_level_count(root->left);
	right = btree_level_count(root->right);
	return (1 + ((left > right) ? left : right));
}

void		btree_process_level(t_ast *root, int *t, int level)
{
	if (!root)
		return ;
    printf("Level %d : node type : %s\n", level, node_to_str(root));
	if (level == 1)
	{
		t[1] = 0;
	}
	else if (level > 1)
	{
		btree_process_level(root->left, t, level - 1);
		btree_process_level(root->right, t, level - 1);
	}
}

void		btree_apply_by_level(t_ast *root)
{
	int t[3];
	int h;
	int i;

	if (!root)
		return ;
	h = btree_level_count(root);
	i = 0;
	while (i < h)
	{
		t[0] = i;
		t[1] = 1;
		btree_process_level(root, t, ++i);
	}
}
