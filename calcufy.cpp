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
#include "functions.cpp"

#define MAX_STORED_CALCULATIONS 20

struct operation {

    int end_number = 0;
    std::string list_of_inputs = ""; 
    std::string comment = "";
    bool is_this_first_command_in_section = true;

    void erase_all () {

        end_number = 0;
        list_of_inputs = "";
        comment = "";
        is_this_first_command_in_section = true;
    }
};

std::string exec(const char* cmd) {

    std::array<char, 128> buffer;

    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void print_an_app_name() {

    //printing app name

    system ( "clear" );
    system ( "figlet -f DOS\\ Rebel \"Calcufy\"" );
    
    sleep (1);
    printf ("version 1.0.\n");

    sleep (1);
    system ("clear");

    printf ("for all the functions type only 'help', to exit type only 'exit'\n");
    sleep (2);

    system ("clear");
}

void print_separator () {

    printf ( "----------------------------------------------------\n");
}

void show_help () {

    //showing help
}

void prepare_for_typing ( std::vector <operation> &previous ) {

    //loading previous calculations
    //showing calculations
}

bool if_exit ( std::string &input ) {

    input = "";
    std::getline ( std::cin, input );

    if ( input == "exit" )
        return true;

    return false;
}

void add_cuurrent_op_to_previous ( operation &current_op, std::vector <operation> &previous_op ) {

    previous_op.push_back ( current_op );

    if ( previous_op.size() > MAX_STORED_CALCULATIONS ) {

        previous_op.erase ( previous_op.begin() );
    }

    current_op.erase_all();
}

bool manage_previous_and_current_operations ( std::string &input,  operation &current_op, std::vector <operation> &previous_op ) {

    if ( input == "\n" || input == "" ) {

        if ( !current_op.is_this_first_command_in_section ){

            print_separator ();

            add_cuurrent_op_to_previous ( current_op, previous_op );
        }

        return false;
    }

    current_op.list_of_inputs += input;

    return true;
}

void paste_commands_into_bc_lunux ( std::string &input,  operation &current_op ) {

    char * temp_cmd = new char [ sizeof ( "echo " + input + " | bc " ) ];

    std::string temp_str;

    strcat ( temp_cmd, "echo " );
    strcat ( temp_cmd, input.c_str() );
    strcat ( temp_cmd, " | bc 2>>/dev/null");

    temp_str = exec ( temp_cmd );

    current_op.end_number = std::stoi ( temp_str );

    current_op.is_this_first_command_in_section = false;

    printf ( "= %d\n%d ", current_op.end_number, current_op.end_number );

    delete temp_cmd;

    return;
}

bool process_and_execute_input ( std::string &input,  operation &current_op, std::vector <operation> &previous_op ) {
    
    if ( input == "help" ) {

        show_help ();
        return true;
    }

    bool was_already_some_digit = false;
    bool is_a_candidate_for_a_function = false;
    int position_for_a_function_to_start = 0;
    int position_for_a_function_to_end = 0;
    int position_of_the_last_closing_bracket = 0;
    std::string temp_str = "";

    if ( !current_op.is_this_first_command_in_section ) {

        input = std::to_string ( current_op.end_number ) + input;
        was_already_some_digit = true;
    }

    for ( int i = 0; i < input.size(); i++ ) {

        if ( isdigit( input [ i ] ) ) {

            was_already_some_digit = true;
            continue;
        }


        else if ( isalpha ( input [ i ] ) ) {

            is_a_candidate_for_a_function = true;

            position_for_a_function_to_start = i;

            i++;

            while ( is_a_candidate_for_a_function && i < input.size() ) {

                    if ( isblank ( input [ i ] ) ) {

                        position_for_a_function_to_end = i;

                        i++;

                        while ( isblank ( input [ i ] ) && i < input.size() ) {

                            i++;
                        }
                    }

                    else {
                        
                        if ( input [ i ] == '(' ) {

                            if ( position_for_a_function_to_end <= position_for_a_function_to_start ) {

                                position_for_a_function_to_end = i;
                            }

                            i++;

                            while ( i < input.size() ) {

                                if ( input [ i ] == ')' ) {
                                    
                                    position_of_the_last_closing_bracket = i;

                                    break;
                                }

                                if ( !isdigit( input [ i ] ) && !isblank( input [ i ] )) {

                                    is_a_candidate_for_a_function = false;

                                    break;
                                }

                                i++;
                            }

                            if ( i >= input.size() ) {

                                is_a_candidate_for_a_function = false;
                            }

                            break;
                        }

                        else {

                            if ( !isalpha ( input [ i ] ) ) {

                                    is_a_candidate_for_a_function = false;
                                }

                                i++;
                        }
                }
            }

            if ( is_a_candidate_for_a_function ) {

                try {

                    temp_str = std::string( std::to_string( perform_a_function ( input.substr ( position_for_a_function_to_start, position_for_a_function_to_end - position_for_a_function_to_start ), std::stoi ( input.substr ( position_for_a_function_to_end, position_for_a_function_to_end - position_of_the_last_closing_bracket ) ) ) ) );      
                }

                catch  ( bool error ) {
                    
                    return false;
                }

                std::string *temp2 = new std::string;

                *temp2 += input.substr ( 0, position_for_a_function_to_start );
                *temp2 += temp_str;
                if ( position_of_the_last_closing_bracket + 1 < input.size() ) {
                    
                    *temp2 += input.substr ( position_of_the_last_closing_bracket + 1, input.size() - position_of_the_last_closing_bracket -  1 );
                }

                input = *temp2;

                delete temp2;

                i = position_for_a_function_to_start;
                position_for_a_function_to_end = position_for_a_function_to_start;

                is_a_candidate_for_a_function = false;
            }

            else {

                return false;
            }
        }

        else {

            if ( isblank ( input [ i ] ) )  {

                continue;
            }

            if ( input [ i ] != '+' && input [ i ] != '-' && input [ i ] != '%' && input [ i ] != '/' && input [ i ] != '*' ) {

                return false;
            }


            if ( !was_already_some_digit ) {

                return false;
            }
        }
    }
    
    if ( !manage_previous_and_current_operations ( input, current_op, previous_op ) ) {

        return true;
    }
    
    paste_commands_into_bc_lunux ( input, current_op );

    return true;
}