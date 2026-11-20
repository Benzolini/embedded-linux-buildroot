################################################################################
#
# gpio-test
#
################################################################################

GPIO_TEST_VERSION = 1.0
GPIO_TEST_SITE = $(BR2_EXTERNAL_RASPBERRYPI_BPLUS_PATH)/package/gpio-test/src
GPIO_TEST_SITE_METHOD = local

define GPIO_TEST_BUILD_CMDS
        $(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS) \
                -o $(@D)/gpio_test $(@D)/gpio_test.c
endef

define GPIO_TEST_INSTALL_TARGET_CMDS
        $(INSTALL) -D -m 0755 $(@D)/gpio_test \
                $(TARGET_DIR)/usr/bin/gpio_test

        $(INSTALL) -D -m 0755 \
                $(BR2_EXTERNAL_RASPBERRYPI_BPLUS_PATH)/package/gpio-test/S99gpio-test \
                $(TARGET_DIR)/etc/init.d/S99gpio-test
endef

$(eval $(generic-package))
