/**
 * @file built_in.h
 * @author Roman Bartl (xbartl06)
 * @author Petr Sopf (xsopfp00)
 * @brief Built in functions of IFJ17 language
 */



/**
 *
 * @param stringsToPrint
 *
 * Function prints out given strings in array of string 'stringsToPrint' next to each other in appropriate format (e.g. escape sequences - \n)
 * Integer will be printed as '% d' (like in C) Double as '% g'.
 *
 * TODO - have no idea, how to recognize data type of output string (perhaps two arrays - array of data types and array of values in appropriate order) + string is not specified in assignment
 */
void print(char **stringsToPrint);


/**
 *
 * @return - value loaded from console in array
 *
 * Firstly prints out string !"? "
 * Then reads value depending on data type of variable, where this value will be stored.
 * First blank characters are ignored.
 * Ends read after EOL character in console, which is ignored too.
 * Function ignores all characters after first inappropriate character (including even this character).
 * (e.g. if data type of variable, where will be this value stored, is integer and gets character, the function will ignore all other characters on input)
 * If loading string, the boundary characters '"' will be also ignored.
 * Escape sequences are not supported.
 * If input value in console is missing, function will return 0, 0.0 or !"", depending on data type of variable
 *
 * TODO - I'd return every value as string and depending on data type of variable this value will be converted; not sure about this
 */
char *input();


/**
 *
 * @param string - string from which will be length computed
 * @return - length of given string
 *
 * Returns length of given string.
 * For example - stringLength ( !"x\nz" ) = 3
 */
int stringLength(char *string);


/**
 *
 * @param string - string from which will be cut substring
 * @param startChar - location of starting character
 * @param stringLength - length of final substring
 * @return - substring of given string
 *
 * Returns substring of 'stringLength' length from given 'string'. Substring will start @ 'startChar' position in 'string'
 * If 'stringLength' <= 0 then returns empty string (!"")
 * if 'stringLength' < 0 or 'stringLength' > stringLength ( 'string' ) - 'startChar' then returns substring from 'startChar' to last character of 'string'
 */
char *subStr(char *string, int startChar, int stringLength);


/**
 *
 * @param string
 * @param charPosition - position of char in 'string' which value of string will return
 * @return - value of ASCII of given 'string' on 'charPosition' position
 *
 * If 'charPosition' < 0 or 'charPosition' > stringLength ( string ) - 1 then function will return 0
 */
int asc(char *string, int charPosition);


/**
 *
 * @param asciiVal - value of ASCII character from interval <0, 255>
 * @return string with only one character - from given ASCII value
 *
 * Function has undefined behavior if value isn't from ASCII interval ( <0, 255> )
 */
char *chr(int asciiVal);