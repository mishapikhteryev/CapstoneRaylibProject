#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

#define RAND_COLUMNS 30
#define STAGE_BLOCKS 5
#define BARRELS 10
#define BARREL_HEIGHT 1.5f;
#define BARREL_RADIUS 0.5f;
#define ROCKET_CAP 50
#define PROJ_RADIUS 0.1f
#define PROJ_LIFETIME 4
#define PROJ_VEL 20.0f
#define PROJ_DELAY 40
#define PROJ_COLOR WHITE
#define PLAYER_GUN_LEN 0.5f

#define V3_FloatFormat "{%f, %f, %f}"
#define V3_Pos(v3) v3.x, v3.y, v3.z

// GameScreen stores the current state of the program
typedef enum GameScreen
{ 
    TITLE = 0,
    GAMEPLAY, 
} GameScreen;


// Rocket is a rocket
typedef struct Rocket
{
    float lifetime;
    Vector3 pos;
    Vector3 dir;
} Rocket;

Rocket rockets[ROCKET_CAP] = {0};


// blocks
typedef struct Block
{
    Vector3 position;
    float width;
    float height;
    float length;
    Color color;
    
} Block;

typedef struct Barrel
{
    Vector3 position;
    float radius;
    float height;
    Color color;
    int pointScore;
} Barrel;

Block randBlocks[RAND_COLUMNS] = {0};
Block blocks[STAGE_BLOCKS] = {0};
Barrel barrels[BARRELS] = {0};

// ------------------------------------------------ functions ---------------------------------------------------

void moveBarrel(int i)
{
    int C = GetRandomValue(0,100);
    int x = (float)GetRandomValue(-15,15);
    int y = 0;
    int z = (float)GetRandomValue(-15,15);
    
    switch(C%10){
        case 1:
            barrels[i].color = MAROON;
            barrels[i].pointScore = 1;
            break;
        case 2:
            barrels[i].color = MAROON;
            barrels[i].pointScore = 1;
            break;
        case 3:
            barrels[i].color = MAROON;
            barrels[i].pointScore = 1;
            break;
        case 4:
            barrels[i].color = MAROON;
            barrels[i].pointScore = 1;
            break;
        case 5:
            barrels[i].color = LIME;
            barrels[i].pointScore = 2;
            break;
        case 6:
            barrels[i].color = LIME;
            barrels[i].pointScore = 2;
            break;
        case 7:
            barrels[i].color = LIME;
            barrels[i].pointScore = 2;
            break;
        case 8:
            barrels[i].color = VIOLET;
            barrels[i].pointScore = 3;
            break;
        case 9:
            barrels[i].color = VIOLET;
            barrels[i].pointScore = 3;
            break;
        case 0:
            barrels[i].color = GOLD;
            barrels[i].pointScore = 5;
            break;
        default:
            barrels[i].color = MAROON;
            barrels[i].pointScore = 1;
            break;
    } // switch
    
    barrels[i].position.x = x;
    barrels[i].position.y = y;
    barrels[i].position.z = z;
    
        for (int j = 0; j < STAGE_BLOCKS; j++){
            if (barrels[i].position.x - barrels[i].radius < blocks[j].position.x + blocks[j].width/2.0f &&
                barrels[i].position.x + barrels[i].radius > blocks[j].position.x - blocks[j].width/2.0f &&
                barrels[i].position.y - barrels[i].height < blocks[j].position.y + blocks[j].height/2.0f &&
                barrels[i].position.y + barrels[i].height > blocks[j].position.y - blocks[j].height/2.0f &&
                barrels[i].position.z - barrels[i].radius < blocks[j].position.z + blocks[j].length/2.0f &&
                barrels[i].position.z + barrels[i].radius > blocks[j].position.z - blocks[j].length/2.0f) 
                {
                    barrels[i].position.y = blocks[j].position.y + blocks[j].height/2.0f;
                }
        }
        
        for (int j = 0; j < RAND_COLUMNS; j++){
            if (barrels[i].position.x - barrels[i].radius < randBlocks[j].position.x + randBlocks[j].width/2.0f &&
                barrels[i].position.x + barrels[i].radius > randBlocks[j].position.x - randBlocks[j].width/2.0f &&
                barrels[i].position.y - barrels[i].height < randBlocks[j].position.y + randBlocks[j].height/2.0f &&
                barrels[i].position.y + barrels[i].height > randBlocks[j].position.y - randBlocks[j].height/2.0f &&
                barrels[i].position.z - barrels[i].radius < randBlocks[j].position.z + randBlocks[j].length/2.0f &&
                barrels[i].position.z + barrels[i].radius > randBlocks[j].position.z - randBlocks[j].length/2.0f) 
                {
                    barrels[i].position.y = randBlocks[j].position.y + randBlocks[j].height/2.0f;
                }
        }
}

// camera direction
Vector3 camera_direction(Camera *camera)
{
    return Vector3Normalize(Vector3Subtract(camera->target, camera->position));
}

void spawn_rocket(Vector3 pos, Vector3 dir)
{
    for (int i = 0; i < ROCKET_CAP; i++) {
        if (rockets[i].lifetime <= 0.0f) {
            rockets[i].lifetime = PROJ_LIFETIME;
            rockets[i].pos = pos;
            rockets[i].dir = dir;
            break;
        }
    }
}

int update_rockets()
{
    const float dt = GetFrameTime();
    
    int points = 0;
    
    for (int i = 0; i < ROCKET_CAP; i++) {
        if (rockets[i].lifetime > 0.0f) {
            rockets[i].lifetime -= dt;
            rockets[i].pos = Vector3Add(
                rockets[i].pos,
                Vector3Scale(rockets[i].dir, dt));
        }
        
        for (int j = 0; j < STAGE_BLOCKS; j++){
            if (rockets[i].pos.x - PROJ_RADIUS < blocks[j].position.x + blocks[j].width/2.0f &&
                rockets[i].pos.x + PROJ_RADIUS > blocks[j].position.x - blocks[j].width/2.0f &&
                rockets[i].pos.y - PROJ_RADIUS < blocks[j].position.y + blocks[j].height/2.0f &&
                rockets[i].pos.y + PROJ_RADIUS > blocks[j].position.y - blocks[j].height/2.0f &&
                rockets[i].pos.z - PROJ_RADIUS < blocks[j].position.z + blocks[j].length/2.0f &&
                rockets[i].pos.z + PROJ_RADIUS > blocks[j].position.z - blocks[j].length/2.0f) 
                {
                    rockets[i].lifetime = -1.0f;
                }
        }
        
        for (int k = 0; k < RAND_COLUMNS; k++){
            if (rockets[i].pos.x - PROJ_RADIUS < randBlocks[k].position.x + randBlocks[k].width/2.0f &&
                rockets[i].pos.x + PROJ_RADIUS > randBlocks[k].position.x - randBlocks[k].width/2.0f &&
                rockets[i].pos.y - PROJ_RADIUS < randBlocks[k].position.y + randBlocks[k].height/2.0f &&
                rockets[i].pos.y + PROJ_RADIUS > randBlocks[k].position.y - randBlocks[k].height/2.0f &&
                rockets[i].pos.z - PROJ_RADIUS < randBlocks[k].position.z + randBlocks[k].length/2.0f &&
                rockets[i].pos.z + PROJ_RADIUS > randBlocks[k].position.z - randBlocks[k].length/2.0f) 
                {
                    rockets[i].lifetime = -1.0f;
                }
        }
        
        for (int h = 0; h < BARRELS; h++){
            if (rockets[i].pos.x - PROJ_RADIUS < barrels[h].position.x + barrels[h].radius &&
                rockets[i].pos.x + PROJ_RADIUS > barrels[h].position.x - barrels[h].radius &&
                rockets[i].pos.y - PROJ_RADIUS < barrels[h].position.y + barrels[h].height &&
                rockets[i].pos.y + PROJ_RADIUS > barrels[h].position.y - barrels[h].height &&
                rockets[i].pos.z - PROJ_RADIUS < barrels[h].position.z + barrels[h].radius &&
                rockets[i].pos.z + PROJ_RADIUS > barrels[h].position.z - barrels[h].radius) 
                {
                    rockets[i].lifetime = -1.0f;
                    points = barrels[h].pointScore;
                    moveBarrel(h);
                }
        }
        
    }
    
    return points;
}

void draw_rockets(void)
{
    for (int i = 0; i < ROCKET_CAP; i++) {
        if (rockets[i].lifetime > 0.0f) {
            DrawSphere(rockets[i].pos,
                     PROJ_RADIUS,
                     PROJ_COLOR);
            DrawSphereWires(rockets[i].pos, PROJ_RADIUS, 8, 8, BLACK);
        }
    }
}


// ------------------------------------------------ main --------------------------------------------------------
int main(void)
{
    
    // initializations
    const int WinWidth = WINDOW_WIDTH;
    const int WinHeight = WINDOW_HEIGHT;
    InitWindow(WinWidth, WinHeight, "CLC Game");
    GameScreen currentScreen = TITLE;
    
    char hud_buffer[1024];  
    
    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 1.0f, 5.0f };  
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // DO NOT CHANGE OR CAMERA BREAKS
    camera.fovy = 90.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
    
    int framesCounter = 0;
    int firingDelayCounter = 0;
    int points = 0;
    
    
    SetCameraMode(camera, CAMERA_FIRST_PERSON);
    
    SetTargetFPS(60);
    
    // block logic
    for (int i = 0; i < RAND_COLUMNS; i++)
    {
        randBlocks[i].height = (float)GetRandomValue(1,12);
        randBlocks[i].width = 2.0f;
        randBlocks[i].length = 2.0f;
        randBlocks[i].position = (Vector3){ (float)GetRandomValue(-15,15), randBlocks[i].height/2.0f, (float)GetRandomValue(-15,15)};
        randBlocks[i].color = (Color){ GetRandomValue(20,255), GetRandomValue(20,255), GetRandomValue(20,255), 255};
    }
    
    for (int i = 0; i < BARRELS; i++)
    {
        barrels[i].position = (Vector3){0.0f, 0.0f, 0.0f};
        barrels[i].radius = BARREL_RADIUS;
        barrels[i].height = BARREL_HEIGHT;
        barrels[i].color = MAROON;
        barrels[i].pointScore = 1;
        
        moveBarrel(i);
    }
    
    blocks[0].height = 2.0f;
    blocks[0].width = 4.0f;
    blocks[0].length = 2.0f;
    blocks[0].position = (Vector3){0.0f, 1.0f, 0.0f};
    blocks[0].color = MAROON;
    
    blocks[1].height = 3.0f;
    blocks[1].width = 0.1f;
    blocks[1].length = 32.0f;
    blocks[1].position = (Vector3){16.0f, 1.5f, 0.0f};
    blocks[1].color = RED;
    
    blocks[2].height = 3.0f;
    blocks[2].width = 0.1f;
    blocks[2].length = 32.0f;
    blocks[2].position = (Vector3){-16.0f, 1.5f, 0.0f};
    blocks[2].color = BLUE;
    
    blocks[3].height = 3.0f;
    blocks[3].width = 32.0f;
    blocks[3].length = 0.1f;
    blocks[3].position = (Vector3){0.0f, 1.5f, 16.0f};
    blocks[3].color = YELLOW;
    
    blocks[4].height = 3.0f;
    blocks[4].width = 32.0f;
    blocks[4].length = 0.1f;
    blocks[4].position = (Vector3){0.0f, 1.5f, -16.0f};
    blocks[4].color = GREEN;
    // end of block logic
    
    
    
    // game loop
    while (!WindowShouldClose())
    {
        
        //-------------------------------------------------------------------------------------------------------------------------------------------
        //------------------------------------------              Update Step              ----------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------------------
        
        switch(currentScreen){
            case TITLE:
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D))
                    {
                        currentScreen = GAMEPLAY;
                    }
                break;
            case GAMEPLAY:
            
                framesCounter++;
                firingDelayCounter--;
                
                if (firingDelayCounter <= 0) firingDelayCounter = 0;
                
                points += update_rockets();
                
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && firingDelayCounter <= 0) {
                    Vector3 dir = camera_direction(&camera);
                    spawn_rocket(
                        Vector3Add(camera.position, Vector3Scale(dir, PLAYER_GUN_LEN)),
                        Vector3Scale(dir, PROJ_VEL));
                    firingDelayCounter = PROJ_DELAY;
                }
                
                // collision
                for (int i = 0; i < STAGE_BLOCKS; i++)
                {   
                    if (camera.position.x - 0.2f < blocks[i].position.x + blocks[i].width/2.0f &&
                        camera.position.x + 0.2f > blocks[i].position.x - blocks[i].width/2.0f &&
                        camera.position.z - 0.2f < blocks[i].position.z + blocks[i].length/2.0f &&
                        camera.position.z + 0.2f > blocks[i].position.z - blocks[i].length/2.0f) 
                        {
                            float Xoff = camera.position.x - blocks[i].position.x;
                            float Zoff = camera.position.z - blocks[i].position.z;
                            
                            if (Xoff < 0) Xoff *= -1;
                            if (Zoff < 0) Zoff *= -1;
                            
                            if (Xoff - blocks[i].width/2.0f > Zoff - blocks[i].length/2.0f)
                            {
                                if (camera.position.x - blocks[i].position.x > 0)
                                {
                                    camera.position.x = blocks[i].position.x + blocks[i].width/2.0f + 0.2f;
                                } else {
                                    camera.position.x = blocks[i].position.x - blocks[i].width/2.0f - 0.2f;
                                }
                            } else {
                                if (camera.position.z - blocks[i].position.z > 0)
                                {
                                    camera.position.z = blocks[i].position.z + blocks[i].length/2.0f + 0.2f;
                                } else {
                                    camera.position.z = blocks[i].position.z - blocks[i].length/2.0f - 0.2f;
                                }
                            }
                            
                        } else {
                            
                        }
                }
                
                for (int i = 0; i < RAND_COLUMNS; i++)
                {   
                    if (camera.position.x - 0.2f < randBlocks[i].position.x + randBlocks[i].width/2.0f &&
                        camera.position.x + 0.2f > randBlocks[i].position.x - randBlocks[i].width/2.0f &&
                        camera.position.z - 0.2f < randBlocks[i].position.z + randBlocks[i].length/2.0f &&
                        camera.position.z + 0.2f > randBlocks[i].position.z - randBlocks[i].length/2.0f) 
                        {
                            float Xoff = camera.position.x - randBlocks[i].position.x;
                            float Zoff = camera.position.z - randBlocks[i].position.z;
                            
                            if (Xoff < 0) Xoff *= -1;
                            if (Zoff < 0) Zoff *= -1;
                            
                            if (Xoff - randBlocks[i].width/2.0f > Zoff - randBlocks[i].length/2.0f)
                            {
                                if (camera.position.x - randBlocks[i].position.x > 0)
                                {
                                    camera.position.x = randBlocks[i].position.x + randBlocks[i].width/2.0f + 0.2f;
                                } else {
                                    camera.position.x = randBlocks[i].position.x - randBlocks[i].width/2.0f - 0.2f;
                                }
                            } else {
                                if (camera.position.z - randBlocks[i].position.z > 0)
                                {
                                    camera.position.z = randBlocks[i].position.z + randBlocks[i].length/2.0f + 0.2f;
                                } else {
                                    camera.position.z = randBlocks[i].position.z - randBlocks[i].length/2.0f - 0.2f;
                                }
                            }
                            
                        } else {
                            
                        }
                }
            break;
        } // switch
        
        
        
        
        //-------------------------------------------------------------------------------------------------------------------------------------------
        //------------------------------------------               Draw Step               ----------------------------------------------------------
        //-------------------------------------------------------------------------------------------------------------------------------------------
        
        switch(currentScreen){
            case TITLE:
            
            BeginDrawing();
                ClearBackground(LIGHTGRAY);
                DrawText("Welcome to my game!", 40, 50, 40, BLACK);
                DrawText("Move with WASD and launch balls with the left mouse button.", 40, 100, 40, BLACK);
                DrawText("Aim for the barrels scattered around the map, different", 40, 150, 40, BLACK);
                DrawText("colours give different point amounts.", 40, 200, 40, BLACK);
                DrawText("Destroying a barrel generates a new barrel in a random location.", 40, 250, 40, BLACK);
                DrawText("         Red      - 1 point", 40, 300, 40, BLACK);
                DrawText("         Green   - 2 points", 40, 350, 40, BLACK);
                DrawText("         Purple  - 3 points", 40, 400, 40, BLACK);
                DrawText("         Yellow   - 5 points", 40, 450, 40, BLACK);
                DrawText("Press left click, Spacebar, w, a, s, or, d to start.", 40, 750, 40, BLACK);
                
            EndDrawing();
                break;
            
            case GAMEPLAY: 
            
            BeginDrawing();

                UpdateCamera(&camera);
                                    
                ClearBackground(RAYWHITE);
                            
                BeginMode3D(camera);
                                        
                    draw_rockets();
                                            
                    // Draw some cubes around
                    
                    for (int i = 0; i < RAND_COLUMNS; i++)
                    {
                       DrawCube(randBlocks[i].position, randBlocks[i].width, randBlocks[i].height, randBlocks[i].length, randBlocks[i].color);
                        DrawCubeWires(randBlocks[i].position, randBlocks[i].width, randBlocks[i].height, randBlocks[i].length, BLACK);
                    }
                    
                    for (int i = 0; i < STAGE_BLOCKS; i++)
                    {
                        DrawCube(blocks[i].position, blocks[i].width, blocks[i].height, blocks[i].length, blocks[i].color);
                        DrawCubeWires(blocks[i].position, blocks[i].width, blocks[i].height, blocks[i].length, BLACK);
                    }
                    
                    for (int i = 0; i < BARRELS; i++)
                    {
                        DrawCylinder(barrels[i].position, barrels[i].radius, barrels[i].radius, barrels[i].height, 32, barrels[i].color);
                        DrawCylinderWires(barrels[i].position, barrels[i].radius, barrels[i].radius, barrels[i].height, 32, BLACK);
                    }
                    
                    DrawCube((Vector3){0.0f,0.0f,0.0f}, 32.0f, 0.1f, 32.0f, LIGHTGRAY);
                    

                EndMode3D();
                 
                //DrawText("GAMEPLAY", 20, 20, 40, BLACK);
                
                //DrawText(TextFormat("Position: %f, %f", camera.position.x, camera.position.z), 20, 20, 40, BLACK);
                
                DrawText(TextFormat("Points: %i", points), 20, 20, 40, BLACK);
                
                int seconds = (int)framesCounter/60;
                int minutes = seconds/60;
                
                if (seconds - minutes*60 <= 9)
                {
                    DrawText(TextFormat("Time %i:0%i", minutes, seconds - minutes*60), 20, 80, 40, BLACK);
                } else {
                    DrawText(TextFormat("Time %i:%i", minutes, seconds - minutes*60), 20, 80, 40, BLACK);
                }
                
                
                DrawText("+", WINDOW_WIDTH/2 - 10, WINDOW_HEIGHT/2 - 20, 40, BLACK);
                
                /*
                snprintf(hud_buffer, sizeof(hud_buffer), "Target: "V3_FloatFormat, V3_Pos(camera.target));
                DrawText(hud_buffer, 10, 10, 20, MAROON);
                            */

            EndDrawing();
            break;
        }
        
    }
    
    CloseWindow();
    
    return 0;
    
}