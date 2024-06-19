#include <stdio.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#define bitmap ALLEGRO_BITMAP*
#define PI 3.14159265358979323846

typedef struct stack
{
    uint16_t x;
    uint16_t y;
    uint8_t width;
    uint8_t height;
    bitmap images[16];
    uint8_t spacing;
    double angle;
} stack;

void draw_stack(stack s, float t);
double deg_to_rad(double a);
double rad_to_deg(double a);

int main(void)
{
    al_init();
    ALLEGRO_DISPLAY* display = al_create_display(800, 800);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    al_set_window_title(display, "sprite stack");

    al_install_keyboard();
    al_init_image_addon();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    stack testStack = (stack)
    {
        .x = 400, .y = 400, .width = 16, .height = 16, .images = {}, .spacing = 3, .angle = 0.0
    };

    bitmap spritesheet = al_load_bitmap("./res/arrow.png");
    for (size_t i = 0; i < 16; i++)
    {
        testStack.images[i] = al_create_sub_bitmap(spritesheet, i * 16, 0, 16, 16);
    }
    
    bool space = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    float t = 0.0f;
    
    bool running = true;
    al_start_timer(timer);
    while (running)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { running = false; }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { running = false; }
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) { space = true; }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) { right = true; }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) { left = true; }
            if (event.keyboard.keycode == ALLEGRO_KEY_UP ) { up = true; }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN ) { down = true; }
        }

        if (event.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) { space = false; }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) { right = false; }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) { left = false; }
            if (event.keyboard.keycode == ALLEGRO_KEY_UP ) { up = false; }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN ) { down = false; }
        }

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            // Update
            if (space) { t = t == 10.0f ? 0.0f : t + 0.01f; }
            if (left) { testStack.angle -= PI; }
            if (right) { testStack.angle += PI; }
            if (up)
            {
                testStack.x += cos(rad_to_deg(testStack.angle)) * 3;
                testStack.y += sin(rad_to_deg(testStack.angle)) * 3;
            }

            // Render
            al_clear_to_color(al_map_rgb(192, 192, 255));
            draw_stack(testStack, t);
            al_flip_display();

            printf("ANGLE: %d\n", rad_to_deg(testStack.angle));
        }
    }
    al_stop_timer(timer);

    al_unregister_event_source(event_queue, al_get_display_event_source(display));
    al_unregister_event_source(event_queue, al_get_timer_event_source(timer));
    al_unregister_event_source(event_queue, al_get_keyboard_event_source());
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_uninstall_keyboard();
}

void draw_stack(stack s, float t)
{
    for (size_t i = 0; i < sizeof(s.images) / sizeof(s.images[0]); i++)
    {
        // al_draw_scaled_rotated_bitmap(s.images[i], s.width / 2, 
        //     s.height / 2, s.x, s.y + (((2.5f * sin(5.0f * t)) - 3.5f) * i * s.spacing), 6, 6, 
        //     s.angle, 0);

        al_draw_scaled_rotated_bitmap(s.images[i], s.width / 2, 
            s.height / 2, s.x, s.y - (i * s.spacing), 6, 6, 
            s.angle, 0);
    }
}

double deg_to_rad(double a)
{
    return a * (PI / 180);
}

double rad_to_deg(double a)
{
    return a * (180 / PI);
}
