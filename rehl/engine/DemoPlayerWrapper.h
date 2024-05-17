#pragma once

#include "ref_params.h"

bool DemoPlayer_Init();

bool DemoPlayer_IsActive();

int DemoPlayer_ReadDemoMessage(unsigned char *buffer, int size);

void DemoPlayer_ReadNetchanState(int *incoming_sequence, int *incoming_acknowledged, int *incoming_reliable_acknowledged, int *incoming_reliable_sequence, int *outgoing_sequence, int *reliable_sequence, int *last_reliable_sequence);

void DemoPlayer_GetDemoViewInfo(ref_params_t *rp, float *view, int *viewmodel);
