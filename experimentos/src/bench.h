#ifndef BENCH_H_
#define BENCH_H_

#include <stdbool.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>

/* LED RGB integrado e GPIOs do experimento */
extern const struct device *rgb_strip;
extern const struct gpio_dt_spec button;
extern const struct gpio_dt_spec stim;

/* Contadores e estado da medicao */
extern atomic_t ev_generated;
extern atomic_t ev_detected;
extern atomic_t ev_processed;

extern volatile bool measuring;
extern volatile uint32_t t_stim;
extern volatile uint32_t hb_max_dt;

/* Estatisticas de latencia */
struct lat_stat {
    uint32_t n;
    uint32_t min;
    uint32_t max;
    uint64_t sum;
};

extern struct lat_stat lat_detect;
extern struct lat_stat lat_handoff;
extern struct lat_stat lat_process;
extern struct lat_stat lat_proc_dur;

/* Funcoes do benchmark */
void lat_reset(struct lat_stat *s);
void lat_add(struct lat_stat *s, uint32_t cycles);
void lat_print(const char *name, const struct lat_stat *s);
void bench_reset_all(void);

void app_process_event(void);
int app_led_toggle(void);

void heartbeat_start(void);
int mode_start(void);
const char *mode_name(void);

#endif /* BENCH_H_ */
