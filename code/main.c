#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/** ** GENERIC TYPES ** **/
typedef uint8_t u8;
typedef int32_t bool;
#define false 0
#define true 1

/** ** PATHS ** **/
static const char s_training_level_part_1[] =
    "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/0_0_2_Training.scl";

/** ** GAME FLAG OFFSETS ** **/
#define NOTHERMAL_FLAG_OFFSET 0x0000182B

/** ** GAME FLAG DATA ** **/
#define NOTHERMAL_FLAG_SIZE 19
static const char nothermal_flag_original_bytes[NOTHERMAL_FLAG_SIZE] = "bNoThermalAvailable";
static const char nothermal_flag_patch_bytes[NOTHERMAL_FLAG_SIZE] = {0};

/** ** MOD FLAGS ** **/
static bool g_mod_thermal;
static bool g_mod_thermal_enable;

/** ** FUNCTIONS ** **/
static bool streq(char* str1, char* str2) {
    return strcmp(str1, str2) == 0;
}

static void exit_with_help_message(char** argv) {
    puts("Primary syntax:");
    printf("  %s <hack> [enable|disable]\n", argv[0]);
    puts("Hacks available:");
    puts("  thermal - Allow thermal vision on all levels");
    exit(1); // NOLINT(concurrency-mt-unsafe)
}

static void parse_arguments(int argc, char** argv) {
    if (argc == 3) {
        if (streq(argv[1], "thermal")) {
            g_mod_thermal = true;
            if (streq(argv[2], "enable")) {
                g_mod_thermal_enable = true;
            } else if (streq(argv[2], "disable")) {
                g_mod_thermal_enable = false;
            } else {
                exit_with_help_message(argv);
            }
        } else {
            exit_with_help_message(argv);
        }
    } else {
        exit_with_help_message(argv);
    }
}

int main(int argc, char** argv) {
    puts("Splinter Cell Trainer");
    parse_arguments(argc, argv);

    // TODO: This only handles part 1 of the training level, and only the training level (level 2 I think is the only
    // other level which requires modification)
    if (g_mod_thermal) {
        FILE* f_training_level_part_1 = {0};
        // FIXME: CLOSE FILE
        if (fopen_s(&f_training_level_part_1, s_training_level_part_1, "r+b") == 0) {
            // Seek to patch location
            if (fseek(f_training_level_part_1, NOTHERMAL_FLAG_OFFSET, SEEK_SET) == 0) {
                if (g_mod_thermal_enable) {
                    size_t bytes_written = fwrite(&nothermal_flag_patch_bytes, sizeof(nothermal_flag_patch_bytes[0]),
                                                  NOTHERMAL_FLAG_SIZE, f_training_level_part_1);
                    if (bytes_written == NOTHERMAL_FLAG_SIZE) {
                        puts("Enabled thermal hack");
                    }
                } else {
                    size_t bytes_written =
                        fwrite(&nothermal_flag_original_bytes, sizeof(nothermal_flag_original_bytes[0]),
                               NOTHERMAL_FLAG_SIZE, f_training_level_part_1);
                    if (bytes_written == NOTHERMAL_FLAG_SIZE) {
                        puts("Disabled thermal hack");
                    }
                }
            }
            fclose(f_training_level_part_1);
        }
    }
}
