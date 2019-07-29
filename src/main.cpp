#include "main.h"
#include "timer.h"
#include "player.h"
#include "score.h"
#include "bricks.h"
#include "life.h"
#include "ring.h"
#include "coin.h"
#include "balloon.h"
#include "ice_balloon.h"
#include "powerups.h"
#include "magnet.h"
#include "enemy1_fireline.h"
#include "enemy2_parallelfireline.h"
#include "enemy3_boomerang.h"
#include "enemy4_boss.h"
using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

/* Remaining stuff
- Zoom 
All below are important
- Proper game construction
*/

Player player;
Score score,sc;
Life heart,hr;
Bricks brick[rows * num_bricks];

float screen_zoom = 1.0, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 90.0;
float x_speed = 0.0f;
int mag_flag = 0, done = 0, contact = 0, cur_life = 3, hold = 0, fi = 0;
float back_move_limit = 0.0f, magnify = 1.0f, curx = 0, cury = 0, cur_rad = 0;
std::vector<std::string> alpha, vec;
std::vector<Fire> flines;
std::vector<ParallelFire> plines;
std::vector<Balloon> balloon;
std::vector<Magnet> magnet;
std::vector<Powerup> power;
std::vector<Boomerang> boom;
std::vector<Coin> coin;
std::vector<Boss> boss;
std::vector<Ring> ring;
Timer t60(1.0 / 60);

bool balloon_fire(Balloon, Fire);
bool balloon_pfire(Balloon, ParallelFire);
bool player_coin(Coin);
bool player_power(Powerup);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw()
{
    // clear the color and depth in the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);
    glm::vec3 eye(0, 0, 1);
    // Eye - Location of camera. Don't change unless you are sure!!
    //    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target(0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up(0, 1, 0);

    // Compute Camera matrix (view)
    //    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane
    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;

    // Scene render

    for (int i = 0; i < rows * num_bricks; i++)
        brick[i].draw(VP);
    for (int i = 0; i < coin.size(); i++)
        coin[i].draw(VP);
    for (int i = 0; i < flines.size(); i++)
        flines[i].draw(VP);
    for (int i = 0; i < plines.size(); i++)
        plines[i].draw(VP);
    for (int i = 0; i < magnet.size(); i++)
        magnet[i].draw(VP);
    for (int i = 0; i < ring.size(); i++)
        ring[i].draw(VP);
    for (int i = 0; i < power.size(); i++)
        power[i].draw(VP);
    for (int i = 0; i < boom.size(); i++)
        boom[i].draw(VP);
    for (int i = 0; i < balloon.size(); i++)
        balloon[i].draw(VP);
    for (int i = 0; i < boss.size(); i++)
    {
        boss[i].draw(VP);
        for (int j = 0; j < boss[i].ice.size(); j++)
            boss[i].ice[j].draw(VP);
    }
    player.draw(VP);
    sc.draw(VP);
    score.draw(VP);
    hr.draw(VP);
    heart.draw(VP);
}

void fill_data()
{
    int sc = player.score;
    char s[] = "1011011", c[] = "1001110", o[] = "1111110", r[] = "1000110", e[] = "1001111", hy[] = "0000001";
    vec.push_back(s), vec.push_back(c), vec.push_back(o), vec.push_back(r), vec.push_back(e), vec.push_back(hy);
    int g[5], co = 0;
    while (sc > 0)
        g[co++] = sc % 10, sc /= 10;
    int k = co - 1;
    co = 4 - co;
    while (co > 0)
    {
        vec.push_back(alpha[0]);
        co--;
    }
    while (k >= 0)
    {
        vec.push_back(alpha[g[k]]);
        k--;
    }
}

void fire_collision()
{
    bounding_box_t b1, b2;
    b1.x = player.position.x, b1.y = player.position.y;
    b1.height = b1.width = 2.0f;
    for (int i = 0; i < plines.size(); i++)
    {
        b2.x = plines[i].position.x, b2.y = plines[i].position.y;
        b2.height = 0.1f, b2.width = 2.0f;
        if (detect_collision(b1, b2) && hold <= 0)
        {
            if (player.buff <= 0 && player.inv <= 0)
                player.life--, player.inv = immune;
            else if (player.buff > 0)
                plines.erase(plines.begin() + i);
        }
    }
    for (int i = 0; i < flines.size(); i++)
    {
        b2.x = flines[i].position.x, b2.y = flines[i].position.y;
        b2.height = 2.0f;
        b2.width = 0.1f;
        if (detect_collision(b1, b2) && hold <= 0)
        {
            if (player.buff <= 0 && player.inv <= 0)
                player.life--, player.inv = immune;
            else if (player.buff > 0)
                flines.erase(flines.begin() + i);
        }
    }
    for (int j = 0; j < balloon.size(); j++)
    {
        if (balloon[j].status == false)
            continue;
        b1.x = balloon[j].position.x, b1.y = balloon[j].position.y;
        b1.height = b1.width = 0.05f;
        for (int i = 0; i < plines.size(); i++)
        {
            b2.x = plines[i].position.x, b2.y = plines[i].position.y;
            b2.height = 0.1f, b2.width = 2.0f;
            if (balloon_pfire(balloon[j], plines[i]))
                plines[i].status = false, balloon.erase(balloon.begin() + j);
        }
    }
    for (int j = 0; j < balloon.size(); j++)
    {
        if (balloon[j].status == false)
            continue;
        b1.x = balloon[j].position.x, b1.y = balloon[j].position.y;
        b1.height = b1.width = 0.05f;
        for (int i = 0; i < flines.size(); i++)
        {
            b2.x = flines[i].position.x, b2.y = flines[i].position.y;
            b2.height = 0.1f, b2.width = 2.0f;
            if (balloon_fire(balloon[j], flines[i]))
                flines[i].status = false, balloon.erase(balloon.begin() + j);
        }
    }
}
void boss_collision()
{
    bounding_box_t b1, b2;
    b1.x = player.position.x, b1.y = player.position.y;
    b1.height = b1.width = 1.0f;
    for (int i = 0; i < boss.size(); i++)
    {
        b2.x = boss[i].position.x, b2.y = boss[i].position.y;
        b2.height = 1.0f, b2.width = 1.0f;
        if (detect_collision(b1, b2) && hold <= 0)
        {
            if (player.buff > 0)
                player.score += 10, boss.erase(boss.begin() + i);
            else if (player.inv > 0)
                ;
            else
                player.life--, player.inv = immune;
        }
        for (int j = 0; j < boss[i].ice.size(); j++)
        {
            b2.x = boss[i].ice[j].position.x, b2.y = boss[i].ice[j].position.y;
            b2.height = b2.width = 0.1f;
            if (detect_collision(b1, b2))
            {
                if (player.buff <= 0 && player.inv <= 0)
                    player.life--, player.inv = immune, boss[i].ice.erase(boss[i].ice.begin() + j);
            }
        }
    }
}

// Correct this part
void coin_collection()
{
    for (int i = 0; i < coin.size(); i++)
    {
        if (player_coin(coin[i]) && coin[i].status == true)
            player.score += coin[i].val, coin[i].status = false;
    }
}

void power_collision()
{
    for (int i = 0; i < power.size(); i++)
    {
        if (player_power(power[i]))
        {
            if (power[i].type == 0)
                player.score += 50;
            else if (power[i].type == 1)
                player.life = min(player.life + 1, max_life);
            else
                player.buff = 600;
            power.erase(power.begin() + i);
        }
    }
}

void boomerang_collision()
{
    bounding_box_t b1, b2;
    b1.x = player.position.x, b1.y = player.position.y;
    b1.height = b1.width = 1.0f;
    for (int i = 0; i < boom.size(); i++)
    {
        b2.x = boom[i].position.x, b2.y = boom[i].position.y;
        b2.height = 2.0f, b2.width = 1.0f;
        if (detect_collision(b1, b2) && hold <= 0)
        {
            if (player.buff <= 0 && player.inv <= 0)
                player.life--, player.inv = immune;
            else if (player.buff > 0)
                boom.erase(boom.begin() + i);
        }
    }
}

void enter_ring()
{
    hold = max(hold - 1, 0);
    bounding_box_t b1, b2;
    b1.x = player.position.x, b1.y = player.position.y;
    b1.height = b1.width = 1.0f;
    for (int i = 0; i < ring.size(); i++)
    {
        if (hold > 0)
            continue;
        b2.x = ring[i].position.x - (ring[i].in + ring[i].out) / 2, b2.y = ring[i].position.y;
        b2.width = 1.0f, b2.height = 1.0f;
        if (detect_collision(b1, b2))
        {
            hold = 60;
            player.x_speed = player.y_speed = 0.0f;
            player.angle = M_PI;
            cur_rad = ring[i].out;
            player.position.x = ring[i].position.x - (ring[i].in + ring[i].out) / 2;
            player.position.y = ring[i].position.y;
            curx = ring[i].position.x, cury = ring[i].position.y;
        }
    }
}

void enter_magnet()
{
    for (int i = 0; i < magnet.size(); i++)
    {
        if (abs(player.position.x - magnet[i].position.x) <= 2.0f)
        {
            if (player.position.x - magnet[i].position.x <= 0)
                mag_flag = -1;
            else
                mag_flag = 1;
            back_move_limit = 0.0f;
            double dist = abs(player.position.x - magnet[i].position.x) + abs(player.position.y - magnet[i].position.y);
            double accn = 1.0 / (dist * dist + 5);
            accn /= 10;
            double ang = atan((player.position.y - magnet[i].position.y) / (player.position.x - magnet[i].position.x));
            if (mag_flag == -1)
                player.x_speed += accn * cos(ang);
            else
            {
                player.x_speed -= accn * cos(ang);
            }
            if (player.position.y > magnet[i].position.y)
                player.y_speed -= mag_flag * accn * sin(ang);
            else
            {
                player.y_speed += accn * sin(ang);
            }
            bounding_box_t b1, b2, b3;
            b1.x = player.position.x, b1.y = player.position.y;
            b1.height = b1.width = 2.0f;
            b2.x = magnet[i].position.x - magnet[i].out, b2.y = magnet[i].position.y - magnet[i].out;
            b2.height = 0.25f, b2.width = 0.25f;
            b3.x = magnet[i].position.x - magnet[i].out, b3.y = magnet[i].position.y + magnet[i].out;
            b3.height = b3.width = b2.height;
            if (detect_collision(b1, b2) || detect_collision(b1, b3) && contact == 0)
                player.y_speed = 0.0f, contact = 1;
        }
        else
            mag_flag = 0, back_move_limit = 0.04f, contact = 0;
    }
}

void tick_input(GLFWwindow *window)
{
    if (hold)
        return;
    int left = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int mouse = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    int f = 0;

    if (left)
    {
        player.x_speed -= 0.25f / 60;
        f = -1;
    }
    else if (right)
    {
        player.x_speed += 0.25f / 60;
        f = 1;
    }
    else
    {
        player.x_speed = 0.0f;
    }
    if (f && mag_flag)
        player.x_speed += f * 0.02f;
    if (up)
    {
        if (done == 1)
            player.y_speed = 0.0f, done = 0;
        player.y_speed += 0.35f / 60;
        player.flag = 1;
    }
    else
    {
        if (done == 0)
            player.y_speed = -0.01f, done = 1;
        player.flag = 0;
    }
    if (space && player.cd <= 0)
    {
        // cout<<"asd";
        // cout<<balloon.size()<<endl;
        player.cd = 60;
        Balloon t = Balloon(player.position.x + 1.0f, player.position.y + 1.0f, COLOR_BLACK);
        balloon.push_back(t);
        // cout<<balloon.size()<<endl;
    }
}

void tick_elements()
{
    color_t c1 = {18,17,19};
    color_t c2 = {74,82,90};
    color_t c = {205, 16, 93};
    color_t d = {7,7,10};
    score = Score(0.0, 3.8/screen_zoom, c1);
    sc = Score(0.05/screen_zoom,3.74/screen_zoom,c2);
    heart = Life(-3.5/screen_zoom, 3.5/screen_zoom,c);
    hr = Life(-3.4/screen_zoom,3.4/screen_zoom,d);
    cur_life = player.life;
    player.tick();
    hr.tick();
    heart.tick();
    score.tick();
    sc.tick();
    for (int i = 0; i < rows * num_bricks; i++)
        brick[i].tick();
    for (int i = 0; i < coin.size(); i++)
        coin[i].tick();
    for (int i = 0; i < plines.size(); i++)
        plines[i].tick();
    for (int i = 0; i < flines.size(); i++)
        flines[i].tick();
    for (int i = 0; i < ring.size(); i++)
        ring[i].tick();
    for (int i = 0; i < magnet.size(); i++)
    {
        if (magnet[i].position.x + magnet[i].w <= left_limit)
            magnet.erase(magnet.begin() + i);
        else
            magnet[i].tick();
    }
    for (int i = 0; i < power.size(); i++)
        power[i].tick();
    for (int i = 0; i < balloon.size(); i++)
    {
        cout << balloon[i].position.x << " " << balloon[i].position.y << endl;
        if (balloon[i].status == false)
            balloon.erase(balloon.begin() + i);
        else
            balloon[i].tick();
    }
    for (int i = 0; i < boss.size(); i++)
    {
        if (boss[i].cd <= 0)
        {
            boss[i].cd = 300;
            Ice p = Ice(boss[i].position.x + 0.5f, boss[i].position.y + 0.5f);
            boss[i].ice.push_back(p);
        }
        boss[i].tick();
        for (int j = 0; j < boss[i].ice.size(); j++)
        {
            if (boss[i].position.x < player.position.x)
                boss[i].ice[j].x_speed = 0.05f;
            else
                boss[i].ice[j].x_speed = -0.05f;
            if (boss[i].ice[j].status == false)
                boss[i].ice.erase(boss[i].ice.begin() + j);
            else
                boss[i].ice[j].tick();
        }
    }
    for (int i = 0; i < boom.size(); i++)
        boom[i].tick();
    screen_zoom = magnify;
    reset_screen();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height)
{

    //Generating player
    color_t c1 = {18,17,19};
    color_t c2 = {215,255,241};
    score = Score(0.0, 3.8/screen_zoom, c1);
    sc = Score(0.05/screen_zoom,3.74/screen_zoom,c2);
    player = Player(-2.0, -3.5);
    heart = Life(-3.5, 3.5,{205, 16, 93});
    hr = Life(-3.4,3.4,{7,7,10});
    //Generating floor/platform
    int k = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < num_bricks; j++)
        {
            brick[k++] = Bricks(-4.25 + 0.5 * j, -3.8 + 0.15 * i, COLOR_ARRAY[k % 4]);
            brick[k - 1].speed = player.x_speed;
        }
    }
    float prev;

    //Generating coins
    for (int i = 0; i < num_coins; i++)
    {
        Coin c = Coin(rand() % 400, rand() % 3, rand()%2);
        coin.push_back(c);
    }

    //Generating parallel firelines;
    prev = 5.0f;
    for (int i = 0; i < num_plines; i++)
    {
        int p = rand() % 7;
        ParallelFire f = ParallelFire(prev+10.0f+p, rand() % 2);
        plines.push_back(f);
        prev += 10.0f+p;
    }
    // //Generating vertical/rotated firelines
    prev = 10.0f;
    for (int i = 0; i < num_flines; i++)
    {
        int p = rand() % 6;
        Fire f = Fire(prev+10.0f+p, rand() % 2, M_PI / (1 + rand() % 10));
        flines.push_back(f);
        prev += 10.0f + p;
    }
    // //Generating magnets
    prev = 15.0f;
    for (int i = 0; i < num_mag; i++)
    {
        int p = rand() % 8;
        Magnet l = Magnet(prev + 10.0f + p, rand() % 2);
        magnet.push_back(l);
        prev += 10.0f + p;
    }
    // //Generating boomerangs
    prev = 18.0f;
    for (int i = 0; i < num_boom; i++)
    {
        int p = rand() % 15;
        Boomerang b = Boomerang(prev+10.0f+p, 0.0f);
        boom.push_back(b);
        prev += 10.0f+p;
    }
    // //Generating powerups
    prev = 10.0f;
    for (int i = 0; i < num_pow; i++)
    {
        int p = rand() % 25;
        Powerup g = Powerup(prev + 10.0f + p, rand() % 2, rand() % 3);
        power.push_back(g);
        prev += 10.0f + p;
    }

    // //Generating rings
    prev = 25.0f;
    for (int i = 0; i < num_ring; i++)
    {
        int p = rand() % 10;
        Ring r = Ring(prev, 0.0f);
        ring.push_back(r);
        prev += p;
    }
    // //Generating boss
    prev = 30.0f;
    for (int i = 0; i < num_boss; i++)
    {
        int p = rand() % 15;
        Boss b = Boss(prev, 0.0f);
        boss.push_back(b);
        prev += p;
    }
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow(window, width, height);

    // Background color of the scene
    glClearColor(COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv)
{
    srand(time(0));
    int width = 1080;
    int height = 1080;
    player.life = 3;
    alpha.push_back("1111110"); //0
    alpha.push_back("0110000"); //1
    alpha.push_back("1101101"); //2
    alpha.push_back("1111001"); //3
    alpha.push_back("0110011"); //4
    alpha.push_back("1011011"); //5
    alpha.push_back("1011111"); //6
    alpha.push_back("1110000"); //7
    alpha.push_back("1111111"); //8
    alpha.push_back("1111011"); //9
    window = initGLFW(width, height);
    initGL(window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process timers
        if (t60.processTick())
        {
            // 60 fps
            // OpenGL Draw commands
            vec.clear();
            fill_data();
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);
            coin_collection();
            fire_collision();
            power_collision();
            boomerang_collision();
            boss_collision();
            enter_magnet();
            enter_ring();
            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}
float transx(float x, float y, double ang)
{
    return (x * cos(ang) - y * sin(ang));
}
float transy(float x, float y, double ang)
{
    return (x * sin(ang) + y * cos(ang));
}
float dist(float x1, float y1, float x2, float y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
bool detect_collision(bounding_box_t a, bounding_box_t b)
{
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen()
{
    float top = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left = screen_center_x - 4 / screen_zoom;
    float right = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

bool balloon_fire(Balloon b, Fire fl)
{
    float x = b.position.x, y = b.position.y, u = fl.position.x, v = fl.position.y;
    if (abs(y - v) < (b.rad + 1.2) && abs(x - u) < (b.rad + 0.125))
        return 1;
    else
        return 0;
}
bool balloon_pfire(Balloon b, ParallelFire fl)
{
    float x = b.position.x, y = b.position.y;
    if (y >= fl.position.y - 0.1f - fl.rad && y <= fl.position.y - 0.1f + fl.rad &&
        x >= fl.position.x - 0.2f - fl.rad && x <= fl.position.x + 2.2f + fl.rad)
        return 1;
    else
        return 0;
}

bool player_coin(Coin c)
{
    float x = c.position.x, y = c.position.y;
    float u = player.position.x + 0.5f, v = player.position.y + 0.5f;
    if (abs(x - u) < c.rad + 0.5f && abs(y - v) < c.rad + 0.5f)
        return 1;
    else
        return 0;
}

bool player_power(Powerup c)
{
    float x = c.position.x, y = c.position.y;
    float u = player.position.x + 0.5f, v = player.position.y + 0.5f;
    if (abs(x - u) < c.radius + 0.5f && abs(y - v) < c.radius + 0.5f)
        return 1;
    else
        return 0;
}
