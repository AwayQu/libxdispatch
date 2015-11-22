/*
* runner.c
* 
* Copyright (c) 2011-2014 MLBA-Team
* All rights reserved.
* 
* @LICENSE_HEADER_START@
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*     http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
* @LICENSE_HEADER_END@
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#ifndef WIN32
 # include <unistd.h>
 # include <sys/time.h>
 # include <sys/resource.h>
 # include <sys/wait.h>
#else /* ifndef WIN32 */
 # include <process.h>
#endif /* ifndef WIN32 */

#include "private.h"

const char *STR_VERBOSE = "-v";
const char *STR_LIST_TESTS = "-l";
const char *STR_SHOW_HELP = "-h";
const char *STR_RUN_TEST = "-t";
const char *STR_KEEP_RUNNING = "-k";
const char *STR_REPEAT_TEST = "-r";
const char *STR_CTEST = "-ctl";
const char *STR_RUN_TEST_BY_NAME = "-n";

void print_header()
{
    printf( "\n   MUnit Test Framework with independent processes\n" );
    printf( "   (c) 2011 MLBA\n\n" );
}


void MU_initFramework(
    MU_messageHandler handler
)
{
    msg_handler = handler;
    suite = createList();
    suite->data = NULL;
}


void MU_printHelp()
{
    print_header();
    printf( "Either call the programm directly or pass an additional argument:\n" );
    printf( "<program> <arg>\n\n" );
    printf( "Argument\t Meaning\n" );
    printf( "-----------------------------------------------------------------\n" );
    printf( "none\tExecute all available tests and stop when the first failed\n" );
    printf( "%s\tExecute all available tests even if some of them fail\n", STR_KEEP_RUNNING );
    printf( "%s\tPrint this help\n", STR_SHOW_HELP );
    printf( "%s\tList all available tests\n", STR_LIST_TESTS );
    printf( "%s [no]\tRepeat the testrun [no] times\n", STR_REPEAT_TEST );
    printf( "%s [no]\tRun only the test with the given [no]\n", STR_RUN_TEST );
    printf( "%s [na]\tRun only the test with the given [na]\n", STR_RUN_TEST_BY_NAME );
    printf( "%s\tVerbose output of all assertions\n", STR_VERBOSE );
    printf( "%s\tPrint a CTest compatible list of all tests\n", STR_CTEST );
    printf( "\n" );
} /* MU_printHelp */


static int test_runs = 1;
static int test_selection = -1;
static char *test_selection_name = NULL;
static char keep_running = 0;
char verbose = 0;

enum modes
{
    DISPLAY_HELP,
    DISPLAY_TESTS,
    DISPLAY_CTEST,
    MULTIPLE_TESTS,
    RUN_SINGLE,
    RUN_SINGLE_BY_NAME,
    SUITE
};


static int mode = SUITE;

void parse_arguments(
    int argc,
    char *argv[]
)
{
    int i;

    for( i = 1; i < argc; i++ )
    {
        if( strcmp( argv[ i ], STR_RUN_TEST ) == 0 )
        {
            if( mode == SUITE ) // we do not want to override the multiple tests
                                // selection
                mode = RUN_SINGLE;

            i++;
            test_selection = atoi( argv[ i ] );
        }
        else if( strcmp( argv[ i ], STR_RUN_TEST_BY_NAME ) == 0 )
        {
            if( mode == SUITE ) // we do not want to override the multiple tests
                                // selection
                mode = RUN_SINGLE_BY_NAME;

            mode = RUN_SINGLE_BY_NAME;
            i++;
            test_selection_name = argv[ i ];
        }
        else if( strcmp( argv[ i ], STR_KEEP_RUNNING ) == 0 )
            keep_running = 1;
        else if( strcmp( argv[ i ], STR_REPEAT_TEST ) == 0 )
        {
            mode = MULTIPLE_TESTS;
            i++;
            test_runs = atoi( argv[ i ] );
        }
        else if( strcmp( argv[ i ], STR_LIST_TESTS ) == 0 )
            mode = DISPLAY_TESTS;
        else if( strcmp( argv[ i ], STR_CTEST ) == 0 )
            mode = DISPLAY_CTEST;
        else if( strcmp( argv[ i ], STR_SHOW_HELP ) == 0 )
            mode = DISPLAY_HELP;
        else if( strcmp( argv[ i ], STR_VERBOSE ) == 0 )
            verbose = 1;
        else
        {
            printf( "Unknown parameter: %s\n", argv[ i ] );
            exit( 1 );
        }
    }
} /* parse_arguments */


int
repeat_suite(
    const char *bin,
    char keep_running,
    int times
);

int
repeat_test(
    const char *bin,
    int no,
    char keep_running,
    int times
);

int MU_main(
    int argc,
    char *argv[]
)
{
    parse_arguments( argc, argv );

    switch( mode )
    {
     case DISPLAY_HELP:
         MU_printHelp();
         return 0;

         break;

     case DISPLAY_TESTS:
         MU_printTests();
         return 0;

         break;

     case DISPLAY_CTEST:
         MU_printTestsCTest();
         return 0;

         break;

     case RUN_SINGLE:
         return MU_runTest( test_selection );

         break;

     case RUN_SINGLE_BY_NAME:
         return MU_runTestName( test_selection_name );

         break;

     case MULTIPLE_TESTS:

         if( test_selection == -1 )
             repeat_suite( argv[ 0 ], keep_running, test_runs );
         else
             repeat_test( argv[ 0 ], test_selection, keep_running, test_runs );

         break;

     default: // SUITE
         return MU_runAllTests( argv[ 0 ], keep_running );
    } /* switch */

    return 1;
} /* MU_main */


/*
int run_suite(const char* bin, bool keep_going){
#ifdef _WIN32
    intptr_t ret;
    if(verbose && keep_going)
        ret = _spawnl(_P_WAIT, bin, bin, STR_KEEP_RUNNING, STR_VERBOSE, NULL);
    else if(verbose)
        ret = _spawnl(_P_WAIT, bin, bin, STR_VERBOSE, NULL);
    else if(keep_runnig)
        ret = _spawnl(_P_WAIT, bin, bin, STR_KEEP_RUNNING, NULL);
    else
        ret = _spawnl(_P_WAIT, bin, bin, NULL);
    if(ret==EXIT_SUCCESS)
        return 0;
    else
        return EXIT_FAILURE;
#else
    pid_t pid;
    int res = 0;
    pid = fork();
    if (pid == -1) { // failed
        printf("Error forking suite: %s\n", strerror(errno));
        exit(1);
    } else if (pid == 0) { // child process
        // launch process
        if(verbose && keep_going) {
            if (execlp(bin, bin, STR_VERBOSE, STR_KEEP_RUNNING, NULL) < 0) {
                printf("Error running suite: %s\n", strerror(errno));
                return EXIT_FAILURE;
            }
        } else if(verbose) {
            if (execlp(bin, bin, STR_VERBOSE, NULL) < 0) {
                printf("Error running suite: %s\n", strerror(errno));
                return EXIT_FAILURE;
            }
        } else if(keep_going) {
            if (execlp(bin, bin, STR_KEEP_RUNNING, NULL) != 0) {
                printf("Error running suite: %s\n", strerror(errno));
                return EXIT_FAILURE;
            }
        } else {
            if (execlp(bin, bin, NULL) != 0) {
                printf("Error running suite: %s\n", strerror(errno));
                return EXIT_FAILURE;
            }
        }
        exit(0);
    } else {
        if(wait(&res)==pid){
            if(WEXITSTATUS(res)==EXIT_SUCCESS)
                return 0;
            else
                return EXIT_FAILURE;
        }
    }
    return 0;
#endif
} */

int run(
    int no,
    const char *bin
)
{
#ifdef _WIN32
    intptr_t ret;
    char test[ 5 ];
 # ifdef WIN32_VS
    sprintf_s( test, 5, "%u", no );
 # else
    sprintf( test, "%u", no );
 # endif
    printf( "(%u) ", no );

    if( verbose && keep_running )
        ret = _spawnl( _P_WAIT, bin, bin, STR_RUN_TEST, test, STR_KEEP_RUNNING, STR_VERBOSE, NULL );
    else if( verbose )
        ret = _spawnl( _P_WAIT, bin, bin, STR_RUN_TEST, test, STR_VERBOSE, NULL );
    else if( keep_running )
        ret = _spawnl( _P_WAIT, bin, bin, STR_RUN_TEST, test, STR_KEEP_RUNNING, NULL );
    else
        ret = _spawnl( _P_WAIT, bin, bin, STR_RUN_TEST, test, NULL );

    if( ret == EXIT_SUCCESS )
        return 0;
    else
        return EXIT_FAILURE;

#else /* ifdef _WIN32 */
    pid_t pid;
    int res = 0;
    char test[ 5 ];
    sprintf( test, "%u", no );
    printf( "(%u) ", no );
    fflush( stdout );
    pid = fork();

    if( pid == -1 ) // failed
    {
        printf( "Error forking test: %s\n", strerror( errno ) );
        exit( 1 );
    }
    else if( pid == 0 ) // child process
    {
        // launch process
        if( verbose && keep_running )
        {
            if( execlp( bin, bin, STR_RUN_TEST, test, STR_VERBOSE, STR_KEEP_RUNNING, NULL ) < 0 )
            {
                printf( "Error running test: %s\n", strerror( errno ) );
                return EXIT_FAILURE;
            }
        }
        else if( verbose )
        {
            if( execlp( bin, bin, STR_RUN_TEST, test, STR_VERBOSE, NULL ) < 0 )
            {
                printf( "Error running test: %s\n", strerror( errno ) );
                return EXIT_FAILURE;
            }
        }
        else if( keep_running )
        {
            if( execlp( bin, bin, STR_RUN_TEST, test, STR_KEEP_RUNNING, NULL ) < 0 )
            {
                printf( "Error running test: %s\n", strerror( errno ) );
                return EXIT_FAILURE;
            }
        }
        else
        {
            if( execlp( bin, bin, STR_RUN_TEST, test, NULL ) < 0 )
            {
                printf( "Error running test: %s\n", strerror( errno ) );
                return EXIT_FAILURE;
            }
        }

        exit( 0 );
    }
    else
    {
        if( wait( &res ) == pid )
        {
            if( WIFEXITED( res ) && WEXITSTATUS( res ) == EXIT_SUCCESS )
                return 0;
            else
                return EXIT_FAILURE;
        }
    }

    return 0;

#endif /* ifdef _WIN32 */
} /* run */


void print_begin(
    int times
)
{
    printf( "Running all Tests %i times\n", times );
    printf( "##################################\n" );
}


void print_result(
    int times,
    int failed_iterations
)
{
    if( failed_iterations != 0 )
    {
        printf( "\n##################################\nTEST RUN HAD FAILURES\n" );
        printf( "\tPassed iterations: %i\n\tFailed iterations: %i\n", times - failed_iterations, failed_iterations );
        printf( "##################################\n" );
    }
    else
    {
        printf( "\n##################################\nTEST RUN PASSED\n" );
        printf( "\tPassed iterations: %i\n\tFailed iterations: %i\n", times - failed_iterations, failed_iterations );
        printf( "##################################\n" );
    }
}


int repeat_test(
    const char *bin,
    int no,
    char keep_running,
    int times
)
{
    int iteration = 0;
    int failed_iterations = 0;

    print_header();
    print_begin( times );

    for( iteration = 0; iteration < times; iteration++ )
    {
        int res;

        printf( "\nTest iteration %i of %i\n", iteration + 1, times );
        printf( "----------------------------------\n" );

        res = run( no, bin );

        if( res != EXIT_SUCCESS )
        {
            failed_iterations++;

            if( !keep_running )
            {
                print_result( times, failed_iterations );
                return EXIT_FAILURE;
            }
        }
    }

    if( times > 1 )
        print_result( times, failed_iterations );

    return failed_iterations == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
} /* repeat_test */


int repeat_suite(
    const char *bin,
    char keep_running,
    int times
)
{
    item_t *curr;
    int iteration = 0;
    int failed_iterations = 0;

    print_header();

    if( times > 1 )
        print_begin( times );
    else
    {
        printf( "Running all Tests\n" );
        printf( "==================================\n" );
    }

    for( iteration = 0; iteration < times; iteration++ )
    {
        int res;
        int passed = 0, failed = 0;
        int no = 0;
        curr = suite;

        // signal the test position
        if( times > 1 )
        {
            printf( "\nTest iteration %i of %i\n", iteration, times );
            printf( "----------------------------------\n" );
        }

        // iterate over all tests
        while( curr->next != NULL ) {
            curr = curr->next;
            no++;

            // clear cache first
            fflush( stdout );
            // then run the test
            res = run( no, bin );

            if( res != EXIT_SUCCESS )
            {
                failed++;

                if( !keep_running )
                {
                    if( times > 1 )
                        printf( "==================================\nTEST FAILURE (at run %i of %i)\n", iteration, times );
                    else
                        printf( "==================================\nTEST FAILURE\n" );

                    printf( "----------------------------------\nPassed: %u\tFailed: %u\n\n", passed, failed );
                    return res;
                }
            }
            else
                passed++;
        }

        printf( "==================================\n%s\n", failed == 0 ? "ALL PASSED." : "!!! NOT ALL PASSED !!!" );

        if( failed != 0 )
            failed_iterations++;

        printf( "----------------------------------\nPassed: %u\tFailed: %u\n\n", passed, failed );
    }

    if( times > 1 )
        print_result( times, failed_iterations );

    return failed_iterations == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
} /* repeat_suite */


int MU_runAllTests(
    const char *bin,
    char keep_running
)
{
    return repeat_suite( bin, keep_running, 1 );
}


int MU_runTest(
    int no
)
{
    int ct = 0;
    item_t *curr = suite;
    mu_test_t *test = NULL;

    for( ct = 0; ct < no; ct++ )
        curr = curr->next;

    if( curr == NULL )
    {
        print_header();
        printf( " ! Test (%u) does not exist\n\n", no );
        return EXIT_FAILURE;
    }

    test = (mu_test_t *)curr->data;
    test->function();

    return EXIT_SUCCESS;
} /* MU_runTest */


int MU_runTestName(
    const char *name
)
{
    item_t *curr = suite->next;
    mu_test_t *test = NULL;

    while( curr != NULL ) {
        test = curr->data;

        if( strcmp( test->name, name ) == 0 )
            break;

        curr = curr->next;
    }

    if( curr == NULL )
    {
        print_header();
        printf( " ! Test (%s) does not exist\n\n", name );
        return EXIT_FAILURE;
    }

    test->function();

    return EXIT_SUCCESS;
} /* MU_runTestName */


void MU_printTestsCTest()
{
    int no = 0;
    item_t *curr;
    mu_test_t *test;

    print_header();
    printf( "Add to your CMakeLists.txt\n" );
    printf( "----------------------------------\n" );

    curr = suite;

    while( curr->next != NULL ) {
        curr = curr->next;
        no++;

        // clear cache first
        fflush( stdout );
        // then print the info
        test = (mu_test_t *)curr->data;
        printf( "add_test( NAME %s COMMAND ${TEST_EXE} -n %s )\n", test->name, test->name );
    }

    printf( "\n" );
} /* MU_printTestsCTest */


void MU_printTests()
{
    int no = 0;
    item_t *curr;
    mu_test_t *test;

    print_header();
    printf( "Available Tests\n" );
    printf( "----------------------------------\n" );

    curr = suite;

    while( curr->next != NULL ) {
        curr = curr->next;
        no++;

        // clear cache first
        fflush( stdout );
        // then print the info
        test = (mu_test_t *)curr->data;
        printf( "%u\t%s\n", no, test->name );
    }

    printf( "\n" );
} /* MU_printTests */
