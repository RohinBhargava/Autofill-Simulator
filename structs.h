typedef struct word {
	char *name;
	int score;
} word;

typedef struct word_tree {
	struct word *name;
	struct word_tree *left;
	struct word_tree **children;
	int tree_count;
} word_tree;

typedef struct word_hash {
	struct word_tree **hash;
} word_hash;