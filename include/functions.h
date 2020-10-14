#include "structs.h"

//function declarations
int is_valid_acceleration(const Map map, const State state, const Acceleration acceleration);
int is_valid_velocity(const Map map, const Position position, const Velocity velocity);
int is_valid_position(Position position);
int is_goal(const Map map, const Position position);
int is_zero(const Velocity velocity);
State simulate_acceleration(const Map map, const State state, const Acceleration acceleration);
State execute_acceleration(const Map map, const State state, const Acceleration acceleration);
State get_initial_state(const Map map);
State get_next_state(const Map map, const State state, const Acceleration acceleration);
int is_goal_state(const Map map, const State state);
int is_goal_position(const Map map, const State state);
Acceleration get_negated_acceleration(const Acceleration acceleration);
Position get_position(const State state);
Velocity get_velocity(const State state);
Acceleration create_acceleration(int x, int y);
Map get_map();
int run_safeguard_controller(const Map map, const State initial_state, int step_limit, int look_ahead_steps, int safety_distance);
Acceleration call_nn_model(const Map map, const State state);
Acceleration call_nn_model_lookahead(const Map map, const State state);
Acceleration compute_acceleration(const Map map, const State state, int look_ahead_steps, int safety_distance);
int look_ahead_check(const Map map, State state, int look_ahead_steps, int safety_distance);
Position get_start_position();
Velocity get_start_velocity();
int is_null_state(const State state);