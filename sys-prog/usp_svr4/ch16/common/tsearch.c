#include <search.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct node {
    char    *string;
    int     length;
};

int     compareNode(const void *, const void *);
void    printNode(void **, VISIT, int);

int
main(void)
{
    void *root;
    struct node *n;
    char buf[BUFSIZ];

    root = NULL;

    /*
     * Read strings until end of file.
     */
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        /*
         * Strip the newline.
         */
        buf[strlen(buf) - 1] = '\0';

        /*
         * Allocate a node structure.
         */
        n = (struct node *) malloc(sizeof(struct node));

        if (n == NULL) {
            fprintf(stderr, "out of memory.\n");
            exit(1);
        }

        /*
         * Save the information in the node.
         */
        n->string = strdup(buf);
        n->length = strlen(buf);

        /*
         * Add the item to the tree.
         */
        (void) tsearch((void *) n, &root, compareNode);
    }

    /*
     * Print out the tree in alphabetical order.
     */
    twalk(root, printNode);

    exit(0);
}

/*
 * compareNode - compare the strings in two nodes.
 */
int
compareNode(const void *a, const void *b)
{
    struct node *aa, *bb;

    aa = (struct node *) a;
    bb = (struct node *) b;

    return(strcmp(aa->string, bb->string));
}

/*
 * printNode - print a node - we only print if this is the postorder
 *             (inorder) visit or a leaf; this results in
 *             alphabetical order.
 */
void
printNode(void **node, VISIT order, int level)
{
    struct node *n;

    n = *(struct node **) node;

    if (order == postorder || order == leaf)
        printf("level=%d, length=%d, string=%s\n", level, n->length,
        n->string);
}
