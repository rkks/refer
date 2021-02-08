/* Program 5.20: Construction of a parse tree
 * Using the same strategy that we used to evaluate prefix expressions
 * (see Program 5.4), this program builds a parse tree from a prefix
 * expression. For simplicity, we assume that operands are single
 * characters. Each call of the recursive function creates a new node
 * with the next character from the input as the token. If the token is
 * an operand, we return the new node; if it is an operator, we set the
 * left and right pointers to the tree built (recursively) for the two
 * arguments.
 *
 * Translation programs such as compilers often use such internal tree
 * representations for programs, because the trees are useful for many
 * purposes. e.g., we might imagine operands corresponding to variables
 * that take on values, and we could generate machine code to evaluate
 * the expression represented by the tree with a postorder traversal.
 * Or, we could use the tree to print out the expression in infix with
 * an inorder traversal or in postfix with a postorder traversal.
 */
char *a;
int i;
typedef struct Tnode* link;
struct Tnode {
  char token;
  link l, r;
};

link NEW(char token, link l, link r) {
  link x = malloc(sizeof *x);

  x->token = token;
  x->l = l;
  x->r = r;
  return x;
}

link parse() {
  char t = a[i++];
  link x = NEW(t, NULL, NULL);

  if ((t == '+') || (t == '*')) {
    x->l = parse();
    x->r = parse();
  }
  return x;
}
