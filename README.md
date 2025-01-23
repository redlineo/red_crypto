# RED_CRYPTO ver 0.6.0

See chapter about [Memory issue](#memory-issue) and [Memory usage](#memory-usage).
I recommend using at the beginning internal FLASH or EEPROM, because you have some resources to realize the need for external memory. Or you can do not change your passwords, if there are very strong one's.

> Red_crypto will remind you constanly about using internal memory.

## Description

Password manager for (almost) any QMK-compatible keyboards. 

### Supports cryptographic algorithms

- [x] Kuznechik (Russian GOST cipher)
- [x] SHA256 (for hashing key)
- [ ] Streebog (for hashing key, Russian GOST hash algorithm)
- [ ] AES256

### Features and plans

- [x] add read all ASCII-printable symbols for key
- [x] add Kuznechik lib for non 128 bit MCU
- [x] use SHA256 for hashing key to set key length to 32 bytes
- [x] expand stored passwords length from 16 byte to 64
- [x] expand number of stored passwords (manual adding new keys)
- [ ] add menu to see stored passwords, rewrite one's, name one's, update master password (via any text editor, raw_hid is disabled) 
- [ ] write new passwords to EEPROM without flashing firmware
- [ ] add support Streebog
- [ ] add CBC mode for Kuznechik, it will increase security
- [ ] add addition procedures for Kuznechik (is it really need?)
- [ ] add support AES256
- [ ] reseach and add KDF algorithm instead of SHA256

## Installation

1. Clone this repo to your keymap folder.

2. Copy this string to **rules.mk**. Don't forget to insert your names. This is make rules for including all needed file to firmwire.

```make
include keyboards/<your keyboard>/<path to your keymap folder>/red_crypto/red_crypto.mk 
```

3. Import library in **keymap.c**.

```c
#include "red_crypto/red_crypto.h"
```

4. Define chosen algorithms and parameters in `config.h`. Now you can use only `Kuznechik`. 

```c
#define USE_KUZNECHIK_8
```

If you want use SHA256 of your key to encrypt passwords, define this in `config.h`.

```c
#define USE_SHA256_KEY
```

Change storage size (default `4`) and maximum password length (default `64`). 

```c
#define STORAGE_SIZE 4
#define STORAGE_PASS_LEN 64 // use a multiple of 16, e.g. 16, 32, 64... but don't forget about available memory! 
```

5. Encrypt your passwords with chosen algorithm (AES, Kuznechik). Now available only Kuznechik. You can use `CyberChef`. 

> Don't use web version! We are here for security, right?

If you've enabled SHA256 for key, choose `SHA2`, then `SHA256` mode and `64` rounds. Hash your key, then copy it to next step.

Choose `GOST Encrypt`, then choose type `GOST R 34.12 (Kuznechik, 2015)`, not `Magma`. Choose mode `ECB`. Now there is no other modes for `Kuznechik`.

> Don't forget IV for GOST
 ```
 94208510C2C001FB01C0C21085209401
 ```

6. Add your encrypted passwords in **keymap.c**. Now you can encrypt 4 passwords with any length (16, 32, 64, more not tested). 

> Keep this information in secret. Even if there are no plaintext. 

Chosen parameter `STORAGE_PASS_LEN` must be the same as the number of zeros in this array.

Chosen parameter `STORAGE_SIZE` must be the same as the number of passwords in this array.

```c
const uint8_t encrypted_passwords[STORAGE_SIZE][STORAGE_PASS_LEN] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    };
```

If you want more passwords in storage, you should:
- add keys `RED_PASSX` into `enum red_crypto_keys` in `red_crypto.h`
- add into `crypto_proccess_record_user` handling new keys

7. Add new layer or set new keyes everywhere you want.

- `RED_TEST` - execute test functions of every algorithm with. Available only with `#define TEST_FUNCTIONS_ENABLED` and `COMMAND_ENABLE=yes`. You can see output via `hid_listen`.
- `RED_CRY_M` - execute reading password for encrypted passwords. After entering password, tap this key again. Now, you can use `RED_PASSX` keys.
- `RED_PASSX` - where `X` is number of your password in database. After tapping, keyboard send decrypted password.
- `RED_RNG` - use 32 times function named `tap_random_base64` from QMK. It's for generating random strings. But now random seed is not unique for every keyboard's power on.  

8. Copy this string to function **process_record_user** in **keymap.c** before all your code. It uses "switch case" construction inside. 

```c
    uint8_t crypto_return = crypto_process_record_user(keycode, event, encrypted_passwords);
    if (crypto_return == 1){
        return false;
    }
```

9. Compile and flash your keyboard.


## Usage

Tap `RED_CRY_M`. Enter password. Tap `RED_CRY_M` again to confirm.
Now you can use `RED_PASSX` to send decrypted passwords.

If you have missed while entering password, just tap `RED_CRY_M` and tap `RED_CRY_M`. 


## Memory usage

To represent the resources that will be spent on the password storage. It is just example calculate.

If you have 10 different password, set 64 byte size for encrypted password, 16 byte size for ecrypted name for each password, you will store:

$ 10*(64+16) = 800b $

i.e. even if you have arduino with 1kb EEPROM, you can store enough passwords.

If you have up to 100.000 rewrite cycles, and if you will change you passwords every month, you will use up the entire resource of memory in:

$ \frac{100.000}{10*(64+16)}=\frac{125}{12}=10.4167 $

i.e. you can rewrite you passwords for 10 years. But in my calculations not consider the probability of forgetting passwords. 

## Memory issue

You have two ways to store passwords:

### FLASH or internal EEPROM

You can use FLASH or internal EEPROM memory to store passwords near with your keymap. 

#### Advantages:
- You can use red_crypto without any upgrading of keyboard. 

#### Disadvantages:
- You have memory limit. E.g. Arduino ProMicro have 1024 byte EEPROM.
- On STM32 MCUs in most cases you don't have true EEPROM, because it's emulated in FLASH.
- You have limit of re-flashing your firmware. Most likely about 100.000 times, but this number may vary depending on the temperature, manufacturer, and revision. Remember this.
- You don't have access to rewrite passwords without re-flashing firmware. May be you can, but with hard way.

### External EEPROM (or FRAM)

You can use external EEPROM (FRAM) memory with access to rewrite on-the-fly and add new passwords, etc.

#### Advantages:

- You can add new passwords, update old one's, renew master password, etc.
- Up to 1.000.000 rewrite cycles (or up to $10^{14}$ with FRAM).
- Faster access to data (more fast with FRAM).
- CAT24C512 EEPROM has built-in ECC module (Error Correction Code). Don't know how it works, but sounds good.
- You can upgrade memory with adding chips on I2C or SPI bus. Look at docs on your memory chip to know about maximum quantity.

#### Disadvantages:

- You will have to buy a chip and solder it to your MCU's pins. Your MCU must have 2 free pins for I2C support or 4 pins for SPI. Look [QMK docs](https://docs.qmk.fm/drivers/eeprom) to choose EEPROM(FRAM) chip.


## Related projects

Thanks to the authors of these repositories:

- https://github.com/mjosaarinen/kuznechik - kuznechik implementation for 8bit and 128bit
- https://github.com/B-Con/crypto-algorithms - other crypto algorithms
