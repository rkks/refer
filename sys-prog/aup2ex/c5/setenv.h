#ifndef _SETENV_H_
#define _SETENV_H_

#define setenv my_setenv
#define unsetenv my_unsetenv

int setenv(const char *var, const char *val, int overwrite);
int unsetenv(const char *var);

#endif /* _SETENV_H_ */
