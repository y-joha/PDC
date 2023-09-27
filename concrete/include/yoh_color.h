
#ifndef __YOH_COLOR_H__
#define __YOH_COLOR_H__

#include <stdio.h>

#ifndef __STDC_VERSION__
  #define PrintColor(color) printf("%s", color)
#elif __STDC_VERSION__ >= 199901L
static inline void PrintColor(const char *color)
{
    printf("%s", color);
}
#endif


#if 0
|*===================================================================*|
|*|                  With Many thanks of course to                  |*|
|*|                Irit the Greatest Programmer Alive.              |*|
|*|                                                                 |*|
|*|         now you can use this H file to Print Colors!!!!         |*|
|*|                 add to the include folder, and                  |*|
|*|         dont forget to include in the H/C/Cpp files.            |*|
|*|                                                                 |*|
|*|                   The colors are listed below.                  |*|
|*|                                                                 |*|
|*|                                                                 |*|
|*|                                                                 |*|
|*|                             Enjoy                               |*|
|*|                         and have fun                            |*|
|*|       ************************************************          |*|
|*|       *        After each use of color on the text   *          |*|
|*|       *               PrintColor(Purple);            *          |*|
|*|       *            you must reset the color          *          |*|
|*|       *             PrintColor(Color_Off);           *          |*|
|*|       ************************************************          |*|
|*|   {                  Code Example :                             |*|
|*|                                                                 |*|
|*|     PrintColor(Red);                                            |*|
|*|     printf("%s");/puts("etc...");  {perror ignores this Shtick} |*|
|*|     PrintColor(Color_Off);                                      |*|
|*|   }                                                             |*|
|*|                                                                 |*|
|*|                     -Why the ifndef                             |*|
|*|        the ifndef above checks to which standart this is        |*|
|*|         compiled to, and either pick the Macro version          |*|
|*|                 for the ansi standard (C89).                    |*|
|*|                                                                 |*|
|*|         or from c99 in order for the func to be inline          |*|
|*|         it requires to be static so each compilation unit       |*|
|*|        will have its own duplicate func but, we dont use       |*|
|*|              the macro version which is ill advised.            |*|
|*|                                                                 |*|
|*|       btw{There is an arrow between the number and text,        |*|
|*|            you can minimize comments/func/etc...        }       |*|
|====================================================================*|
#endif
 
/*    # Reset   */
#define Color_Off "\033[0m"

/*    # Regular Colors   */
#define Black "\033[0;30m"
#define Red "\033[0;31m"
#define Green "\033[0;32m"
#define Yellow "\033[0;33m"
#define Blue "\033[0;34m"
#define Purple "\033[0;35m"
#define Cyan "\033[0;36m"
#define White "\033[0;37m"

/*    # Bold   */
#define BBlack "\033[1;30m"
#define BRed "\033[1;31m"
#define BGreen "\033[1;32m"
#define BYellow "\033[1;33m"
#define BBlue "\033[1;34m"
#define BPurple "\033[1;35m"
#define BCyan "\033[1;36m"
#define BWhite "\033[1;37m"

/*    # Underline   */
#define UBlack "\033[4;30m"
#define URed "\033[4;31m"
#define UGreen "\033[4;32m"
#define UYellow "\033[4;33m"
#define UBlue "\033[4;34m"
#define UPurple "\033[4;35m"
#define UCyan "\033[4;36m"
#define UWhite "\033[4;37m"

/*    # Background   */
#define On_Black "\033[40m"
#define On_Red "\033[41m"
#define On_Green "\033[42m"
#define On_Yellow "\033[43m"
#define On_Blue "\033[44m"
#define On_Purple "\033[45m"
#define On_Cyan "\033[46m"
#define On_White "\033[47m"

/*    # High Intensity   */
#define IBlack "\033[0;90m"
#define IRed "\033[0;91m"
#define IGreen "\033[0;92m"
#define IYellow "\033[0;93m"
#define IBlue "\033[0;94m"
#define IPurple "\033[0;95m"
#define ICyan "\033[0;96m"
#define IWhite "\033[0;97m"

/*    # Bold High Intensity   */
#define BIBlack "\033[1;90m"
#define BIRed "\033[1;91m"
#define BIGreen "\033[1;92m"
#define BIYellow "\033[1;93m"
#define BIBlue "\033[1;94m"
#define BIPurple "\033[1;95m"
#define BICyan "\033[1;96m"
#define BIWhite "\033[1;97m"

/*    # High Intensity backgrounds   */
#define On_IBlack "\033[0;100m"
#define On_IRed "\033[0;101m"
#define On_IGreen "\033[0;102m"
#define On_IYellow "\033[0;103m"
#define On_IBlue "\033[0;104m"
#define On_IPurple "\033[0;105m"
#define On_ICyan "\033[0;106m"
#define On_IWhite "\033[0;107m"

#endif
