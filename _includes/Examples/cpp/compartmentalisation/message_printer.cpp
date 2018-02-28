#include <iostream>

// forward declared function prototype
void print_message();

int main(){
    print_message();
    return 0;
}

// function definition
void print_message(){
    std::cout << "Hello world" << std::endl;
}

