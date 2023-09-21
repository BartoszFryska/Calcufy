#include <math.h>
#include <string>
#include <cstring>

const std::string LIST_OF_FUNCTIONS [] = { "sin", "cos", "tg", "ctg" }  ;

int perform_a_function ( std::string name, int input ) {

    if ( name == "sin" ) {

        return sin ( input );
    }

    if ( name == "cos" ) {

        return cos ( input );
    }

    if ( name == "tg" ) {

        return tan ( input );
    }

    if ( name == "ctg" ) {

        return 1 / tan ( input );
    }

    throw false;
}