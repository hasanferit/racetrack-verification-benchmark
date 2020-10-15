#include <stdio.h>

#include "racetrack.h"
#include "safeguard.h"

int main()
{

    /* TODO command line interface */
    char *nn_model_filename = "policies/corner/";
    Map *map = get_map();
    int step_limit = 50;
    int look_ahead_steps = 3;
    int safety_distance = 1;
    State *initial_state = get_intial_state(map);

    int success = run_safeguard_controller(map, initial_state, nn_model_filename, step_limit,
                                           look_ahead_steps, safety_distance);

    delete_state(initial_state);
    delete_map(map);
    return success;
}