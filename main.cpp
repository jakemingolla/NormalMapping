/*  =================== File Information =================
	File Name: main.cpp
	Description:
	Author: Michael Shah
	Last Modified: 4/14/14

	Purpose: 
	Usage:	

	Further Reading resources: 
	===================================================== */

#include <string.h>
#include <iostream>
#include <fstream>
// You may need to include glee.h here if you are having problems
#include <GL/glew.h>
#include <GL/glui.h>
#include <math.h>
#include "shaderManager.h"

#define PI 3.14

/** These are the live variables passed into GLUI ***/
int main_window;
int wireframe = 0;
int filled = 1;
int grid = 0;

float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };

float xLightPos = 0.0;
float yLightPos = 0.0;
float zLightPos = 0.0;


// Pointer to widget
GLUI *glui2;


/* 	======== Shader Manager ========
	Setup a new shader manager
	============================== */
shaderManager* myShaderManager;


void drawSphere();

/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0);
	glTranslatef(0, 0, 0);

	glutPostRedisplay();
}



void draw_grid(){
	int grid_size = 10;
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-grid_size/2,0,-grid_size/2);
    // Draw a grid under the object
        for(int i = 0 ; i < grid_size; i=i+1){
            glBegin(GL_LINES);
                glColor3f( 1.0, 1.0, 1.0 );
                glVertex3f( 0.0, 0.0, i );  glVertex3f( grid_size, 0.0, i ); /* X axis grid */
                glVertex3f( i, 0.0, 0.0 );  glVertex3f( i, 0.0, grid_size ); /* X axis grid */
            glEnd();
        }
    
        glBegin(GL_LINES);
            glColor3f( 1.0, 1.0, 1.0 );
            glVertex3f( 0.0, 0.0, grid_size );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
            glVertex3f( grid_size, 0.0, 0.0 );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
        glEnd();
    glPopMatrix();
    glEnable( GL_LIGHTING );

}

/***************************************** myGlutDisplay() *****************/

// Draw a triangle that represents the light source
void drawLightSource(){
	xLightPos += 0.01f;
	yLightPos = 0.5;
	zLightPos += 0.01f;

	// Actual values that we calculate and send to the shader
	float xLightValue = sin(xLightPos);
	float yLightValue = yLightPos;
	float zLighValue = cos(zLightPos)+1;

	// Associate our uniform value 'light Vector' in our shader with these 3 values
	// the '3f' at the end means 'lightVector' is a vector of floats
	glUniform3f(glGetUniformLocation(myShaderManager->program,"lightVector"),xLightValue,yLightValue,zLighValue);

	glPushMatrix();
		glTranslatef(xLightValue,yLightValue,zLighValue);
		glColor3f(0.7,0.7,0);
		glBegin(GL_QUADS);
			glVertex3f(-.1,.1,0);
			glVertex3f(-.1,-.1,0);
			glVertex3f(.1,-.1,0);
			glVertex3f(.1,.1,0);
		glEnd();
	glPopMatrix();
}


void drawQuad(){

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	// Pass information to our shader to our shader
		int texture_location = glGetUniformLocation(myShaderManager->program,"brick_image");
		glUniform1i(texture_location,0);
	glBindTexture(GL_TEXTURE_2D,myShaderManager->layer1ID);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
		glUniform1i(glGetUniformLocation(myShaderManager->program,"bump_image"),1);
	glBindTexture(GL_TEXTURE_2D,myShaderManager->layer2ID);

	// Pass in the shader program id, which is 'program'		

	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(-1,1,0);
		glTexCoord2f(1,0);
		glVertex3f(-1,-1,0);
		glTexCoord2f(1,1);
		glVertex3f(1,-1,0);
		glTexCoord2f(0,1);
		glVertex3f(1,1,0);
	glEnd();

	//glutSolidCube(1.0);
	//drawSphere();

	glActiveTexture(GL_TEXTURE1);  
	glBindTexture(GL_TEXTURE_2D, 0);  
	glDisable(GL_TEXTURE_2D);  
	  
	glActiveTexture(GL_TEXTURE0);  
	glBindTexture(GL_TEXTURE_2D, 0);  
	glDisable(GL_TEXTURE_2D);

	drawLightSource();

}

void myGlutDisplay(void)
{

	static float scale = 0.1;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef(scale,scale,scale);
	glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2]-6 );
	glMultMatrixf(view_rotate);

		if (filled) {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.5, 0.5, 0.5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			drawQuad();
		}
		
		if (wireframe) {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.0, 0.0, 0.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPopMatrix();
			drawQuad();
		}

		if(grid){
			draw_grid();
		}

	glutSwapBuffers();
}

/*
	Reclaim memory we've allocated
*/
void onExit()
{
	delete myShaderManager;
}



/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 In Class Assignment 9");
	glutDisplayFunc(myGlutDisplay);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	GLUI_Master.set_glutSpecialFunc(NULL);

	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/

	glShadeModel(GL_SMOOTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	static float one[] = { 1, 1, 1, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);


	/****************************************/
	/*       Set up OpenGL Shaders          */
	/****************************************/

	GLenum err = glewInit();

	// Initialize our shader using our special shader manager class.
	myShaderManager = new shaderManager();
	
	myShaderManager->loadImage("brick.ppm",0);
	myShaderManager->loadImage("bump.ppm",1);

	myShaderManager->initShader("shaders/Bump/bump.vert","shaders/Bump/bump.frag");

	/****************************************/
	/*          Enable z-buferring          */
	/****************************************/

	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(1, 1);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	//GLUI *glui = GLUI_Master.create_glui("GLUI");

	/*** Create the bottom subwindow ***/
    glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
    // Create a rotation widget
    GLUI_Rotation *view_rot = new GLUI_Rotation(glui2, "Objects", view_rotate );
    view_rot->set_spin( 1.0 );

    // Navigate our world
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Objects X", GLUI_TRANSLATION_X, obj_pos );
    trans_x->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y =  new GLUI_Translation( glui2, "Objects Y", GLUI_TRANSLATION_Y, &obj_pos[1] );
    trans_y->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z =  new GLUI_Translation( glui2, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( .1 );

	glui2->add_column(true);

	GLUI_Panel *render_panel = glui2->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Filled", &filled);
	new GLUI_Checkbox(render_panel, "Grid", &grid);


	glui2->add_button("Quit", 0, (GLUI_Update_CB)exit);



	glui2->set_main_gfx_window(main_window);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);



	glutMainLoop();

	return EXIT_SUCCESS;
}



void drawSphere()
{
	float radius = 1.0f;
	float angle = 0;
	float angleH = -PI / (float)2.0;

	int m_segmentsX = 20;
	int m_segmentsY = 20;

	float angle_delta = 2.0 * PI / (float)m_segmentsX;
	float angleH_delta = PI / (float)m_segmentsY;

	glEnable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < m_segmentsY; i++) {
		angle = 0;
		for (int j = 0; j < m_segmentsX; j++) {
			// equation of a sphere
			float x = radius * cos(angleH) * cos(angle);
			float z = radius * cos(angleH) * sin(angle);
			float y = radius * sin(angleH);

			float newx = radius * cos(angleH) * cos(angle + angle_delta);
			float newz = radius * cos(angleH) * sin(angle + angle_delta);
			float newy = radius * sin(angleH);

			// Generate second triangle to make a quad
			float x_next = radius * cos(angleH + angleH_delta) * cos(angle);
			float z_next = radius * cos(angleH + angleH_delta) * sin(angle);
			float y_next = radius * sin(angleH + angleH_delta);

			float newx_next = radius * cos(angleH + angleH_delta) * cos(angle + angle_delta);
			float newz_next = radius * cos(angleH + angleH_delta) * sin(angle + angle_delta);
			float newy_next = radius * sin(angleH + angleH_delta);

			float textureCoordX_delta = (float)1 / (float)m_segmentsX;	// increments with j
			float textureCoordY_delta = (float)1 / (float)m_segmentsY; 	// increments with i

			float tx = 1 - j*textureCoordX_delta;// starting x pixel coordinate
			float ty = 1 - i*textureCoordY_delta;// starting y pixel coordinate
			float etx = 1 - (j + 1)*textureCoordX_delta;// ending x pixel coordinate
			float ety = 1 - (i - 1)*textureCoordY_delta;// ending y pixel coordinate

			glTexCoord2f(tx, ety); 		// glTexCoord2f(0.0f, 1.0f);
			glNormal3f(x, y, z);
			glVertex3f(x, y, z);

			glTexCoord2f(etx, ety); 		// glTexCoord2f(1.0f, 1.0f);
			glNormal3f(newx, newy, newz);
			glVertex3f(newx, newy, newz);

			glTexCoord2f(etx, ty); 		// glTexCoord2f(1.0f, 0.0f);
			glNormal3f(newx_next, newy_next, newz_next);
			glVertex3f(newx_next, newy_next, newz_next);

			glTexCoord2f(etx, ty); 		// glTexCoord2f(1.0f, 0.0f);
			glNormal3f(newx_next, newy_next, newz_next);
			glVertex3f(newx_next, newy_next, newz_next);

			glTexCoord2f(tx, ty); 		// glTexCoord2f(0.0f, 0.0f);
			glNormal3f(x_next, y_next, z_next);
			glVertex3f(x_next, y_next, z_next);

			glTexCoord2f(tx, ety); 		// glTexCoord2f(0.0f, 1.0f);
			glNormal3f(x, y, z);
			glVertex3f(x, y, z);


			angle = angle + angle_delta;
		}
		angleH = angleH + angleH_delta;
	}
	glEnd();
}

