#ifndef ASSIGN2_COLOURS_H
#define ASSIGN2_COLOURS_H

#include <map>
#include <string>

#define RESET   "\033[0m"      // Resets output to white 
#define BOLD_BRIGHT_RED       "\033[1m\033[31m"
#define BOLD_BRIGHT_YELLOW    "\033[1m\033[33m"
#define BOLD_BRIGHT_GREEN     "\033[1m\033[32m"   
#define BOLD_BRIGHT_CYAN      "\033[1m\033[36m"
#define BOLD_BRIGHT_BLUE      "\033[1m\033[34m" 
#define BOLD_BRIGHT_MAGENTA   "\033[1m\033[35m"   
#define BOLD_BRIGHT_WHITE     "\033[1m\033[37m"      


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
