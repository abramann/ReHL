#include "precompiled.h"

void GLBeginHud()
{
	qglViewport(glx, gly, glwidth, glheight);

	qglMatrixMode(GL_PROJECTION);
	qglPushMatrix();
	qglLoadIdentity();

	// TODO: probably should match the max render distance? - Solokiller
	qglOrtho(0, glwidth, glheight, 0, -99999, 99999);

	qglMatrixMode(GL_MODELVIEW);
	qglPushMatrix();
	qglLoadIdentity();

	qglDisable(GL_DEPTH_TEST);
	qglDisable(GL_CULL_FACE);
	qglDisable(GL_BLEND);
	qglEnable(GL_ALPHA_TEST);
	qglColor4f(1, 1, 1, 1);

	VGUI2_ResetCurrentTexture();
}

void GLFinishHud()
{
	qglMatrixMode(GL_PROJECTION);
	qglPopMatrix();

	qglMatrixMode(GL_MODELVIEW);
	qglPopMatrix();

	qglEnable(GL_DEPTH_TEST);
	qglEnable(GL_CULL_FACE);
	qglEnable(GL_BLEND);
}

void DrawWedge(float centerx, float centery, float angle1, float angle2, float radius)
{
	double forSin = 0, forCos = 0;

	qglBegin(GL_TRIANGLES);
	qglVertex2f(centerx, centery);
	forSin = sin(angle1 * 0.0174532925199433);
	forCos = cos(angle1 * 0.0174532925199433);
	qglVertex2f(centerx - forSin * radius, centery - forCos * radius);
	forSin = sin(angle2 * 0.0174532925199433);
	forCos = cos(angle2 * 0.0174532925199433);
	qglVertex2f(centerx - radius * forSin, centery - forCos * radius);
	qglEnd();
}
