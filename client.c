#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "expression.h"
#include "custom_io.h"

#define DEBUG 0

int main() {
    struct process *process = create_process();
    
    while(1) {
        struct expression *expression = create_expression();

        add_process(process, expression);

        calculate_expression(expression);
        
        #if DEBUG
            PRINT_EXPRESSION(expression);
        #endif


        if (getchar() == '\n' || getchar() == EOF) {
            break;
        }
    }

    print_process(process);

    destroy_process(process);
}