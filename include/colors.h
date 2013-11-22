#ifndef __COLORS_H__
#define __COLORS_H__

#ifndef COLORS_OFF

#define DEFCOLOR "\e[0m" 
#define DEF      DEFCOLOR
#define RED      "\e[0;31m" 
#define LRED     "\e[1;31m"
#define LGREEN   "\e[1;32m"
#define GREEN    "\e[0;32m"
#define YELLOW   "\e[1;33m"
#define BLUE     "\e[1;34m"
#define MAGENTA  "\e[1;35m"
#define LBLUE    "\e[1;36m"
#define CYAN     "\e[0;36m"
#define WHITE    "\e[1;37m"
#define GRAY     "\e[0;38m"

#else

#define DEFCOLOR
#define RED    
#define LRED   
#define LGREEN 
#define GREEN  
#define YELLOW 
#define BLUE   
#define MAGENTA
#define LBLUE  
#define CYAN   
#define WHITE  
#define GRAY   

#endif

#endif
