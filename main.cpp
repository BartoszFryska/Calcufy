#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <array>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "calcufy.cpp"

int main () {

    operation current_op;
    std::vector < operation > previous_op;
    print_an_app_name();

    prepare_for_typing ( previous_op );

    std::string input;

    while ( !if_exit( input ) ) {

        if ( !process_and_execute_input (input, current_op, previous_op ) ) {

            printf ( "(invalid command, please try again)\n");
            continue;
        }
    }
}