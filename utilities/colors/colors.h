#ifndef _COLORS_H_
#define _COLORS_H_

/* Fonte: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c */

#define RED(string)     "\x1b[31m" string "\x1b[0m"
#define GREEN(string)   "\x1b[32m" string "\x1b[0m"
#define YELLOW(string)  "\x1b[33m" string "\x1b[0m"
#define BLUE(string)    "\x1b[34m" string "\x1b[0m"
#define MAGENTA(string) "\x1b[35m" string "\x1b[0m"
#define CYAN(string)    "\x1b[36m" string "\x1b[0m"

/* ESEMPI:
 *
 *     printf("this is " RED("red") "!\n");
 *
 *     printf("this is " BLUE("%s") "!\n","blue");
 */

#endif