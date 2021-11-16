#include "timer.h"
#include "isr.h"
#include "../drivers/screen.h"
#include "../cpu/ports.h"
#include "../libc/string.h"
#include "../libc/function.h"

uint32_t tick = 0;

static void timer_callback(registers_t* regs)
{
    tick += 1;
    /* kprint("Tick: "); */

    char tick_ascii[0x80];
    int_to_ascii(tick, tick_ascii);
    /* kprint(tick_ascii); */
    /* kprint("\n"); */

	UNUSED(regs);
}

void init_timer(uint32_t freq)
{
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint32_t divisor = 1193180 / freq;
    uint8_t low = (uint8_t)(divisor & 0xff);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xff);

    /* set the command */
    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
