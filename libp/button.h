
enum button_id
{
  BUTTON_LEFT     = 0,
  BUTTON_RIGHT    = 1,
  BUTTON_TOP      = 2,
  BUTTON_BOTTOM   = 3,
  BUTTON_CENTER   = 4,
  BUTTON_BACK     = 5
};

typedef enum button_id button_id;

enum button_state
{
  BUTTON_UP       = 0,
  BUTTON_DOWN     = 1
};

typedef enum button_state button_state;


void button_init(void);

button_state button_get_state(button_id button);
