#include "stdlib.h"

//struct definitions
typedef struct Position
{
    int x;
    int y;
} Position;

typedef struct Map
{
    int width;
    int height;
    /* number of goal positions */
    int ngoals;
    /* number of start positions */
    int nstarts;
} Map;

typedef struct Velocity
{
    int x;
    int y;
} Velocity;

typedef struct Acceleration
{
    int x;
    int y;
} Acceleration;

typedef struct State
{
    int position_x;
    int position_y;
    int velocity_x;
    int velocity_y;
} State;

typedef struct Distance
{
    int l1;
    int x;
    int y;
} Distance;

typedef struct NN_Input
{
    int feature_one;
    int feature_two;
    int feature_three;
    int feature_four;
    int feature_five;
    int feature_six;
    int feature_seven;
    int feature_eight;
    int feature_nine;
    int feature_ten;
    int feature_eleven;
    int feature_twelve;
    int feature_thirteen;
    int feature_fourteen;
} NN_Input;
