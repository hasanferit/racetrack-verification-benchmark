#include <stdio.h>
#include <stdlib.h>

#include "racetrack.h"
#include "maps.h"

const int velocity_limit_x = 5;

const int velocity_limit_y = 5;

const int velocity_to_traversed_positions[6][6][6][6] =
    {{{{1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 1, 1, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 1, 1, 1, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 1, 1, 1, 1}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}},
     {{{1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 1, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 1}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}},
     {{{1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 1, 1, 0}, {0, 0, 0, 0, 1, 1}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}},
     {{{1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 1, 1}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}},
     {{{1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 1, 1}, {0, 0, 0, 0, 0, 0}}},
     {{{1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}},
      {{1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}},
      {{1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 0, 0, 0}},
      {{1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 0, 0}},
      {{1, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 1, 0}},
      {{1, 1, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 0, 1, 1}, {0, 0, 0, 0, 0, 1}}}};

struct Map
{
    int width;
    int height;
    char **grid;
    /* number of goal positions */
    int ngoals;
    /* number of start positions */
    int nstarts;
    /* goal positions */
    Position **goals;
    /* start positions */
    Position **starts;
};

struct Position
{
    int x;
    int y;
};

struct Velocity
{
    int x;
    int y;
};

struct Acceleration
{
    int x;
    int y;
};

struct State
{
    Position *position;
    Velocity *velocity;
};

struct Distance
{
    int l1;
    int x;
    int y;
};

int is_valid_acceleration(const Map *map, const State *state, const Acceleration *acceleration);

int is_valid_velocity(const Map *map, const Position *position, const Velocity *velocity);

int is_valid_position(const Map *map, const Position *position);

int is_goal(const Map *map, const Position *position);

int is_zero(const Velocity *velocity);

Position *get_start_position(const Map *map);

Velocity *get_start_velocity();

State *simulate_acceleration(const Map *map, const State *state, const Acceleration *acceleration)
{
    return get_next_state(map, state, acceleration);
}

State *execute_acceleration(const Map *map, const State *state, const Acceleration *acceleration)
{
    State *next_state = get_next_state(map, state, acceleration);
    print_state(map, next_state);
    return next_state;
}

State *get_intial_state(const Map *map)
{
    Position *start_position = get_start_position(map);
    Velocity *start_velocity = get_start_velocity();
    State *initial_state = malloc(sizeof(State));
    initial_state->position = start_position;
    initial_state->velocity = start_velocity;
    return initial_state;
}

State *get_next_state(const Map *map, const State *state, const Acceleration *acceleration)
{
    if (!is_valid_acceleration(map, state, acceleration))
    {
        return NULL;
    }
    Position *position = state->position;
    Velocity *velocity = state->velocity;
    Velocity *next_velocity = malloc(sizeof(Velocity));
    Position *next_position = malloc(sizeof(Position));
    next_velocity->x = velocity->x + acceleration->x;
    next_velocity->y = velocity->y + acceleration->y;
    next_position->x = position->x + next_velocity->x;
    next_position->y = position->y + next_velocity->y;
    State *next_state = malloc(sizeof(State));
    next_state->position = next_position;
    next_state->velocity = next_velocity;
    return next_state;
}

int is_goal_state(const Map *map, const State *state)
{
    Position *position = state->position;
    Velocity *velocity = state->velocity;
    return is_goal(map, position) && is_zero(velocity);
}

void print_state(const Map *map, const State *state)
{
    int width = map->width;
    int height = map->height;
    char **grid = map->grid;
    Position *position = state->position;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (x == position->x && y == position->y)
            {
                printf("c");
            }
            else
            {
                printf("%c", grid[x][y]);
            }
        }
        printf("\n");
    }
}

Distance *get_goal_distance(const Map *map, const Position *position)
{
    Distance *goal_distance = malloc(sizeof(Distance));
    goal_distance->x = map->width;
    goal_distance->y = map->height;
    goal_distance->l1 = map->width + map->height;

    Position **goals = map->goals;
    for (int i = 0; i < map->ngoals; i++)
    {
        Position *goal = goals[i];
        int x = abs(position->x - goal->x);
        int y = abs(position->y - goal->y);
        int l1 = x + y;
        if (l1 < goal_distance->l1)
        {
            goal_distance->x = x;
            goal_distance->y = y;
            goal_distance->l1 = l1;
        }
    }
    return goal_distance;
}

Acceleration *get_negated_acceleration(const Acceleration *acceleration)
{
    Acceleration *negated_acceleration = malloc(sizeof(Acceleration));
    negated_acceleration->x = -acceleration->x;
    negated_acceleration->y = -acceleration->y;
    return negated_acceleration;
}

Position *get_position(const State *state)
{
    return state->position;
}

Velocity *get_velocity(const State *state)
{
    return state->velocity;
}

Acceleration *create_acceleration(int x, int y)
{
    Acceleration *acceleration = malloc(sizeof(Acceleration));
    acceleration->x = x;
    acceleration->y = y;
    return acceleration;
}

int get_wall_distance(const Map *map, const Position *position, const Velocity *velocity)
{
    int distance = 0;
    Position traversed_position = {position->x, position->y};
    while (is_valid_velocity(map, &traversed_position, velocity))
    {
        traversed_position.x += velocity->x;
        traversed_position.y += velocity->y;
        distance++;
    }
    return distance;
}

int is_valid_acceleration(const Map *map, const State *state, const Acceleration *acceleration)
{
    Position *position = state->position;
    Velocity *velocity = state->velocity;
    Velocity accumulated_velocity = {velocity->x + acceleration->x, velocity->y + acceleration->y};
    return is_valid_velocity(map, position, &accumulated_velocity);
}

int is_valid_velocity(const Map *map, const Position *position, const Velocity *velocity)
{
    int x = position->x;
    int y = position->y;
    int vx = velocity->x;
    int vy = velocity->y;

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
            if (velocity_to_traversed_positions[vx_abs][vy_abs][step_vx][step_vy])
            {
                Position traversed_position = {x + sign_vx * step_vx, y + sign_vy * step_vy};
                if (!is_valid_position(map, &traversed_position))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int is_valid_position(const Map *map, const Position *position)
{
    int width = map->width;
    int height = map->height;
    char **grid = map->grid;
    int x = position->x;
    int y = position->y;
    return x >= 0 && x < width && y >= 0 && y < height && grid[x][y] != 'x';
}

int is_goal(const Map *map, const Position *position)
{
    int x = position->x;
    int y = position->y;
    char **grid = map->grid;
    return grid[x][y] == 'g';
}

int is_zero(const Velocity *velocity)
{
    int vx = velocity->x;
    int vy = velocity->y;
    return vx == 0 && vy == 0;
}

Position *get_start_position(const Map *map)
{
    int width = map->width;
    int height = map->height;
    char **grid = map->grid;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (grid[x][y] == 's')
            {
                Position *start_position = malloc(sizeof(Position));
                start_position->x = x;
                start_position->y = y;
                return start_position;
            }
        }
    }
    return NULL;
}

Velocity *get_start_velocity()
{
    Velocity *start_velocity = malloc(sizeof(Velocity));
    start_velocity->x = 0;
    start_velocity->y = 0;
    return start_velocity;
}

void delete_map(Map *map)
{
    free(map->grid);
    for (int i = 0; i < map->nstarts; i++)
    {
        free(map->starts[i]);
    }
    for (int i = 0; i < map->ngoals; i++)
    {
        free(map->goals[i]);
    }
    free(map->starts);
    free(map->goals);
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
    delete_position(state->position);
    delete_velocity(state->velocity);
    free(state);
}

Map *get_map()
{
    int width = sizeof(CORNER_MAP) / sizeof(CORNER_MAP[0]);
    int height = sizeof(CORNER_MAP[0]) / sizeof(CORNER_MAP[0][0]);
    Map *map = malloc(sizeof(Map));
    map->width = width;
    map->height = height;
    map->nstarts = 0;
    map->ngoals = 0;
    int starts_size = 1;
    int goals_size = 1;
    map->starts = malloc(starts_size * sizeof(Position *));
    map->goals = malloc(goals_size * sizeof(Position *));
    char **grid = (char **)malloc(width * sizeof(char *));
    for (int x = 0; x < width; x++)
    {
        grid[x] = (char *)malloc(height * sizeof(char));
    }
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            grid[x][y] = CORNER_MAP[x][y];
            if (grid[x][y] == 's')
            {
                if (map->nstarts == starts_size)
                {
                    starts_size *= 2;
                    map->starts = realloc(map->starts, starts_size * sizeof(Position *));
                }
                Position *start = malloc(sizeof(Position));
                start->x = x;
                start->y = y;
                map->starts[map->nstarts] = start;
                map->nstarts += 1;
            }
            if (grid[x][y] == 'g')
            {
                if (map->ngoals == goals_size)
                {
                    goals_size *= 2;
                    map->goals = realloc(map->goals, goals_size * sizeof(Position *));
                }
                Position *goal = malloc(sizeof(Position));
                goal->x = x;
                goal->y = y;
                map->goals[map->ngoals] = goal;
                map->ngoals += 1;
            }
        }
    }
    map->grid = grid;
    return map;
}

float *get_feature_values(const Map *map, const State *state)
{
    float *feature_values = malloc(14 * sizeof(int));
    Position *position = get_position(state);
    feature_values[0] = (float)position->x;
    feature_values[1] = (float)position->y;
    Velocity *velocity = get_velocity(state);
    feature_values[2] = (float)velocity->x;
    feature_values[3] = (float)velocity->y;
    int i = 4;
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            if (x == 0 && y == 0)
            {
                continue;
            }
            Velocity direction = {x, y};
            feature_values[i] = (float)get_wall_distance(map, position, &direction);
            i++;
        }
    }
    Distance *goal_distance = get_goal_distance(map, position);
    feature_values[12] = (float)goal_distance->x;
    feature_values[13] = (float)goal_distance->y;
    return feature_values;
}
