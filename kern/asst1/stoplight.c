/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>

/*
 *
 * Constants
 *
 */

/*
 * Number of cars created.
 */

#define NCARS 20


/*
 *
 * Function Definitions
 *
 */

static int car_count;           // number of cars occupying the four intersection regions
static struct lock *lock_count; // mutex when attempting to modify car_count variable

static struct lock *lock_n;    // lock held if a car approached N side
static struct lock *lock_e;    // lock held if a car approached E side
static struct lock *lock_s;    // lock held if a car approached S side
static struct lock *lock_w;    // lock held if a car approached W side
static struct lock *approach_locks[4];

static struct lock *lock_nw;    // lock held if a car occupies NW region
static struct lock *lock_ne;    // lock held if a car occupies SW region
static struct lock *lock_se;    // lock held if a car occupies NE region
static struct lock *lock_sw;    // lock held if a car occupies SE region
static struct lock *region_locks[4];

static struct semaphore *donesem;

static const char *directions[] = { "N", "E", "S", "W" };


static const char *msgs[] = {
        "approaching:",
        "region1:    ",
        "region2:    ",
        "region3:    ",
        "leaving:    "
};

/* use these constants for the first parameter of message */
enum { APPROACHING, REGION1, REGION2, REGION3, LEAVING };

static void
message(int msg_nr, int carnumber, int cardirection, int destdirection)
{
        kprintf("%s car = %2d, direction = %s, destination = %s\n",
                msgs[msg_nr], carnumber,
                directions[cardirection], directions[destdirection]);
}
 
/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
    // approach
    int destdirection = (cardirection + 2) % 4;
    lock_acquire(approach_locks[cardirection]);
    message(APPROACHING, carnumber, cardirection, destdirection);
    
    // region 1
    struct lock *lock_region1 = region_locks[(cardirection + 0) % 4];
    while (1) {
        while (car_count > 2);
        lock_acquire(lock_region1);
        if (car_count > 2) {
            lock_release(lock_region1);
        } else {
            break;
        }
    }
    lock_release(approach_locks[cardirection]);
    lock_acquire(lock_count);
    car_count += 1;
    lock_release(lock_count);
    message(REGION1, carnumber, cardirection, destdirection);

    // region 2
    struct lock *lock_region2 = region_locks[(cardirection + 3) % 4];
    lock_acquire(lock_region2);
    message(REGION2, carnumber, cardirection, destdirection);
    lock_release(lock_region1);
    
    // leave
    message(LEAVING, carnumber, cardirection, destdirection);
    lock_release(lock_region2);
    lock_acquire(lock_count);
    car_count -= 1;
    lock_release(lock_count);
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
    // approach
    int destdirection = (cardirection + 1) % 4;
    lock_acquire(approach_locks[cardirection]);
    message(APPROACHING, carnumber, cardirection, destdirection);
    
    // region 1
    struct lock *lock_region1 = region_locks[(cardirection + 0) % 4];
    while (1) {
        while (car_count > 2);
        lock_acquire(lock_region1);
        if (car_count > 2) {
            lock_release(lock_region1);
        } else {
            break;
        }
    }
    lock_release(approach_locks[cardirection]);
    lock_acquire(lock_count);
    car_count += 1;
    lock_release(lock_count);
    message(REGION1, carnumber, cardirection, destdirection);

    // region 2
    struct lock *lock_region2 = region_locks[(cardirection + 3) % 4];
    lock_acquire(lock_region2);
    message(REGION2, carnumber, cardirection, destdirection);
    lock_release(lock_region1);
    
    // region 3
    struct lock *lock_region3 = region_locks[(cardirection + 2) % 4];
    lock_acquire(lock_region3);
    message(REGION3, carnumber, cardirection, destdirection);
    lock_release(lock_region2);

    // leave
    message(LEAVING, carnumber, cardirection, destdirection);
    lock_release(lock_region3);
    lock_acquire(lock_count);
    car_count -= 1;
    lock_release(lock_count);
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
    // approach
    int destdirection = (cardirection + 3) % 4;
    lock_acquire(approach_locks[cardirection]);
    message(APPROACHING, carnumber, cardirection, destdirection);
    
    // region 1
    struct lock *lock_region1 = region_locks[(cardirection + 0) % 4];
    while (1) {
        while (car_count > 2);
        lock_acquire(lock_region1);
        if (car_count > 2) {
            lock_release(lock_region1);
        } else {
            break;
        }
    }
    lock_release(approach_locks[cardirection]);
    lock_acquire(lock_count);
    car_count += 1;
    lock_release(lock_count);
    message(REGION1, carnumber, cardirection, destdirection);
    
    // leave
    message(LEAVING, carnumber, cardirection, destdirection);
    lock_release(lock_region1);
    lock_acquire(lock_count);
    car_count -= 1;
    lock_release(lock_count);
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */
 
static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
    int cardirection;
    int carturn;
    (void) unusedpointer;

    cardirection = random() % 4;
    carturn = random() % 3;
    switch (carturn) {
        case 0: gostraight(cardirection, carnumber);    break;
        case 1: turnleft(cardirection, carnumber);      break;
        case 2: turnright(cardirection, carnumber);     break;
        default: break;
    }
    V(donesem);
    return;
}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;

        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;

        /*
         * Start NCARS approachintersection() threads.
         */

        inititems();

        for (index = 0; index < NCARS; index++) {
                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );
                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }

        for (index = 0; index < NCARS; index++) {
            P(donesem);
        }
        

        return 0;
}

static
void
inititems(void)
{
    car_count = 0;
    if (lock_count == NULL) {
        lock_count = lock_create("lock_count");
        if (lock_count == NULL) {
            panic("stoplight: lock_create failed\n");
        }
    }
    if (lock_n == NULL) {
        lock_n = lock_create("lock_n");
        if (lock_n == NULL) {
            panic("stoplight: lock_create failed\n");
        }
        approach_locks[0] = lock_n;
    }
    if (lock_e == NULL) {
        lock_e = lock_create("lock_e");
        if (lock_e == NULL) {
            panic("stoplight: lock_create failed\n");
        }
        approach_locks[1] = lock_e;
    }
    if (lock_s == NULL) {
        lock_s = lock_create("lock_s");
        if (lock_s == NULL) {
            panic("stoplight: lock_create failed\n");
        }
        approach_locks[2] = lock_s;
    }
    if (lock_w == NULL) {
        lock_w = lock_create("lock_w");
        if (lock_w == NULL) {
            panic("stoplight: lock_create failed\n");
        }
        approach_locks[3] = lock_w;
    }
    if (lock_nw == NULL) {
        lock_nw = lock_create("lock_nw");
        if (lock_nw == NULL) {
            panic("stoplight: lock_create failed\n");
        }
        region_locks[0] = lock_nw;
    }
    if (lock_ne == NULL) {
        lock_ne = lock_create("lock_ne");
        if (lock_ne == NULL) {
            panic("stoplight: lock_create failed\n");
        }
        region_locks[1] = lock_ne;
    }
    if (lock_se == NULL) {
        lock_se = lock_create("lock_se");
        if (lock_se == NULL) {
            panic("stoplight: lock_create failed\n");
        }
        region_locks[2] = lock_se;
    }
    if (lock_sw == NULL) {
        lock_sw = lock_create("lock_sw");
        if (lock_sw == NULL) {
            panic("stoplight: lock_create failed\n");
        }
        region_locks[3] = lock_sw;
    }
    if (donesem == NULL) {
        donesem = sem_create("donesem", 0);
        if (donesem == NULL) {
            panic("stoplight: sem_create failed\n");
        }
    }
}

