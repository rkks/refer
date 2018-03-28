/* Gregory Pietsch - gkp1@flash.net */

/* Given the problem, I thought that this would be a better
 * description of daytab.
 */
static int *daytab = {
    0, 
    31,
    31+28,
    31+28+31,
    31+28+31+30,
    31+28+31+30+31,
    31+28+31+30+31+30,
    31+28+31+30+31+30+31,
    31+28+31+30+31+30+31+31,
    31+28+31+30+31+30+31+31+30,
    31+28+31+30+31+30+31+31+30+31,
    31+28+31+30+31+30+31+31+30+31+30,
    0, 
    31,
    31+29,
    31+29+31,
    31+29+31+30,
    31+29+31+30+31,
    31+29+31+30+31+30,
    31+29+31+30+31+30+31,
    31+29+31+30+31+30+31+31,
    31+29+31+30+31+30+31+31+30,
    31+29+31+30+31+30+31+31+30+31,
    31+29+31+30+31+30+31+31+30+31+30,
};

/* is it a leap year?  (assume it's my calendar, the Gregorian) */
int leap(int year)
{
    return ((year % 4) == 0) 
            && (((year % 100) != 0)
                || (year % 400) == 0)));
}

/* day_of_year:  set day of year from month & day */
int day_of_year(int year, int month, int day)
{
    return *(daytab + ((month - 1) + (leap(year) * 12))) + day;
}

/* month_day: set month, day from day of year */
void month_day(int year, int yearday, int *pmonth, int *pday)
{
    int m, ly;

    ly = leap(year);
    if (yearday < 1 || yearday > (365 + ly))
        return; /* no real error checking */
    m = leap(year) ? 23 : 11;
    while (*(daytab + m) > yearday)
        m--;
    if (pmonth)
        *pmonth = (m % 12) + 1;
    if (pday)
        *pday = yearday - (*(daytab + m));
}

