////////=================================//////
///////=================================//////
//////       Dimitrios Kapetanios.     //////                 
/////---------------------------------//////
////		AM: 01544            //////
///=================================//////	
//=================================//////
#include <string.h>			
#include <GL/glui.h>		//   GLUI (GLUT-based User Interface)  --->   GLUT (OpenGL Utility Toolkit).

float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;
int   main_window;

/** These are the live variables passed into GLUI ***/
int   wireframe = 0;
int   obj_type = 1;
int   light0_enabled = 1;
int   light1_enabled = 1;
float light0_intensity = 1.0;
float light1_intensity = .4;
float scale = 0.75;
int   show_pyramid = 1;
int   show_axes = 1;
int   show_text = 1;
float pyramid_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };	// 4x4 matrix stored in column-major order as 16 consecutive values.
float obj_pos[] = { 0.0, 0.0, 0.0 };
char *string_list[] = { "University of Thessaly", "Vasileios Drakopoulos PhD", "Computer Graphics", "Dimitrios Kapetanios 01544" };
int   curr_string = 0;

/** Pointers to the windows and some of the controls we'll create **/
GLUI *glui, *glui2;										// pointers to GLUI windows. 
GLUI_Spinner    *light0_spinner, *light1_spinner;		// controls to adjust the intensity of lights.
GLUI_RadioGroup *radio;									// group of radio buttons for selecting the object type.
GLUI_Panel      *obj_panel;								// a panel which groups related controls together.

/********** User IDs for callbacks ********/
// Set up integer constants that will be used as identifiers for various callback functions. 
// These identifiers are useful for determining which control has triggered a callback.

#define LIGHT0_ENABLED_ID    200
#define LIGHT1_ENABLED_ID    201
#define LIGHT0_INTENSITY_ID  250
#define LIGHT1_INTENSITY_ID  260
#define ENABLE_ID            300
#define DISABLE_ID           301
#define SHOW_ID              302
#define HIDE_ID              303


/********** Miscellaneous global variables **********/
// Ambient lighting color --> (general light in the scene). 
// Diffuse lighting color --> (directional light that bounces off surfaces).
// Position of the lights.

GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};

///GLfloat light0_ambient[] =  {0.5f, 0.25f, 0.0f, 1.0f}; // Slightly bright orange
//GLfloat light0_diffuse[] =  {1.0f, 0.5f, 0.0f, 1.0f}; // Bright orange
//GLfloat light0_position[] = {1.0f, 1.0f, 1.0f, 0.0f};
//GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};

GLfloat light1_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light1_diffuse[] =  {.9f, .6f, 0.0f, 1.0f};
GLfloat light1_position[] = {-1.0f, -1.0f, 1.0f, 0.0f};

// Rotation matrix for the lights.
GLfloat lights_rotation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };


/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */
// callback function that gets invoked when a GLUI control is interacted with by the user. 
// In this case, it is used to handle various UI interactions related to the lighting of the scene.

void control_cb( int control )	//The argument control is an integer identifier used to differentiate between the different controls.
{
  if ( control == LIGHT0_ENABLED_ID ) {	// Enables light source 0 and the control (spinner) related to light 0's intensity. Else disables them.
    if ( light0_enabled ) {
      glEnable( GL_LIGHT0 );
      light0_spinner->enable();
    }
    else {
      glDisable( GL_LIGHT0 ); 
      light0_spinner->disable();
    }
  }
  else if ( control == LIGHT1_ENABLED_ID ) {	// Same for light 1 .
    if ( light1_enabled ) {
      glEnable( GL_LIGHT1 );
      light1_spinner->enable();
    }
    else {
      glDisable( GL_LIGHT1 ); 
      light1_spinner->disable();
    }
  }
  else if ( control == LIGHT0_INTENSITY_ID ) {	// Multiplies the diffuse color of light 0 by the current intensity of light 0 and sets the new diffuse color.
    float v[] = { 
      light0_diffuse[0],  light0_diffuse[1],
      light0_diffuse[2],  light0_diffuse[3] };
    
    v[0] *= light0_intensity;
    v[1] *= light0_intensity;
    v[2] *= light0_intensity;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, v );
  }
  else if ( control == LIGHT1_INTENSITY_ID ) {	// Same for light 1.
    float v[] = { 
      light1_diffuse[0],  light1_diffuse[1],
      light1_diffuse[2],  light1_diffuse[3] };
    
    v[0] *= light1_intensity;
    v[1] *= light1_intensity;
    v[2] *= light1_intensity;

    glLightfv(GL_LIGHT1, GL_DIFFUSE, v );
  }
  else if ( control == ENABLE_ID )	//enables the glui2 window.
  {
    glui2->enable();
  }
  else if ( control == DISABLE_ID )	//disables the glui2 window.
  {
    glui2->disable();
  }
  else if ( control == SHOW_ID )	//makes the glui2 window visible.
  {
    glui2->show();
  }
  else if ( control == HIDE_ID )	//makes the glui2 window invisible.
  {
    glui2->hide();
  }
}

/**************************************** myGlutKeyboard() **********/
// This callback function is called when a key is pressed on the keyboard. 
// The pressed key is passed as an argument. 
// If the key pressed is the escape key (ascii value 27) or the letter 'q', the program exits. 
// After processing the keyboard input, it posts a redisplay event, which means it's requesting the window to be refreshed.

void myGlutKeyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
  case 27: 
  case 'q':
    exit(0);
    break;
  };
  
  glutPostRedisplay();
}


/***************************************** myGlutMenu() ***********/
// This callback function is called when a menu item is selected. 
// It merely calls myGlutKeyboard with the value of the menu item as the key. 
// This means that the menu items are processed the same way as key presses.

void myGlutMenu( int value )
{
  myGlutKeyboard( value, 0, 0 );
}


/***************************************** myGlutIdle() ***********/
// This callback function is called when there are no events to process. 
// It first checks whether the current GLUT window is the main window, and if not, it sets it to the main window. 
// Then it posts a redisplay event, causing the window to be refreshed.

void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window ) 
    glutSetWindow(main_window);  

  /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
                                       application  */

  glutPostRedisplay();
}

/***************************************** myGlutMouse() **********/
// This callback function is called when a mouse button is pressed or released. 

// button: 			This represents the mouse button that is being clicked. It can be GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.
// button_state: 	This represents the state of the button, which can be either GLUT_UP (button released) or GLUT_DOWN (button pressed).
// x, y: 			These represent the x and y coordinates of the mouse cursor at the time of the mouse event, in window-relative coordinates.

void myGlutMouse(int button, int button_state, int x, int y )
{
  if ( button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) {		// checks if the left mouse button has been pressed down
    last_x = x;
    last_y = y;
	
	// retrieves and stores the mouse coordinates
  }
}


/***************************************** myGlutMotion() **********/
// This callback function is called when the mouse moves while a button is pressed. 
// Again, it only posts a redisplay event.

void myGlutMotion(int x, int y )
{
  glutPostRedisplay(); 
}

/**************************************** myGlutReshape() *************/
// This function is called when the window is reshaped   (i.e. its dimensions are changed.)

void myGlutReshape( int x, int y )
{
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );	// gets the viewport area. 
  glViewport( tx, ty, tw, th );							// sets the viewport to the new area using. 

  xy_aspect = (float)tw / (float)th;					// updates aspect ratio of the window (width/height).

  glutPostRedisplay();									// posts a redisplay event.
}


/**************************************** drawPyramid() *************/
void drawPyramid() {
    glBegin(GL_TRIANGLES);	// delimit the vertices of a primitive or a group of like primitives (in this case; triangles).
    						//Initiates a sequence of triangles to be drawn. Each set of three vertices that follow will define a triangle.
    // Front facade
    //glColor3f(1.0f, 1.0f, 0.0f); // Yellow color
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Right facade
    //glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glVertex3f(0.0f, 1.0f, 0.0f);	//	
    glVertex3f(1.0f, -1.0f, 1.0f);	 //	Vertices
    glVertex3f(1.0f, -1.0f, -1.0f);	  //

    // Back facade
    //glColor3f(1.0f, 0.5f, 0.0f); // Orange color
    glVertex3f(0.0f, 1.0f, 0.0f);	  //
    glVertex3f(1.0f, -1.0f, -1.0f);	   // Vertices
    glVertex3f(-1.0f, -1.0f, -1.0f);	//

    // Left facade
    //glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glVertex3f(0.0f, 1.0f, 0.0f);	  //
    glVertex3f(-1.0f, -1.0f, -1.0f);   // Vertices
    glVertex3f(-1.0f, -1.0f, 1.0f);		//
    glEnd();

    // Base plain
    glBegin(GL_QUADS);	//Begins a new sequence of vertices, this time for a quadrilateral (the base of the pyramid).
    //glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glVertex3f(-1.0f, -1.0f, -1.0f);	//
    glVertex3f(1.0f, -1.0f, -1.0f);		// Vertices
    glVertex3f(1.0f, -1.0f, 1.0f);		//
    glVertex3f(-1.0f, -1.0f, 1.0f);		//
    glEnd();	// Ends the sequence of vertices. This tells OpenGL to draw the triangles now.
}


/************************************************** draw_axes() **********/
/* Disables lighting, then draws RGB axes                                */
// draw the XYZ axes in the scene.

void draw_axes( float scale )
{
  glDisable( GL_LIGHTING );				// disables lighting in the scene. 
   // This is done because the axes will be drawn in pure colors and we don't want lighting to affect their colors.

  glPushMatrix();						// saves the current transformation matrix. 
   // This is done because the following transformations should only apply to the axes, not to other elements in the scene.
   
  glScalef( scale, scale, scale );		// scales the model by the given factor. 
   // The factor is the same in all three dimensions, so the model is scaled uniformly.

  glBegin( GL_LINES );				// The following vertices will form lines. (read in couples)
 
  glColor3f( 1.0, 0.0, 0.0 );
  glVertex3f( .8f, 0.05f, 0.0 );  glVertex3f( 1.0, 0.25f, 0.0 ); /* Letter X */
  glVertex3f( 0.8f, .25f, 0.0 );  glVertex3f( 1.0, 0.05f, 0.0 );
  
  glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 1.0, 0.0, 0.0 ); /* X axis      */

  glColor3f( 0.0, 1.0, 0.0 );
  glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 1.0, 0.0 ); /* Y axis      */

  glColor3f( 0.0, 0.0, 1.0 );
  glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 0.0, 1.0 ); /* Z axis    */
  
  glEnd();						// indicates the end of vertex definitions.

  glPopMatrix();	// This function restores the transformation matrix that was saved with glPushMatrix(). 
   // This is done to ensure that the transformations applied to the axes don't affect other elements in the scene.

  glEnable( GL_LIGHTING );		// re-enables lighting in the scene after the axes have been drawn.
}


/***************************************** myGlutDisplay() *****************/
// A callback function that GLUT (OpenGL Utility Toolkit) calls whenever it needs to redraw the window. 
// The function is responsible for drawing the entire scene.

void myGlutDisplay( void )
{
  glClearColor( .9f, .9f, .9f, 1.0f );	// Sets the color that will be used to clear the color buffer, which is the buffer that stores the color of each pixel in the window. 
   // The color is set to a light gray (.9, .9, .9), with full opacity (1.0).
   
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// Clears the color and depth buffers. 
   // The depth buffer stores the depth (i.e., distance from the viewpoint) of each pixel in the window, and is used to handle visibility of 3D objects.

  glMatrixMode( GL_PROJECTION );	// Sets the current matrix mode to projection, which controls the camera's lens.
  glLoadIdentity();					// Resets the current matrix to the Identity matrix, which is the mathematical equivalent of no transformation.

  glFrustum( -xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0 );		// Defines a perspective projection matrix.
   // This sets up a perspective view, which mimics the way the human eye perceives depth and perspective.

  glMatrixMode( GL_MODELVIEW );		//  Sets the current matrix mode to modelview, which controls object transformations.
	
  glLoadIdentity();					// Resets the current matrix to the Identity matrix.
  glMultMatrixf( lights_rotation );						//  
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);	// Applies a transformation to the light source and sets its position.
  
//// Positions the objects in the 3D scene and applies transformations to them. 
  glLoadIdentity();											//This function resets the current matrix to the identity matrix. In this case, it clears all previous transformations and sets the point of view back to the origin (0, 0, 0) in the world coordinates. This is commonly used before applying a new set of transformations.
  glTranslatef( 0.0, 0.0, -2.6f );							//This function applies a translation (movement) transformation to the current matrix. It moves the point of view along the Z axis by -2.6 units, effectively moving the entire scene 2.6 units towards the viewer.
  glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 		//This applies another translation transformation, this time using the values in the obj_pos array. This moves the entire scene by obj_pos[0] units in the X direction, obj_pos[1] units in the Y direction, and -obj_pos[2] units in the Z direction.
  		//This function multiplies the current matrix by the view_rotate matrix. In the context of OpenGL, this typically applies a rotation transformation. The view_rotate matrix likely defines a rotation transformation, and applying it to the current matrix rotates the entire scene. The specific rotation depends on the values in the view_rotate matrix.
  glScalef( scale, scale, scale );							//Applies a scaling transformation to the current matrix.
   // The parameters of glScalef define how much the current drawing is scaled along the X, Y, and Z axes, respectively. In this case, all three parameters are set to the same value (scale), meaning the entire scene is uniformly scaled.
   // If scale is greater than 1, this will magnify the size of the objects in the scene. If scale is less than 1 but greater than 0, it will reduce the size of the objects. If scale equals 1, it will leave the objects' size unchanged.
   // It's worth noting that scaling, like all transformations in OpenGL, affects all subsequent drawing operations, not just the objects drawn immediately after it. The effect will remain until the current transformation matrix is changed or reset with glLoadIdentity().					
////
//------------------------------------------------------------------------------------------------

  /*** Now we render object, using the variables 'obj_type', 'segments', and
    'wireframe'.  These are _live_ variables, which are transparently 
    updated by GLUI ***/
    
 // The two glPushMatrix() and glPopMatrix() blocks draw a sphere and a torus (a doughnut shape), each with an optional wireframe, and the XYZ axes.
 
	// Pyramid
  glPushMatrix();	//This function saves the current transformation matrix by pushing it onto the stack. This allows us to modify the transformation matrix (with scaling, translation, or rotation operations), then later return it to its previous state.
  glTranslatef( 0.0, 0.0, -5.0 );	//Applies a translation transformation to the current matrix. Here, it moves the current drawing position to the left along the X-axis by 0.5 units. The Y and Z coordinates are unchanged.
  glMultMatrixf( pyramid_rotate );	//Multiplies the current transformation matrix by the matrix specified by pyramid_rotate. This effectively applies the transformation encoded in sphere_rotate to the current drawing position. pyramid_rotate is an array of 16 floating-point values representing a 4x4 transformation matrix.
  
  /*** Now we render object, using the variable 'wireframe' ***/
  if (wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  drawPyramid(); // Call pyramid-drawing function here.
  
  if ( show_axes )
    draw_axes( scale );	// This function call draws XYZ axes at the current position, with a length (or scale factor) of 0.52 units.
  
  glPopMatrix();	// Restores the transformation matrix to the state it was in before the last glPushMatrix() call. This essentially undoes all the transformations that have been applied since that call.
  
 // glut--Radius(innerRadius, outerRadius, nsides, rings)
  				//Inner radius of the torus.
							//Outer radius of the torus.
										//Number of sides for each radial section.
												//Number of radial divisions for the torus.
// Description: render a solid or wireframe torus (doughnut) respectively centered at the modeling coordinates origin whose axis is aligned with the Z axis.  
//---------------

  if ( show_text )  
  { 
    // Handles rendering of text in the scene. 
    // It sets up an orthographic projection, which is useful for 2D rendering (like text), 
	// sets the text color, position, and then renders the text character by character.
	
    glDisable( GL_LIGHTING );  /* Disable lighting while we render text */ //This call disables lighting calculations. Lighting is not typically used for rendering text in OpenGL as it wouldn't contribute to the readability of the text.
    glMatrixMode( GL_PROJECTION );	//This sets the current matrix mode to projection, which means subsequent matrix operations will affect the projection matrix stack. The projection matrix is used to transform 3D space into 2D screen space.
    glLoadIdentity();	//This resets the current matrix (which is the projection matrix due to the previous command) to the identity matrix. The identity matrix is a special matrix that, when multiplied with another matrix, doesn't change the other matrix.
    gluOrtho2D( 0.0, 100.0, 0.0, 100.0  );	//This creates an orthographic projection and multiplies it with the current matrix. Orthographic projections are linear projections where the 3D space is directly mapped to the 2D space, maintaining parallelism. The parameters specify the left, right, bottom, and top clipping planes, respectively.
    glMatrixMode( GL_MODELVIEW );	//This sets the current matrix mode back to model-view. The model-view matrix is used to transform all vertices and normals of objects from the model's local space to the view's eye space.
    glLoadIdentity();	// resets the model-view matrix to the identity matrix.
    glColor3ub( 0, 0, 0 );	//sets the current color to black. It is used to color the text that is going to be rendered.
    glRasterPos2i( 10, 10 );	//sets the position in the raster where pixel operations will take place. In this case, it's setting the position at which the text will start being drawn.

    /*  printf( "text: %s\n", text );              */

    /*** Render the live character array 'text' ***/
    int i;
    // This starts a loop over each character of the current string in the string list.
    for( i=0; i<(int)strlen( string_list[curr_string] ); i++ )
      glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, string_list[curr_string][i] );
    // prints a bitmap character at the current raster position. The character is specified by string_list[curr_string][i] and the font is Helvetica 18.
  }
  // This block of code essentially checks whether to display text. 
  // If text is to be displayed, it sets up an orthographic projection (useful for 2D rendering), 
  // sets the text color, position, and then renders the text character by character.
  

  glEnable( GL_LIGHTING );		// Re-enables lighting after the text rendering, as it was disabled before the text rendering.


  glutSwapBuffers(); 			// Swaps the front and back buffers. 
   // In double buffering, one buffer is shown while the other is being drawn into. 
   // When the drawing is complete, the buffers are swapped. This provides smooth animation because the viewer never sees incomplete or partial drawing.
}

////====================================================================================================================///
///====================================================================================================================///
//====================================================================================================================///
/**************************************** main() ********************/

int main(int argc, char* argv[])
{
  /****************************************/
  /*   Initialize GLUT and create window  */
  /****************************************/

  glutInit(&argc, argv);	//Initializes the GLUT library. This is typically the first GLUT command in any program.
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );	//RGB color, double buffering, and a depth buffer.
  glutInitWindowPosition( 50, 50 );	// Sets the initial position of the window, in pixels from the top left of the screen.
  glutInitWindowSize( 800, 600 );	// Sets the initial size of the window in pixels.
 
  main_window = glutCreateWindow( "Project 1." ); //Creates a window and returns its identifier. The string argument provides the window title.
  glutDisplayFunc( myGlutDisplay );	//Registers a callback function for when GLUT determines that the normal plane for the window needs to be redisplayed.
  // These lines are setting up GLUT and GLUI callback functions. These functions will be called when certain events occur, such as a key press, mouse movement, window resizing, etc.
  GLUI_Master.set_glutReshapeFunc( myGlutReshape );  
  GLUI_Master.set_glutKeyboardFunc( myGlutKeyboard );
  GLUI_Master.set_glutSpecialFunc( NULL );
  GLUI_Master.set_glutMouseFunc( myGlutMouse );
  glutMotionFunc( myGlutMotion );

  /****************************************/
  /*       Set up OpenGL lights           */
  /****************************************/
	// The series of glEnable, glLightfv commands are setting up two light sources in the scene. 
	// GL_LIGHT0 and GL_LIGHT1 are being configured with specific ambient, diffuse, and position properties.
	
  glEnable(GL_LIGHTING);
  glEnable( GL_NORMALIZE );

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);


  /****************************************/
  /*          Enable z-buferring          */
  /****************************************/
	//  Enables depth testing, which causes OpenGL to check, when drawing a pixel, whether it is behind or in front of what is already drawn.
	
  glEnable(GL_DEPTH_TEST);


  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/

  printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );

  /*** Create the side subwindow ***/
  glui = GLUI_Master.create_glui_subwindow( main_window, 
					    GLUI_SUBWINDOW_RIGHT );

  obj_panel = new GLUI_Rollout(glui, "Properties", true );

  /***** Control for object params *****/

  new GLUI_Checkbox( obj_panel, "Wireframe", &wireframe, 1, control_cb );
  
  GLUI_Spinner *scale_spinner = 
    new GLUI_Spinner( obj_panel, "Scale:", &scale);
  scale_spinner->set_float_limits( .2f, 2.0 );
  scale_spinner->set_alignment( GLUI_ALIGN_RIGHT );


  /******** Add some controls for lights ********/

  GLUI_Rollout *roll_lights = new GLUI_Rollout(glui, "Lights", false );

  GLUI_Panel *light0 = new GLUI_Panel( roll_lights, "Light 1" );
  GLUI_Panel *light1 = new GLUI_Panel( roll_lights, "Light 2" );
  

  new GLUI_Checkbox( light0, "Enabled", &light0_enabled,
                     LIGHT0_ENABLED_ID, control_cb );
  light0_spinner = 
    new GLUI_Spinner( light0, "Intensity:", 
                      &light0_intensity, LIGHT0_INTENSITY_ID,
                      control_cb );
  light0_spinner->set_float_limits( 0.0, 1.0 );
  GLUI_Scrollbar *sb;
  sb = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL,
                           &light0_diffuse[0],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL,
                           &light0_diffuse[1],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &light0_diffuse[2],LIGHT0_INTENSITY_ID,control_cb);
                           
                           
  sb->set_float_limits(0,1);
  new GLUI_Checkbox( light1, "Enabled", &light1_enabled,
                     LIGHT1_ENABLED_ID, control_cb );
  light1_spinner = 
    new GLUI_Spinner( light1, "Intensity:",
                      &light1_intensity, LIGHT1_INTENSITY_ID,
                      control_cb );
  light1_spinner->set_float_limits( 0.0, 1.0 );
  sb = new GLUI_Scrollbar( light1, "Red",GLUI_SCROLL_HORIZONTAL,
                           &light1_diffuse[0],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light1, "Green",GLUI_SCROLL_HORIZONTAL,
                           &light1_diffuse[1],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light1, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &light1_diffuse[2],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);


  /*** Add another rollout ***/
  GLUI_Rollout *options = new GLUI_Rollout(glui, "Options", true );
  new GLUI_Checkbox( options, "Draw axes", &show_axes );
  new GLUI_Checkbox( options, "Draw text", &show_text );

  /**** Add listbox ****/
  new GLUI_StaticText( glui, "" );
  GLUI_Listbox *list = new GLUI_Listbox( glui, "Text:", &curr_string );
  int i;
  for( i=0; i<4; i++ )
    list->add_item( i, string_list[i] );

  new GLUI_StaticText( glui, "" );


  /*** Disable/Enable buttons ***/
  new GLUI_Button( glui, "Disable movement", DISABLE_ID, control_cb );
  new GLUI_Button( glui, "Enable movement", ENABLE_ID, control_cb );
  new GLUI_Button( glui, "Hide taskbar", HIDE_ID, control_cb );
  new GLUI_Button( glui, "Show taskbar", SHOW_ID, control_cb );

  new GLUI_StaticText( glui, "" );

  /****** A 'quit' button *****/
  new GLUI_Button( glui, "Close window", 0,(GLUI_Update_CB)exit );


  /**** Link windows to GLUI, and register idle callback ******/
   // This makes sure GLUI knows which window is the main graphics window.
  glui->set_main_gfx_window( main_window );


  /*** Create the bottom subwindow ***/
  glui2 = GLUI_Master.create_glui_subwindow( main_window, 
                                             GLUI_SUBWINDOW_BOTTOM );
  glui2->set_main_gfx_window( main_window );

  GLUI_Rotation *pyr_rot = new GLUI_Rotation(glui2, "Pentahedron", pyramid_rotate );
  pyr_rot->set_spin( .98 );
  new GLUI_Column( glui2, false );
  
  GLUI_Rotation *lights_rot = new GLUI_Rotation(glui2, "Lighting", lights_rotation );
  lights_rot->set_spin( .82 );
  new GLUI_Column( glui2, false );
  
  GLUI_Translation *trans_xy = 
    new GLUI_Translation(glui2, "Plain (X-Y)", GLUI_TRANSLATION_XY, obj_pos );
  trans_xy->set_speed( .005 );
  new GLUI_Column( glui2, false );
  
  GLUI_Translation *trans_x = 
    new GLUI_Translation(glui2, "Axis red (X)", GLUI_TRANSLATION_X, obj_pos );
  trans_x->set_speed( .005 );
  new GLUI_Column( glui2, false );
  
  GLUI_Translation *trans_y = 
    new GLUI_Translation( glui2, "Axis blue (Y)", GLUI_TRANSLATION_Y, &obj_pos[1] );
  trans_y->set_speed( .005 );
  new GLUI_Column( glui2, false );
  
  GLUI_Translation *trans_z = 
    new GLUI_Translation( glui2, "Axis green (Z)", GLUI_TRANSLATION_Z, &obj_pos[2] );
  trans_z->set_speed( .005 );


#if 0
  /**** We register the idle callback with GLUI, *not* with GLUT ****/
  GLUI_Master.set_glutIdleFunc( myGlutIdle );
#endif


  /**** Regular GLUT main loop ****/
   // This enters the GLUT event processing loop. 
   // This function does not return until the program is terminated. It will repeatedly call any callbacks that have been registered as needed.
  glutMainLoop();


  return EXIT_SUCCESS;
}
