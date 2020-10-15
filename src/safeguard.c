#include "safeguard.h"
#include "racetrack.h"
#include "nn.h"

Acceleration *compute_acceleration(const Map *map, const State *state, const NNModel *nn_model,
                                   int look_ahead_steps, int safety_distance);

int look_ahead_check(const Map *map, const State *state, const NNModel *nn_model,
                     int look_ahead_steps, int safety_distance);

/* TODO implement safety distance */
int run_safeguard_controller(const Map *map, const State *initial_state,
                             const char *nn_model_directory, int step_limit, int look_ahead_steps, int safety_distance)
{

    if (step_limit < 1)
    {
        return 0;
    }

    if (is_goal_state(map, initial_state))
    {
        return 1;
    }

    NNModel *nn_model = load_nn_model(nn_model_directory);
    if (nn_model == NULL)
    {
        return 0;
    }

    /* step zero */
    Acceleration *acceleration = compute_acceleration(map, initial_state, nn_model,
                                                      look_ahead_steps, safety_distance);
    State *state = execute_acceleration(map, initial_state, acceleration);
    delete_acceleration(acceleration);
    if (state == NULL)
    {
        delete_nn_model(nn_model);
        return 0;
    }

    int step = 1;
    State *next_state;
    while (!is_goal_state(map, state) && step < step_limit)
    {
        acceleration = compute_acceleration(map, state, nn_model, look_ahead_steps, safety_distance);
        next_state = execute_acceleration(map, state, acceleration);
        delete_acceleration(acceleration);
        delete_state(state);
        state = next_state;
        if (state == NULL)
        {
            delete_nn_model(nn_model);
            return 0;
        }
        step++;
    }

    delete_nn_model(nn_model);
    return 1;
}

Acceleration *compute_acceleration(const Map *map, const State *state, const NNModel *nn_model,
                                   int look_ahead_steps, int safety_distance)
{

    NNInput *nn_input = get_nn_input(map, state, nn_model);
    Acceleration *acceleration = call_nn_model(nn_model, nn_input);
    delete_nn_input(nn_input);
    if (!look_ahead_check(map, state, nn_model, look_ahead_steps, safety_distance))
    {
        Acceleration *negated_acceleration = get_negated_acceleration(acceleration);
        delete_acceleration(acceleration);
        acceleration = negated_acceleration;
    }
    return acceleration;
}

int look_ahead_check(const Map *map, const State *state, const NNModel *nn_model,
                     int look_ahead_steps, int safety_distance)
{

    if (look_ahead_steps < 1)
    {
        return 1;
    }

    NNInput *nn_input = get_nn_input(map, state, nn_model);
    Acceleration *simulated_acceleration = call_nn_model(nn_model, nn_input);
    ;
    State *simulated_state = simulate_acceleration(map, state, simulated_acceleration);
    delete_nn_input(nn_input);
    delete_acceleration(simulated_acceleration);
    if (simulated_state == NULL)
    {
        return 0;
    }
    int safe = look_ahead_check(map, simulated_state, nn_model, look_ahead_steps - 1, safety_distance);
    delete_state(simulated_state);
    return safe;
}