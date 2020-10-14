//Crab statements
extern int __CRAB_nd();
extern void __CRAB_assert(int);
extern void __CRAB_assume(int);
extern void __CRAB_intrinsic_logger(int);
extern void __CRAB_intrinsic_access_map(int, int, int);
extern void __CRAB_intrinsic_get_wall_distance(int, int, int, int, int, int, int);
extern void __CRAB_intrinsic_get_goal_distance(int, int, int, int, int, int);
extern void __CRAB_intrinsic_is_valid_velocity(int, int, int, int, int);
extern void __CRAB_intrinsic_is_goal_state(int, int, int, int, int);
extern void __CRAB_intrinsic_check_status(int, int, int, int, int);
extern void __CRAB_intrinsic_print_invariants();
extern void __CRAB_intrinsic_execute_acceleration(int, int, int, int, int, int, int ,int, int ,int);
extern void __CRAB_intrinsic_print_invariants_to_variables(int, int);
extern void __CRAB_intrinsic_print_state_invariants(int ,int, int ,int);
extern void __CRAB_intrinsic_print_invariants_generic_stateacc(int ,int, int ,int, int, int);
extern void __CRAB_intrinsic_access_velocity_traversed_position(int, int, int, int, int);
extern void __CLAM_print_var_tags(int, int, int, int, int, int, int, int, int, int, int, int, int, int);
extern void __CRAB_intrinsic_deepsymbol(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
extern void __CRAB_intrinsic_deepsymbol_lookahead(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
extern void __CRAB_intrinsic_eran(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
extern void __CRAB_intrinsic_eran_lookahead(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);

void trajectory_lookup(int a, int b, int c, int d, int e){
    __CRAB_intrinsic_access_velocity_traversed_position(a, b, c, d, e);
}

void map_lookup(int a, int b, int c){
    __CRAB_intrinsic_access_map(a, b, c);
}

void execute_acc(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j){
    // Mainly accesses the 6x6x6x6 projection array
    __CRAB_intrinsic_execute_acceleration(a, b, c, d, e, f, g, h, i, j);
}

void check_goal_state(int a, int b, int c, int d, int e){
    // Mainly accesses the map array
    __CRAB_intrinsic_is_goal_state(a, b, c, d, e);
}

void check_goal(int a, int b, int c, int d, int e){
    // Mainly accesses the map array with minor optimisations
    __CRAB_intrinsic_check_status(a, b, c, d, e);
}

void nn(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p){
    //Projects invariants onto input variables and computes symbolic output (for last two inputs) for the NN abstraction 
    __CRAB_intrinsic_deepsymbol(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);
}

void nn_lookahead(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p){
    //Projects invariants onto input variables and computes symbolic output for the NN abstraction with minor variation for lookahead
    __CRAB_intrinsic_deepsymbol_lookahead(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);
}

void compute_wall_distance(int a, int b, int c, int d, int e, int f, int g){
    //Mainly accesses the map array (45x50 for Ring)
    __CRAB_intrinsic_get_wall_distance(a, b, c, d, e, f, g);
}

void compute_goal_distance(int a, int b, int c, int d, int e, int f){
    //Mainly accesses the map array (45x50 for Ring)
    __CRAB_intrinsic_get_goal_distance(a, b, c, d, e, f);
}

void check_valid_velocity(int a, int b, int c, int d, int e){
    __CRAB_intrinsic_is_valid_velocity(a ,b, c, d, e);
}

