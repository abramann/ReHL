/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/

#include "precompiled.h"
#include "gl_rsuf.h"
#include "cl_pred.h"
#include "cl_ents.h"

cl_enginefunc_dst_t g_engdstAddrs = k_engdstNull;
client_static_t g_pcls;
client_state_t g_pcl;

qboolean cl_inmovie;

cvar_t console = { "console", "1.0", FCVAR_ARCHIVE, 0.0f, NULL };

void CL_RecordHUDCommand(const char *cmdname) { NOT_IMPLEMENTED; }
qboolean CL_CheckFile(sizebuf_t *msg, char *filename) { return 1; }
void CL_DecayLights(void) { NOT_IMPLEMENTED; }
void CL_InitClosest(void) { NOT_IMPLEMENTED; }
void CL_PrintLogos(void) { NOT_IMPLEMENTED; }
void CL_UpdateSoundFade(void) { NOT_IMPLEMENTED; }
void CL_AdjustClock(void) { NOT_IMPLEMENTED; }
void Con_Shutdown(void) { NOT_IMPLEMENTED; }
int DispatchDirectUserMsg(const char *pszName, int iSize, void *pBuf) { NOT_IMPLEMENTED; return 0; }
void EXT_FUNC Con_NPrintf(int idx, const char *fmt, ...) { NOT_IMPLEMENTED; }
void Sequence_OnLevelLoad(const char* mapName) { NOT_IMPLEMENTED; }
void CL_WriteMessageHistory(int starting_count, int cmd) { NOT_IMPLEMENTED; }
void CL_AddVoiceToDatagram(qboolean bFinal) { NOT_IMPLEMENTED; }
void CL_VoiceIdle(void) { NOT_IMPLEMENTED; }
void PollDInputDevices(void) { NOT_IMPLEMENTED; }
void CL_KeepConnectionActive(void) { NOT_IMPLEMENTED; }
void CL_UpdateModuleC(void) { NOT_IMPLEMENTED; }
void EXT_FUNC ConstructTutorMessageDecayBuffer(int *buffer, int bufferLength) { NOT_IMPLEMENTED; }
void EXT_FUNC ProcessTutorMessageDecayBuffer(int *buffer, int bufferLength) { NOT_IMPLEMENTED; }
int EXT_FUNC GetTimesTutorMessageShown(int id) { return -1; }
void EXT_FUNC RegisterTutorMessageShown(int mid) { NOT_IMPLEMENTED; }
void EXT_FUNC ResetTutorMessageDecayData(void) { NOT_IMPLEMENTED; }
void SetCareerAudioState(int state) { NOT_IMPLEMENTED; }

void VguiWrap2_GetMouseDelta(int *x, int *y) { NOT_IMPLEMENTED; }