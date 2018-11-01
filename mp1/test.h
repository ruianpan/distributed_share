#ifndef TEST_H
#define TEST_H
#include <string>
#include "dgrep.h"

#define ASC_ZERO 48

/**
 * [test_generates_logs description: This function generates testing log on every vms
 * for infrequent testing]
 * @param vm_id [description: vm number]
 */
void test_generates_logs(int vm_id){
    FILE *new_log;
    switch(vm_id){
        case 1:
            new_log = fopen("testvm1.log", "w");
            fprintf(new_log, "This is the generated log for testing on vm %d.\n", vm_id);
            fprintf(new_log, "How do you know your program works? \n");
            fprintf(new_log, "This is the MP’s second part \n");
            fprintf(new_log, "– you will write unit tests\n");
            fprintf(new_log, "While unit tests typically run locally, for this MP,\n");
            fprintf(new_log, "you will think more broadly and write distributed unit tests. \n");
            break;
        case 2:
            new_log = fopen("testvm2.log", "w");
            fprintf(new_log, "This is the generated log for testing on vm %d.\n", vm_id);
            fprintf(new_log, "This is an example of what is called a nondeterministic finite automaton (NFA)\n");
            fprintf(new_log, "Intuitvely, such a machine could have many possible computations on a given input.\n");
            fprintf(new_log, "finite automaton (NFA). Intuitvely, such a machine given input. For example, on an input of the form u001v, \n");
            fprintf(new_log, "it is possible for the machine to reach qp also on the input u01v\n");
            fprintf(new_log, "The fact that the machines behavior is not determined by the input string\n");
            break;
        case 3:
            new_log = fopen("testvm3.log", "w");
            fprintf(new_log, "This is the generated log for testing on vm %d.\n", vm_id);
            fprintf(new_log, "In CS 225, we feel it’s important you understand how a C++ program compiles.\n");
            fprintf(new_log, "Your Makefile must compile together your own solution files, \n");
            fprintf(new_log, "Open mp1.cpp and complete the rotate function.\n");
            fprintf(new_log, "Rotate the image 180 degrees,\n");
            fprintf(new_log, "Write the rotated image out as outputFile\n");
            break;
        case 4:
            new_log = fopen("testvm4.log", "w");
            fprintf(new_log, "This is the generated log for testing on vm %d.\n", vm_id);
            fprintf(new_log, "1.ForthefollowingcircuitfromLab1,willstatic-0hazardhappen\n");
            fprintf(new_log, "Transition from 000 to 010 (toggling B) will not cause the output to change\n");
            fprintf(new_log, " the NAND gate will always give a 1 and hence the output Z is always 0.\n");
            fprintf(new_log, "Astudentconnected4bitsinshiftregisterAtotheswitchboxinthefollowingway,\n");
            fprintf(new_log, "A signal that uses 0 to represent “enabled”\n");
            break;
        case 5:
            new_log = fopen("testvm5.log", "w");
            fprintf(new_log, "This is the generated log for testing on vm %d.\n", vm_id);
            fprintf(new_log, "ow to specify a set of outcomes, events, and probabilities for a given experiment \n");
            fprintf(new_log, "set theory (e.g. de Morgan's law, Karnaugh maps for two sets) \n");
            fprintf(new_log, "using Karnaugh maps for three sets\n");
            fprintf(new_log, "random variables, probability mass functions\n");
            fprintf(new_log, "independence of events and random variables\n");
            break;
        case 6:
            new_log = fopen("testvm6.log", "w");
            fprintf(new_log, "This is the generated log for testing on vm %d.\n", vm_id);
            fprintf(new_log, "RMaster the basic concepts and methodologies of digital signal processing with this systematic introduction,\n");
            fprintf(new_log, "ithout the need for an extensive mathematical background. The authors lead the reader through the funda\n");
            fprintf(new_log, "o engineering practice, and equips students and\n");
            fprintf(new_log, "of applications. Chapters include worke\n");
            fprintf(new_log, "pplied Digital Signal Processin\n");
            break;
        case 7:
            new_log = fopen("testvm7.log", "w");
            fprintf(new_log, "This is the generated log for testing on vm %d.\n", vm_id);
            fprintf(new_log, "You must be enrolled in ECE391 to access this resource.\n");
            fprintf(new_log, "nding the Linux Kernel, 3rd edition, O'Reilly, 2005, ISBN 0-596-00565-2. T\n");
            fprintf(new_log, "one of the authors of the Linux Device Drivers book. T\n");
            fprintf(new_log, "Understanding the Linux Virtual Memory Manag\n");
            fprintf(new_log, "C Language and Library Reference \n");
            break;
        case 8:
            new_log = fopen("testvm8.log", "w");
            fprintf(new_log, "This is the generated log for testing on vm %d.\n", vm_id);
            fprintf(new_log, "Demo to a TA in office hours by 1/23 at 11:59pm\n");
            fprintf(new_log, "HW Dropbox #54 in ECEB on 2/12 by 5PM\n");
            fprintf(new_log, "Committed to GitLab by 6PM 2/5\n");
            fprintf(new_log, "Tuesday, 6-8:30pm: Last name starts with M-Z\n");
            fprintf(new_log, "C to x86 linkage, device I/O;\n");
            break;
        case 9:
            new_log = fopen("testvm9.log", "w");
            fprintf(new_log, "This is the generated log for testing on vm %d.\n", vm_id);
            fprintf(new_log, "In the past, we've been able to admit everyone who wanted to g\n");
            fprintf(new_log, "Important This will change as I purge typos, etc. In the past,\n");
            fprintf(new_log, "This construction works for standard deviation and covariance, too\n");
            fprintf(new_log, "The weak law of large numbers sa\n");
            fprintf(new_log, "is very close to E[X] for large N\n");
            break;
        case 10:
            new_log = fopen("testvm10.log", "w");
            fprintf(new_log, "This is the generated log for testing on vm %d.\n", vm_id);
            fprintf(new_log, "Obtenga traducciones inglés-español en contexto a partir de ejemplos \n");
            fprintf(new_log, "Regístrese en Reverso y tendrá la oportunidad de guardar su historial y sus favoritos.\n");
            fprintf(new_log, "El español se habla en muchos países a través de cuatro continentes\n");
            fprintf(new_log, "cuentra una traducción al inglés en nuestro diccionario español-inglés o en nuestro buscador con acceso\n");
            fprintf(new_log, "cialistas en información que pueden responder a sus \n");
            break;
    }
    fclose(new_log);
}

/**
 * [remove_test_file description: This function removes all testing log files for this test]
 * @param vm_id [description: vm number]
 */
void remove_test_file(int vm_id){
    std::string file_name("testvm");
    file_name.push_back(vm_id + ASC_ZERO);
    file_name += ".log";
    remove(&file_name[0]);
}

#endif
