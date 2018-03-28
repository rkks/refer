#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

int
mkstemp(char *template)
{
    char *tempf;

    tempf = mktemp(template);

    if (strlen(template) == 0)
        return(-1);
    
    return(open(tempf, O_RDWR | O_CREAT | O_TRUNC, 0666));
}
