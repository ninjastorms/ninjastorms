
enum led_id
{
  LED_LEFT   = 0,
  LED_RIGHT  = 1
};

typedef enum led_id led_id;

enum led_color
{
  LED_BLACK  = 0,
  LED_RED    = 1,
  LED_GREEN  = 2,
  LED_ORANGE = 3
};

typedef enum led_color led_color;

void led_init(void);

void led_set(led_id led, led_color color);
