#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"

const char MAP[0x50] = "  1234567890-+  QWERTYUIOP[]  ASDFGHJKL;'` \\ZXCVBNM,./    ";

static void keyboard_callback(registers_t regs)
{
    char sc_ascii[4] = {0};

    u8 scancode = port_byte_in(0x60);
    int_to_ascii((int)scancode, sc_ascii);
    kprint("Keyboard ScanCode: ");
    kprint(sc_ascii);
    kprint(", ");
    print_letter(scancode);
    kprint("\n");
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}

void print_letter(u8 scancode)
{
    if(0 <= scancode && scancode < 0x3a)
    {
        kprint("key down ");
    }
    else if(scancode >= 0x80 && scancode < 0x3a+0x80)
    {
        kprint("key up ");
        scancode -= 0x80;
    } 
    else {
        kprint("Unkown key");
        return ;
    }

    char ch[4] = {0};
    ch[0] = MAP[scancode];

    if(ch[0] != '\x20')
    {
        kprint(ch);
    }
    else {
        switch(scancode) {
            case 0x0:
                kprint("ERROR");
                break;
            case 0x1:
                kprint("ESC");
                break;
            case 0xe:
                kprint("Backspace");
                break;
            case 0xf:
                kprint("Tab");
                break;
            case 0x1c:
                kprint("ENTER");
                break;
            case 0x1d:
                kprint("LCtrl");
                break;
            case 0x2a:
                kprint("LShift");
                break;
            case 0x36:
                kprint("RShift");
                break;
            case 0x37:
                kprint("Keypad *");
                break;
            case 0x38:
                kprint("LAlt");
                break;
            case 0x39:
                kprint("Space");
                break;
            default:
                break;
        }
    }
}