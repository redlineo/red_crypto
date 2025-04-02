# RED_CRYPTO ver 0.7.3-full

## Description

Password manager for (almost) any QMK-compatible keyboards. It is full version of `RED_CRYPTO` lib.

There are two versions:
- hard-coded passwords in `keymap.c`, change branch to [`hard_coded_storage`](https://github.com/redlineo/red_crypto/tree/hard_coded_storage) or releases 
- storing passwords in EEPROM with adding and editing via `RED_MENU`, change branch to [`eeprom_storage`](https://github.com/redlineo/red_crypto/tree/eeprom_storage) or releases (YOU ARE HERE)

## Alert

> Now red_crypto supports STM32 EEPROM.
> 
> Tested on Keychron K9 Pro with STM32L432xx with virtual EEPROM

See chapter about [Memory issue](#memory-issue) and [Memory usage](#memory-usage).
I recommend using at the beginning internal FLASH or EEPROM, because you have some resources not to need external memory. Or you can do not change your passwords, if there are very strong one's.

> Red_crypto will remind you constanly about using internal memory.

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
- [x] add menu to managing stored passwords
- [x] adding new passwords via menu to EEPROM without flashing firmware
- [x] add feature to edit passwords in EEPROM
- [ ] add feature to backspacing while adding password
- [ ] add feature to name passwords in EEPROM
- [ ] add feature to update master password in EEPROM
- [ ] add feature to count memory usage
- [ ] add feature to see how many passwords you can store
- [ ] add feature to interactive choosing passwords with arrows
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

If you want (and if you can) use SHA256 of your key to encrypt passwords, define this in `config.h`.

```c
#define USE_SHA256_KEY
```

Change storage size (default `5`) and maximum password length (default `64`). 

```c
#define INIT_STORAGE_SIZE 5
#define INIT_STORAGE_PASS_LEN 64 // use a multiple of 16, e.g. 16, 32, 64... but don't forget about available memory! 
```

5. Adding passwords now in menu. Look [Usage](#usage).

6. Add new layer or set new keys everywhere you want.

- `RED_CRY_M` - execute reading password for encrypted passwords. After entering password, tap this key again. Now, you can use `RED_PASSX` keys.
- `RED_PASSX` - where `X` is number of your password in database. After tapping, keyboard send decrypted password.
- `RED_MENU` - exec menu to change passwords, show them, etc.
- `RED_RNG` - use 32 times function named `tap_random_base64` from QMK. It's for generating random strings. But now random seed is not unique for every keyboard's power on.  
- `RED_TEST` - execute test functions of every algorithm with. Available only with `#define TEST_FUNCTIONS_ENABLED` and `COMMAND_ENABLE=yes`. You can see output via `hid_listen`.

7. Add some code to **keymap.c**.

Copy this function above **process_record_user**. It is initializing EEPROM of STM32.

```c
uint16_t density=0; // for test
void keyboard_post_init_user(void) {
    density=EEPROM_Init();
}
```

Copy this string to at the beginning of function **process_record_user** in **keymap.c**. It uses "switch case" construction inside. 

```c
    uint8_t crypto_return = crypto_process_record_user(keycode, record);
    if (crypto_return == RED_KEY_READ){
        return false;
    }
```

8. Compile and flash your keyboard.


## Usage

Tap `RED_CRY_M`. Enter master key. Tap `RED_CRY_M` again to confirm.
Now you can use `RED_PASSX` to send decrypted passwords. And you can use menu via `RED_MENU`.

> If you have missed while entering password, just tap `RED_CRY_M` and tap `RED_CRY_M`. 

### Adding passwords

You must firstly enter master key to enter in **crypto mode**.

You should open any program with text box (notepad, telegram, html text box). Then tap `RED_MENU` and you will see menu.

Choose `6-Add password` with entering `6`. Enter password, tap `Enter` and `Enter`.

### Changing passwords

Open menu with `RED_MENU`, choose `4-Change password in storage` with entering `4`.
Choose **index** in hex from 0 to F. Now you can change first 16 password.
Enter new password, tap `Enter` and `Enter`.

## Memory usage

To represent the resources that will be spent on the password storage. It is just example calculate.

If you have 10 different password, set 64 byte size for encrypted password, 16 byte size for ecrypted name for each password, you will store:

$10*(64+16)=800b$

i.e. even if you have arduino with 1kb EEPROM, you can store enough passwords.

If you have up to 100.000 rewrite cycles, and if you will change you passwords every month, you will use up the entire resource of memory in:

$\frac{100.000}{10*(64+16)}=\frac{125}{12}=10.4167$

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
