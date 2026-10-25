#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STRIP_NODE DT_ALIAS(led_strip)
#define STRIP_LENGTH DT_PROP(STRIP_NODE, chain_length)

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

int main(void)
{
    struct led_rgb pixel[STRIP_LENGTH] = {0};

    if (!device_is_ready(strip)) {
        printk("ERRO: LED RGB nao esta pronto.\n");
        return 0;
    }

    printk("Teste de LED RGB - ESP32-S3\n");

    while (1) {
        pixel[0] = (struct led_rgb){ .r = 255, .g = 0, .b = 0 };
        if (led_strip_update_rgb(strip, pixel, STRIP_LENGTH) < 0) {
            printk("Erro ao acender vermelho.\n");
            return 0;
        }
        printk("Vermelho\n");
        k_msleep(1000);

        pixel[0] = (struct led_rgb){ .r = 0, .g = 255, .b = 0 };
        if (led_strip_update_rgb(strip, pixel, STRIP_LENGTH) < 0) {
            printk("Erro ao acender verde.\n");
            return 0;
        }
        printk("Verde\n");
        k_msleep(1000);

        pixel[0] = (struct led_rgb){ .r = 0, .g = 0, .b = 255 };
        if (led_strip_update_rgb(strip, pixel, STRIP_LENGTH) < 0) {
            printk("Erro ao acender azul.\n");
            return 0;
        }
        printk("Azul\n");
        k_msleep(1000);

        pixel[0] = (struct led_rgb){ .r = 0, .g = 0, .b = 0 };
        if (led_strip_update_rgb(strip, pixel, STRIP_LENGTH) < 0) {
            printk("Erro ao apagar o LED.\n");
            return 0;
        }
        printk("Apagado\n");
        k_msleep(1000);
    }

    return 0;
}
