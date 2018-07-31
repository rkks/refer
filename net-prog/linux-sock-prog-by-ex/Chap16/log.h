/* log.h
 *
 * log.c externs:
 */
extern int log_open(const char *pathname);
extern void log(const char *format,...);
extern void log_close(void);
extern void bail(const char *on_what);
