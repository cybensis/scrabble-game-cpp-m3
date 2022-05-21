#ifndef ASSIGN2_COLOURS_H
#define ASSIGN2_COLOURS_H

#include <map>
#include <string>

#define RESET   "\033[0m"       
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

#define BOLD_BRIGHT_RED       "\033[1m\033[31m"      /* Bold Bright Red */
#define BOLD_BRIGHT_YELLOW    "\033[1m\033[33m"      /* Bold Bright Yellow */
#define BOLD_BRIGHT_GREEN     "\033[1m\033[32m"      /* Bold Bright Green */
#define BOLD_BRIGHT_CYAN      "\033[1m\033[36m"      /* Bold Bright Cyan */
#define BOLD_BRIGHT_BLUE      "\033[1m\033[34m"      /* Bold Bright Blue */
#define BOLD_BRIGHT_MAGENTA   "\033[1m\033[35m"      /* Bold Bright Magenta */
#define BOLD_BRIGHT_WHITE     "\033[1m\033[37m"      /* Bold Bright White */


const std::map<int, std::string> tilePointColours = {
    { 1, BOLD_BRIGHT_GREEN },
    { 2, BOLD_BRIGHT_BLUE },
    { 3, BOLD_BRIGHT_CYAN },
    { 4, BOLD_BRIGHT_MAGENTA },
    { 5, BOLD_BRIGHT_WHITE },
    { 8, BOLD_BRIGHT_YELLOW },
    { 10, BOLD_BRIGHT_RED }
};



#endif // ASSIGN2_COLOURS_H
