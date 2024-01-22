#include <glut.h>  
#include <math.h>

//teapot rotation factor of the objects
float teapotRotation = 0.0;

// variables to move outermost Object Frame (to move all the rendered environment)
float moveX = 0.0f;
float moveY = 0.0f;
float moveZ = 0.0f;

GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat rotZ = 0.0f;

//transform camera
float camX = 0.0f;
float camY = 0.0f;
float camZ = 0.0f;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void drawAxes() {

    glBegin(GL_LINES);

    glLineWidth(1.5);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-20, 0, 0);
    glVertex3f(20, 0, 0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, -20, 0);
    glVertex3f(0, 20, 0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, -20);
    glVertex3f(0, 0, 20);

    glEnd();
}
void drawGrid() {
    GLfloat ext = 20.0f;
    GLfloat step = 1.0f;
    GLfloat yGrid = -0.4f;
    GLint line;

    glBegin(GL_LINES);
    for (line = -ext; line <= ext; line += step) {
        glVertex3f(line, yGrid, ext);
        glVertex3f(line, yGrid, -ext);

        glVertex3f(ext, yGrid, line);
        glVertex3f(-ext, yGrid, line);
    }
    glEnd();
}

void drawObject() {
    glColor3f(0.0, 1.0, 0.5);
    glutWireTeapot(2.0);
}


void display() {

    //clears the color and depth buffers
    //ensure clean slate for rendering next frame by removing the previous frame content
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //subsequent matrix operations will be applied to modelview matrix
    //modelview matrix - transforming objects from model space to view space
    glMatrixMode(GL_MODELVIEW);
    //loading identity matrix to current matrix (modelview matrix)
    //reset previous transformations and sets matrix to default
    glLoadIdentity();

    //pushes a copy of the current matrix onto the matrix stack
    //allow modifications without affecting the original matrix
    glPushMatrix();

    //configure view matrix for a camera perspective
    //sets up the camera position(eye), target point, and up vector
    gluLookAt(0.0 + camX, 2.0 + camY, 5.0 + camZ, 0, 0, 0, 0, 1.0, 0);
    glColor3f(1.0, 1.0, 1.0);

    glTranslatef(moveX, moveY, moveZ);
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

    drawGrid();

    drawAxes();

    glPushMatrix();

    //animate teapot rotation
    glRotatef(teapotRotation, 0.0, 1.0, 0.0);
    drawObject();

    glPopMatrix();

    glPopMatrix();

    //swapping the front and back buffers of the window
    glutSwapBuffers();

}

void keyboardSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        camY += 0.5;
    if (key == GLUT_KEY_DOWN)
        camY -= 0.5;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {

    if (key == 'z')
        moveZ += 1;

    if (key == 'Z')
        moveZ -= 1;

    if (key == 'l')
        rotY -= 5.0;

    if (key == 'r')
        rotY += 1.0;

    glutPostRedisplay();

}

//rotating teapot
void Timer(int x) {
   
    teapotRotation += teapotRotation >= 360.0 ? -teapotRotation : 2;
    glutPostRedisplay();

    glutTimerFunc(60, Timer, 1);
}

void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

    //sets the current matrix mode to the projection matrix
    //projection matrix --> to set the perspective or orthographic projection of the scene
    glMatrixMode(GL_PROJECTION);
    //loads the identity matrix into the current matrix(projection matrix)
    glLoadIdentity();

    //define the Perspective projection frustum 
    // (FOV_in_vertical, aspect_ratio, z-distance to the near plane from the camera position, z-distance to far plane from the camera position)
    gluPerspective(120.0, aspect_ratio, 1.0, 100.0);

}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    //
    // GLUT_DOUBLE
    // double-buffered display mode
    // one buffer is displayed on the screen while the other is being drawn
    //
    // GLUT_DEPTH
    // enables the depth buffer --> keep track of the depth (distance from the viewer) of each pixel
    //
    // GLUT_RGBA
    // framebuffer should be in RGBA (Red, Green, Blue, Alpha) mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("Rotating Teapot");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // keyboard function activation
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    
    //register a timer callback function
    //Timmer is the callback function that will be invoked when the timer expires
    //sets up a timer that will call the Timer function after every 60 milliseconds
    glutTimerFunc(60.0, Timer, 1);
    init();
    glutMainLoop();


    return 0;
}