#pragma once

namespace controller
{
    enum JoystickAxisChannel
    {
        LX = 0,
        LY,
        RX,
        RY,
        LT,
        RT
    };

    enum JoystickButtonChannel
    {
        A = 0,
        B,
        Y,
        X,
        LB,
        RB,
        SELECT,
        START,
        HOME,
        LS,
        RS,
        DPAD_UP,
        DPAD_RIGHT,
        DPAD_DOWN,
        DPAD_LEFT
    };
}

namespace glfw
{
    enum KeyboardKeys
    {
        KEY_UNKNOWN = -1,
        KEY_SPACE = 32,
        KEY_APOSTROPE = 39,

        KEY_COMMA = 44,
        KEY_MINUS,
        KEY_PERIOD,
        KEY_SLASH,
        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,

        KEY_SEMICOLON = 59,
        KEY_EQUAL = 61,

        KEY_A = 65,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_y,
        KEY_Z,
        KEY_LEFT_BRACKET,
        KEY_BACKSLASH,
        KEY_RIGHT_BRACKET,

        KEY_GRAVE_ACCENT = 96,
        KEY_WORLD_1 = 161,
        KEY_WORLD_2,

        KEY_ESCAPE = 256,
        KEY_ENTER,
        KEY_TAB,
        KEY_BACKSPACE,
        KEY_INSERT,
        KEY_DELETE,
        KEY_RIGHT,
        KEY_LEFT,
        KEY_DOWN,
        KEY_UP,
        KEY_PAGE_UP,
        KEY_PAGE_DOWN,
        KEY_HOME,
        KEY_END,

        KEY_CAPS_LOCK = 280,
        KEY_SCROLL_LOCK,
        KEY_NUM_LOCK,
        KEY_PRINT_SCREEN,
        KEY_PAUSE,

        KEY_F1 = 290,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,
        KEY_F13,

        KEY_LEFT_SHIFT = 340,
        KEY_LEFT_CONTROL,
        KEY_LEFT_ALT,
        KEY_LEFT_SUPER,
        KEY_RIGHT_SHIFT,
        KEY_RIGHT_CONTROL,
        KEY_RIGHT_ALT,
    };
}
