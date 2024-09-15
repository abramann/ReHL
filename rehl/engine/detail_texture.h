#pragma once

void DT_Initialize();
void DT_ClearRenderState();
void DT_LoadDetailMapFile(char *levelName);
int DT_SetRenderState(int diffuseId);
void DT_SetTextureCoordinates(float u, float v);
