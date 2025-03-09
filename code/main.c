#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/** ** GENERIC TYPES ** **/
typedef uint8_t u8;
typedef int32_t bool;
#define false 0
#define true 1

/** ** LEVEL PATHS ** **/
// Training
static const char level_path_training_2[] =
    "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/0_0_2_Training.scl";
static const char level_path_training_3[] =
    "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/0_0_3_Training.scl";

// Tbilisi
static const char level_path_tbilisi_0[] =
    "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/1_1_0Tbilisi.scl";
static const char level_path_tbilisi_1[] =
    "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/1_1_1Tbilisi.scl";
static const char level_path_tbilisi_2[] =
    "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/1_1_2Tbilisi.scl";

// Defense Ministry
static const char level_path_defense_ministry_1[] =
    "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/1_2_1DefenseMinistry.scl";
static const char level_path_defense_ministry_2[] =
    "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/1_2_2DefenseMinistry.scl";

// Caspian Oil Refinery
static const char level_path_caspian_oil_refinery_2[] =
    "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/1_3_2CaspianOilRefinery.scl";
static const char level_path_caspian_oil_refinery_3[] =
    "C:/Program Files (x86)/Steam/steamapps/common/Splinter Cell/maps/1_3_3CaspianOilRefinery.scl";

/** ** NOTHERMAL FLAG OFFSETS ** **/
#define NOTHERMAL_FLAG_OFFSET_COUNT 9
#define NOTHERMAL_FLAG_OFFSET_TRAINING_2 0x0000182B
#define NOTHERMAL_FLAG_OFFSET_TRAINING_3 0x00001B72
#define NOTHERMAL_FLAG_OFFSET_TBILISI_0 0x00002689
#define NOTHERMAL_FLAG_OFFSET_TBILISI_1 0x000034AD
#define NOTHERMAL_FLAG_OFFSET_TBILISI_2 0x00002D4F
#define NOTHERMAL_FLAG_OFFSET_DEFENSE_MINISTRY_1 0x000038AE
#define NOTHERMAL_FLAG_OFFSET_DEFENSE_MINISTRY_2 0x00003624
#define NOTHERMAL_FLAG_OFFSET_CASPIAN_OIL_REFINERY_2 0x00001FFB
#define NOTHERMAL_FLAG_OFFSET_CASPIAN_OIL_REFINERY_3 0x000037E7

/** ** GAME FLAG STRINGS ** **/
#define NOTHERMAL_FLAG_SIZE 19
#define NOTHERMAL_TOTAL_SIZE_TO_WRITE (size_t)(NOTHERMAL_FLAG_SIZE * NOTHERMAL_FLAG_OFFSET_COUNT)
static const char nothermal_flag_original_bytes[NOTHERMAL_FLAG_SIZE] = "bNoThermalAvailable";
static const char nothermal_flag_patch_bytes[NOTHERMAL_FLAG_SIZE] = {0};

/** ** MOD FLAGS ** **/
static bool g_mod_thermal;
static bool g_mod_thermal_enable;

/** ** UTILITIES ** **/
static bool streq(char* str1, char* str2) {
    return strcmp(str1, str2) == 0;
}

/** ** TRAINER FUNCTIONS ** **/
static void exit_with_help_message(void) {
    puts("Primary syntax:");
    printf("  sct <hack> [enable|disable]\n");
    puts("Hacks available:");
    puts("  thermal - Unlock thermal vision for all levels");
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
                exit_with_help_message();
            }
        } else {
            exit_with_help_message();
        }
    } else {
        exit_with_help_message();
    }
}

static size_t patch_level_flag(const char level_path[], long flag_offset, int flag_size,
                               const char flag_patch_bytes[]) {
    size_t bytes_written = {0};
    FILE* level_file = {0};
    if (fopen_s(&level_file, level_path, "r+b") == 0) {
        if (fseek(level_file, flag_offset, SEEK_SET) == 0) {
            bytes_written = fwrite(flag_patch_bytes, sizeof(flag_patch_bytes[0]), flag_size, level_file);
        } else {
            puts("ERROR: Failed to seek to flag patch offset");
        }
        fclose(level_file);
    }
    return bytes_written;
}

static void patch_thermal(void) {
    size_t bytes_written = 0;
    if (g_mod_thermal_enable) {
        bytes_written += patch_level_flag(level_path_training_2, NOTHERMAL_FLAG_OFFSET_TRAINING_2, NOTHERMAL_FLAG_SIZE,
                                          nothermal_flag_patch_bytes);
        bytes_written += patch_level_flag(level_path_training_3, NOTHERMAL_FLAG_OFFSET_TRAINING_3, NOTHERMAL_FLAG_SIZE,
                                          nothermal_flag_patch_bytes);

        bytes_written += patch_level_flag(level_path_tbilisi_0, NOTHERMAL_FLAG_OFFSET_TBILISI_0, NOTHERMAL_FLAG_SIZE,
                                          nothermal_flag_patch_bytes);
        bytes_written += patch_level_flag(level_path_tbilisi_1, NOTHERMAL_FLAG_OFFSET_TBILISI_1, NOTHERMAL_FLAG_SIZE,
                                          nothermal_flag_patch_bytes);
        bytes_written += patch_level_flag(level_path_tbilisi_2, NOTHERMAL_FLAG_OFFSET_TBILISI_2, NOTHERMAL_FLAG_SIZE,
                                          nothermal_flag_patch_bytes);

        bytes_written += patch_level_flag(level_path_defense_ministry_1, NOTHERMAL_FLAG_OFFSET_DEFENSE_MINISTRY_1,
                                          NOTHERMAL_FLAG_SIZE, nothermal_flag_patch_bytes);
        bytes_written += patch_level_flag(level_path_defense_ministry_2, NOTHERMAL_FLAG_OFFSET_DEFENSE_MINISTRY_2,
                                          NOTHERMAL_FLAG_SIZE, nothermal_flag_patch_bytes);

        bytes_written +=
            patch_level_flag(level_path_caspian_oil_refinery_2, NOTHERMAL_FLAG_OFFSET_CASPIAN_OIL_REFINERY_2,
                             NOTHERMAL_FLAG_SIZE, nothermal_flag_patch_bytes);
        bytes_written +=
            patch_level_flag(level_path_caspian_oil_refinery_3, NOTHERMAL_FLAG_OFFSET_CASPIAN_OIL_REFINERY_3,
                             NOTHERMAL_FLAG_SIZE, nothermal_flag_patch_bytes);

        if (bytes_written == NOTHERMAL_TOTAL_SIZE_TO_WRITE) {
            puts("Enabled thermal hack");
        }
    } else {
        bytes_written += patch_level_flag(level_path_training_2, NOTHERMAL_FLAG_OFFSET_TRAINING_2, NOTHERMAL_FLAG_SIZE,
                                          nothermal_flag_original_bytes);
        bytes_written += patch_level_flag(level_path_training_3, NOTHERMAL_FLAG_OFFSET_TRAINING_3, NOTHERMAL_FLAG_SIZE,
                                          nothermal_flag_original_bytes);

        bytes_written += patch_level_flag(level_path_tbilisi_0, NOTHERMAL_FLAG_OFFSET_TBILISI_0, NOTHERMAL_FLAG_SIZE,
                                          nothermal_flag_original_bytes);
        bytes_written += patch_level_flag(level_path_tbilisi_1, NOTHERMAL_FLAG_OFFSET_TBILISI_1, NOTHERMAL_FLAG_SIZE,
                                          nothermal_flag_original_bytes);
        bytes_written += patch_level_flag(level_path_tbilisi_2, NOTHERMAL_FLAG_OFFSET_TBILISI_2, NOTHERMAL_FLAG_SIZE,
                                          nothermal_flag_original_bytes);

        bytes_written += patch_level_flag(level_path_defense_ministry_1, NOTHERMAL_FLAG_OFFSET_DEFENSE_MINISTRY_1,
                                          NOTHERMAL_FLAG_SIZE, nothermal_flag_original_bytes);
        bytes_written += patch_level_flag(level_path_defense_ministry_2, NOTHERMAL_FLAG_OFFSET_DEFENSE_MINISTRY_2,
                                          NOTHERMAL_FLAG_SIZE, nothermal_flag_original_bytes);

        bytes_written +=
            patch_level_flag(level_path_caspian_oil_refinery_2, NOTHERMAL_FLAG_OFFSET_CASPIAN_OIL_REFINERY_2,
                             NOTHERMAL_FLAG_SIZE, nothermal_flag_original_bytes);
        bytes_written +=
            patch_level_flag(level_path_caspian_oil_refinery_3, NOTHERMAL_FLAG_OFFSET_CASPIAN_OIL_REFINERY_3,
                             NOTHERMAL_FLAG_SIZE, nothermal_flag_original_bytes);

        if (bytes_written == NOTHERMAL_TOTAL_SIZE_TO_WRITE) {
            puts("Disabled thermal hack");
        }
    }
    if (bytes_written != NOTHERMAL_TOTAL_SIZE_TO_WRITE) {
        puts("ERROR: Something went wrong during patch");
    }
}

int main(int argc, char** argv) {
    puts("Splinter Cell Trainer");
    parse_arguments(argc, argv);

    if (g_mod_thermal) {
        patch_thermal();
    }
}
