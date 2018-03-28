/*
 * A solution to exercise 5-8 in K&R2, page 112:
 *
 *	There is no error checking in day_of_year or month_day. Remedy
 *	this defect.
 *
 * The error to check for is invalid argument values. That is simple, what's
 * hard is deciding what to do in case of error. In the real world, I would
 * use the assert macro from assert.h, but in this solution I take the
 * approach of returning -1 instead. This is more work for the caller, of
 * course.
 *
 * I have selected the year 1752 as the lowest allowed year, because that
 * is when Great Britain switched to the Gregorian calendar, and the leap
 * year validation is valid only for the Gregorian calendar.
 *
 * Lars Wirzenius <liw@iki.fi>
 */

#include <stdio.h>

static char daytab[2][13] =  {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

/* day_of_year: set day of year from month & day */
int day_of_year(int year, int month, int day)
{
	int i, leap;
	
	if (year < 1752 || month < 1 || month > 12 || day < 1)
		return -1;

	leap = (year%4 == 0 && year%100 != 0) || year%400 == 0;
	if (day > daytab[leap][month])
		return -1;

	for (i = 1; i < month; i++)
		day += daytab[leap][i];
	return day;
}

/* month_day: set month, day from day of year */
int month_day(int year, int yearday, int *pmonth, int *pday)
{
	int i, leap;
	
	if (year < 1752 || yearday < 1)
		return -1;

	leap = (year%4 == 0 && year%100 != 0) || year%400 == 0;
	if ((leap && yearday > 366) || (!leap && yearday > 365))
		return -1;

	for (i = 1; yearday > daytab[leap][i]; i++)
		yearday -= daytab[leap][i];
	*pmonth = i;
	*pday = yearday;
	
	return 0;
}


/* main: test day_of_year and month_day */
int main(void)
{
	int year, month, day, yearday;
	
	for (year = 1970; year <= 2000; ++year) {
		for (yearday = 1; yearday < 366; ++yearday) {
			if (month_day(year, yearday, &month, &day) == -1) {
				printf("month_day failed: %d %d\n",
					year, yearday);
			} else if (day_of_year(year, month, day) != yearday) {
				printf("bad result: %d %d\n", year, yearday);
				printf("month = %d, day = %d\n", month, day);
			}
		}
	}
	
	return 0;
}
