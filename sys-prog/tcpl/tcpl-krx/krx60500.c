int undef(char * name) {
    struct nlist * np1, * np2;

    if ((np1 = lookup(name)) == NULL)  /*  name not found  */
        return 1;

    for ( np1 = np2 = hashtab[hash(name)]; np1 != NULL;
          np2 = np1, np1 = np1->next ) {
        if ( strcmp(name, np1->name) == 0 ) {  /*  name found  */

            /*  Remove node from list  */

            if ( np1 == np2 )
                hashtab[hash(name)] = np1->next;
            else
                np2->next = np1->next;

            /*  Free memory  */

            free(np1->name);
            free(np1->defn);
            free(np1);

            return 0;
        }
    }

    return 1;  /*  name not found  */
}

