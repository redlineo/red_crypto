# make config rules for red_crypto
SRC += red_crypto/red_crypto.c

SRC += red_crypto/sha256.c
SRC += red_crypto/aes.c
SRC += red_crypto/kuznechik_8bit.c
SRC += red_crypto/kc_to_ascii.c
SRC += red_crypto/utils.c
SRC += red_crypto/red_menu.c

# SRC += red_crypto/kuznechik_128bit.c # you can't include this, because you may not have ARMv8 and upper, check this
# TODO: do something like this:
# ifneq ($(findstring HAL_USE_CRY TRUE,$(HALCONF)),)
