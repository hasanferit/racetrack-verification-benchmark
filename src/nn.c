#include <stdio.h>
#include <stdlib.h>
#include <tensorflow/c/c_api.h>
#include <string.h>

#include "nn.h"
#include "racetrack.h"

#define INPUT_SIZE 14
#define OUTPUT_SIZE 9

struct NNModel
{
    TF_Graph *graph;
    TF_Session *session;
    TF_Status *status;
};

struct NNInput
{
    TF_Tensor **values;
};

NNModel *load_nn_model(const char *filename)
{

    /* computation graph */
    TF_Graph *graph = TF_NewGraph();
    /* holds error information */
    TF_Status *status = TF_NewStatus();
    /* options that can be passed at session creation */
    TF_SessionOptions *session_opts = TF_NewSessionOptions();
    TF_Buffer *run_opts = NULL;
    const char *tags = "serve";
    int tags_len = 1;

    TF_Session *session = TF_LoadSessionFromSavedModel(session_opts, run_opts, filename,
                                                       &tags, tags_len, graph, NULL, status);

    NNModel *nn_model = malloc(sizeof(NNModel));
    nn_model->graph = graph;
    nn_model->session = session;
    nn_model->status = status;
    return nn_model;
}

/* TODO free memory */
NNInput *get_nn_input(const Map *map, const State *state, const NNModel *nn_model)
{
    const int64_t dims[2] = {1, INPUT_SIZE};
    const size_t ndata = INPUT_SIZE * sizeof(float);
    float *feature_values = get_feature_values(map, state);

    TF_Tensor *tensor = TF_AllocateTensor(TF_FLOAT, dims, 2, ndata);
    memcpy(TF_TensorData(tensor), feature_values, ndata);
    TF_Tensor **input_values = malloc(sizeof(TF_Tensor *));
    input_values[0] = tensor;

    NNInput *nn_input = malloc(sizeof(NNInput));
    nn_input->values = input_values;
    return nn_input;
}

/* TODO free memory */
Acceleration *call_nn_model(const NNModel *nn_model, const NNInput *nn_input)
{

    TF_Operation *input_operation = TF_GraphOperationByName(nn_model->graph, "main/input");
    TF_Output input[1] = {input_operation, 0};

    TF_Operation *output_operation = TF_GraphOperationByName(nn_model->graph, "main/output/BiasAdd");
    TF_Output output[1] = {output_operation, 0};

    TF_Tensor **input_values = nn_input->values;

    TF_Tensor *output_values[1] = {NULL};

    TF_SessionRun(nn_model->session, NULL, input, input_values, 1, output, output_values, 1, NULL, 0, NULL, nn_model->status);

    float *q_values = (float *)malloc(OUTPUT_SIZE * sizeof(float));
    memcpy(q_values, TF_TensorData(output_values[0]), OUTPUT_SIZE * sizeof(float));

    int max_q_value_index = 0;
    float max_q_value = q_values[max_q_value_index];
    for (int i = 0; i < OUTPUT_SIZE; i++)
    {
        if (q_values[i] > max_q_value)
        {
            max_q_value = q_values[i];
            max_q_value_index = i;
        }
    }
    int ax = (max_q_value_index / 3) - 1;
    int ay = (max_q_value_index % 3) - 1;
    return create_acceleration(ax, ay);
}

void delete_nn_model(NNModel *nn_model)
{
    TF_DeleteGraph(nn_model->graph);
    TF_DeleteSession(nn_model->session, nn_model->status);
    TF_DeleteStatus(nn_model->status);
    free(nn_model);
}

/* TODO implement this function */
void delete_nn_input(NNInput *nn_input)
{
}
