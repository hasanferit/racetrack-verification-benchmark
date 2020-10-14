//includes
#include <stdio.h>
#include <stdlib.h>
#include "../include/intrinsics.h"
#include "../include/functions.h"

//Constants
const int velocity_limit_x = 5;
const int velocity_limit_y = 5;

//Functions - main.c
int main()
{
    /* TODO command line interface */
    Map map = get_map();
    int step_limit = 50;
    int look_ahead_steps = 3;
    int safety_distance = 1;
    State initial_state = get_initial_state(map);
    int success = run_safeguard_controller(map, initial_state, step_limit, look_ahead_steps, safety_distance);

	if(__CRAB_nd()){
    	__CRAB_assert(success != 2);
	}
	else{
    	__CRAB_assert(success == 1);
	}

    return success;
}

//Functions - safeguard.c
/* TODO implement safety distance */
int run_safeguard_controller(const Map map, const State initial_state, int step_limit, int look_ahead_steps, int safety_distance)
{

    if (step_limit < 1){
        return 0;
    }

    int goal_check = __CRAB_nd();
	check_goal_state(initial_state.position_x, initial_state.position_y, initial_state.velocity_x, initial_state.velocity_y, goal_check);

    if (goal_check == 1)
    {
        return 1;
    }

	Position initial_position = {initial_state.position_x, initial_state.position_y};

    /* step zero */
	int step = 0;

    Acceleration acceleration;
    State state, next_state;
	state.position_x = __CRAB_nd();
	state.position_y = __CRAB_nd();
	state.velocity_x = __CRAB_nd();
	state.velocity_y = __CRAB_nd();
	next_state.position_x = __CRAB_nd();
	next_state.position_y = __CRAB_nd();
	next_state.velocity_x = __CRAB_nd();
	next_state.velocity_y = __CRAB_nd();

    acceleration = compute_acceleration(map, initial_state, look_ahead_steps, safety_distance);
    execute_acc(initial_state.position_x, initial_state.position_y, initial_state.velocity_x, initial_state.velocity_y, acceleration.x, acceleration.y, state.position_x, state.position_y, state.velocity_x, state.velocity_y);
	goal_check = __CRAB_nd();
	for(int step = 1; step < step_limit; step++){
		check_goal_state(state.position_x, state.position_y, state.velocity_x, state.velocity_y, goal_check);
		switch(goal_check){
			case 0:
				return 0;
				break;
			case 1:
				return 1;
				break;
			case 2:
            	acceleration = compute_acceleration(map, state, look_ahead_steps, safety_distance);
            	execute_acc(state.position_x, state.position_y, state.velocity_x, state.velocity_y, acceleration.x, acceleration.y, next_state.position_x, next_state.position_y, next_state.velocity_x, next_state.velocity_y);
            	state = next_state;
				break;
			default:
				break;
		}
	}

	check_goal(state.position_x, state.position_y, state.velocity_x, state.velocity_y, goal_check);
	return goal_check;

}

Acceleration compute_acceleration(const Map map, const State state, int look_ahead_steps, int safety_distance){

    Acceleration acceleration;
    acceleration.x = __CRAB_nd();
    acceleration.y = __CRAB_nd();
    acceleration = call_nn_model(map, state);

    if (!look_ahead_check(map, state, look_ahead_steps, safety_distance))
    {
        Acceleration negated_acceleration = get_negated_acceleration(acceleration);
		return negated_acceleration;
    }
	else{
    	return acceleration;
	}
}

int look_ahead_check(const Map map, State state, int look_ahead_steps, int safety_distance)
{
	State simulated_state; 
	simulated_state.position_x = __CRAB_nd();
	simulated_state.position_y = __CRAB_nd();
	simulated_state.velocity_x = __CRAB_nd();
	simulated_state.velocity_y = __CRAB_nd();
	
    Acceleration simulated_acceleration;
    simulated_acceleration.x = __CRAB_nd();
    simulated_acceleration.y = __CRAB_nd();
	
	for(int i=0;i<look_ahead_steps;i++){
		Acceleration simulated_acceleration = call_nn_model_lookahead(map, state);
    	execute_acc(state.position_x, state.position_y, state.velocity_x, state.velocity_y, simulated_acceleration.x, simulated_acceleration.y, simulated_state.position_x, simulated_state.position_y, simulated_state.velocity_x, simulated_state.velocity_y);
		if(is_null_state(simulated_state)){
			return 0;
		}
        else if(is_goal_position(map, simulated_state)){
            return 1;
        }
		else{
			state = simulated_state;
		}
	}
	return 1;
}

Acceleration call_nn_model(const Map map, const State state)
{
    // First we prepare the input here - we have no use for get_nn_input anymore
    // Get fresh feature variables
    int f1 = __CRAB_nd(), f2 = __CRAB_nd(), f3 = __CRAB_nd(), f4 = __CRAB_nd(),
        f5 = __CRAB_nd(), f6 = __CRAB_nd(), f7 = __CRAB_nd(), f8 = __CRAB_nd(),
        f9 = __CRAB_nd(), f10 = __CRAB_nd(), f11 = __CRAB_nd(), f12 = __CRAB_nd(),
        f13 = __CRAB_nd(), f14 = __CRAB_nd();
    f1 = state.position_x;
    f2 = state.position_y;
    f3 = state.velocity_x;
    f4 = state.velocity_y;
    
    Position position = {state.position_x, state.position_y};
	Velocity velocity = {state.velocity_x, state.velocity_y};

    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, -1, -1, f5); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, -1, 0, f6); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, -1, 1, f7); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, 0, -1, f8); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, 0, 1, f9); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, 1, -1, f10); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, 1, 0, f11); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, 1, 1, f12); 

    compute_goal_distance(position.x, position.y, velocity.x, velocity.y, f13, f14);

    /* The fourteen variables we pass are going to be used to project the invariants**/
    
    int ax = __CRAB_nd();
    int ay = __CRAB_nd();

	nn(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, ax, ay);
    Acceleration acceleration = create_acceleration(ax, ay);
    return acceleration;

}

Acceleration call_nn_model_lookahead(const Map map, const State state)
{
    // First we prepare the input here - we have no use for get_nn_input anymore
    // Get fresh feature variables
    int f1 = __CRAB_nd(), f2 = __CRAB_nd(), f3 = __CRAB_nd(), f4 = __CRAB_nd(),
        f5 = __CRAB_nd(), f6 = __CRAB_nd(), f7 = __CRAB_nd(), f8 = __CRAB_nd(),
        f9 = __CRAB_nd(), f10 = __CRAB_nd(), f11 = __CRAB_nd(), f12 = __CRAB_nd(),
        f13 = __CRAB_nd(), f14 = __CRAB_nd();
    f1 = state.position_x;
    f2 = state.position_y;
    f3 = state.velocity_x;
    f4 = state.velocity_y;
    
    Position position = {state.position_x, state.position_y};
	Velocity velocity = {state.velocity_x, state.velocity_y};

    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, -1, -1, f5); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, -1, 0, f6); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, -1, 1, f7); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, 0, -1, f8); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, 0, 1, f9); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, 1, -1, f10); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, 1, 0, f11); 
    compute_wall_distance(position.x, position.y, velocity.x, velocity.y, 1, 1, f12); 

    compute_goal_distance(position.x, position.y, velocity.x, velocity.y, f13, f14);

    /* The fourteen variables we pass are going to be used to project the invariants**/
    
    int ax = __CRAB_nd();
    int ay = __CRAB_nd();

	nn_lookahead(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, ax, ay);
    Acceleration acceleration = create_acceleration(ax, ay);
    return acceleration;

}
//Functions - racetrack.c
State simulate_acceleration(const Map map, const State state, const Acceleration acceleration)
{
    return get_next_state(map, state, acceleration);
}

State execute_acceleration(const Map map, const State state, const Acceleration acceleration)
{
    return get_next_state(map, state, acceleration);
}

State get_initial_state(const Map map)
{
    Position initial_position = get_start_position(map);
    Velocity initial_velocity = get_start_velocity();
    State initial_state = {initial_position.x, initial_position.y, initial_velocity.x, initial_velocity.y};

    return initial_state;
}

int is_null_state(State state)
{
	if(state.position_x == -100 && state.position_y == -100 && state.velocity_x == -100 && state.velocity_y == -100)
    {
		return 1;
	}
	else
    {
		return 0;
	}
}

State get_next_state(const Map map, const State state, const Acceleration acceleration)
{
    if (!is_valid_acceleration(map, state, acceleration))
    {
    	State null_state = {-100, -100, -100, -100}; //Arbitary choice
        return null_state;
    }

    Velocity next_velocity = {state.velocity_x + acceleration.x, state.velocity_y + acceleration.y};
    Position next_position = {state.position_x + next_velocity.x, state.position_y + next_velocity.y};
    State next_state = {next_position.x, next_position.y, next_velocity.x, next_velocity.y};
    return next_state;
}

int is_goal_state(const Map map, const State state)
{
    Position position = {state.position_x, state.position_y};
    Velocity velocity = {state.velocity_x, state.velocity_y};
    return is_goal(map, position);// && is_zero(velocity);
}

int is_goal_state_manual(const State state){
    int px = state.position_x;
    int py = state.position_y;
    if(py != 0){
        return 0;
    }
    else if(px == 32 || px == 33 || px == 34){
        return 1;
    }
    else{
        return 0;
    }
}

int is_goal_position(const Map map, const State state)
{
    Position position = {state.position_x, state.position_y};
    return is_goal(map, position);
}

Acceleration get_negated_acceleration(const Acceleration acceleration)
{
    Acceleration negated_acceleration = {-acceleration.x, -acceleration.y};
    return negated_acceleration;
}

Position get_position(const State state)
{
    Position p = {state.position_x, state.position_y};
    return p;
}

Velocity get_velocity(const State state)
{
    Velocity v = {state.velocity_x, state.velocity_y};
    return v;
}

Acceleration create_acceleration(int x, int y)
{
    Acceleration acceleration = {x, y};
    return acceleration;
}

int is_valid_acceleration(const Map map, const State state, const Acceleration acceleration)
{
    Velocity accumulated_velocity = {state.velocity_x + acceleration.x, state.velocity_y + acceleration.y};
    int is_valid = __CRAB_nd();
    check_valid_velocity(state.position_x, state.position_y, accumulated_velocity.x, accumulated_velocity.y, is_valid);
    return is_valid;
}

int is_valid_velocity(const Map map, const Position position, const Velocity velocity)
{
    int x = position.x;
    int y = position.y;
    int vx = velocity.x;
    int vy = velocity.y;

    if (vx > velocity_limit_x || vy > velocity_limit_y)
    {
        return 0;
    }

    int sign_vx = vx >= 0 ? 1 : -1;
    int sign_vy = vy >= 0 ? 1 : -1;
    int vx_abs = abs(vx);
    int vy_abs = abs(vy);

    for (int step_vx = 0; step_vx <= vx_abs; step_vx++)
    {
        for (int step_vy = 0; step_vy <= vy_abs; step_vy++)
        {
            int is_valid = __CRAB_nd();
            trajectory_lookup(vx_abs, vy_abs, step_vx, step_vy, is_valid);
            if (is_valid)
            {
                Position traversed_position = {x + sign_vx * step_vx, y + sign_vy * step_vy};
                int is_valid_p = is_valid_position(traversed_position);
                if (!is_valid_p)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int is_goal(const Map map, const Position position)
{
    int p = __CRAB_nd();
    map_lookup(position.x, position.y, p);
    return p == 'g';
}

int is_valid_position(const Position position){
    int p = __CRAB_nd();
    map_lookup(position.x, position.y, p);
    return p != 'x';
}

int is_zero(const Velocity velocity)
{
    int vx = velocity.x;
    int vy = velocity.y;
    return vx == 0 && vy == 0;
}

Position get_start_position(const Map map)
{
    Position start_position = {0, 14};
    return start_position;
}

Velocity get_start_velocity()
{
    Velocity start_velocity = {0, 0};
    return start_velocity;
}

void delete_map(Map *map)
{
    free(map);
}

void delete_position(Position *position)
{
    free(position);
}

void delete_velocity(Velocity *velocity)
{
    free(velocity);
}

void delete_acceleration(Acceleration *acceleration)
{
    free(acceleration);
}

void delete_distance(Distance *distance)
{
    free(distance);
}

void delete_state(State *state)
{
    free(state);
}

Map get_map()
{
    Map map = {35, 12, 4, 3}; //For our fixed Map
    return map;
}
