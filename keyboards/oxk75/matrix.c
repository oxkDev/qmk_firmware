// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "wait.h"
#include "print.h"
#include "matrix.h"
#include "debounce.h"

// Define a default delay in microseconds
#ifndef MATRIX_IO_DELAY
#    define MATRIX_IO_DELAY 30
#endif

#ifndef MATRIX_INPUT_PRESSED_STATE
#    define MATRIX_INPUT_PRESSED_STATE 0
#endif

/* Variables */

static pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

// matrix state variables(1:on, 0:off)
static matrix_row_t raw_matrix[MATRIX_ROWS]; // raw values
static matrix_row_t matrix[MATRIX_ROWS];     // debounced values

/* Defaults for weak functions */

__attribute__((weak)) void matrix_init_kb(void) {
    matrix_init_user();
}

__attribute__((weak)) void matrix_scan_kb(void) {
    matrix_scan_user();
}

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}

/* Helper functions */

static inline void gpio_set_pin_output_low(pin_t pin) {
    gpio_set_pin_output(pin); // Set pin as output
    gpio_write_pin_low(pin);  // Drive pin low
}
static inline void gpio_set_pin_output_high(pin_t pin) {
    gpio_set_pin_output(pin); // Set pin as output
    gpio_write_pin_high(pin); // Drive pin low
}

// Turns pin to input with pull-up
static void unselect_pin(pin_t pin) {
    if (pin != NO_PIN) {
#ifdef MATRIX_UNSELECT_DRIVE_HIGH
        gpio_set_pin_output_high(pin);
#else
        gpio_set_pin_input_high(pin);
#endif
    }
}

// Turns pin to output low
static bool select_pin(pin_t pin) {
    if (pin != NO_PIN) {
        gpio_set_pin_output_low(pin);
        return true;
    }
    return false;
}

// All row pins to input pull-up
static void unselect_rows(void) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++)
        unselect_pin(row_pins[row]);
}

// All column pins to input pull-up
static void unselect_cols(void) {
    for (uint8_t col = 0; col < MATRIX_COLS; col += 2) { // Only unselect half the cols for custom matrix
        unselect_pin(col_pins[col]);
    }
}

inline matrix_row_t matrix_get_row(uint8_t row) {
    // TODO: return the requested row data
    return matrix[row];
}

void matrix_print(void) {
    // TODO: use print() to dump the current matrix state to console
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        print_hex8(row);
        print(": ");
        print_bin_reverse16(matrix[row]);
        print("\n");
    }
}

/* Initialisation */
void matrix_init(void) {
    // TODO: initialize hardware and global matrix state here

    // init pins
    unselect_rows();
    for (uint8_t col = 0; col < MATRIX_COLS; col += 2) { // Only init half the cols for custom matrix
        if (col_pins[col] != NO_PIN) gpio_set_pin_input_high(col_pins[col]);
    }

    unselect_cols();
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        if (row_pins[row] != NO_PIN) gpio_set_pin_input_high(row_pins[row]);
    }

    // initialize matrix state: all keys off
    memset(matrix, 0, sizeof(matrix));
    memset(raw_matrix, 0, sizeof(raw_matrix));

    // Unless hardware debouncing - Init the configured debounce routine
    debounce_init();

    // This *must* be called for correct keyboard behavior
    matrix_init_kb(); // Used to be matrix_init_quantum();
}

/* Scan Helper Functions */

static uint8_t readMatrixPin(pin_t pin) { // 0 if pressed, 1 if NOT pressed
    if (pin != NO_PIN) {
        return (gpio_read_pin(pin) == MATRIX_INPUT_PRESSED_STATE) ? 0 : 1;
    } else {
        return 1;
    }
}

// Scan delay functions
void inline matrix_output_select_delay(void) {
    waitInputPinDelay();
}

void inline matrix_output_unselect_delay(uint8_t line, bool key_pressed) {
    wait_us(MATRIX_IO_DELAY);
}

// Scan all columns on a given row (ROW2COL)
static void read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Initialise key pressed flag
    bool key_pressed = false;

    // Select row and wait for row selecton to stabilize
    if (!select_pin(row_pins[current_row])) { // select row
        return;                               // skip NO_PIN row, default to no change
    }
    matrix_output_select_delay();

    // Scan alternate columns from index 1 (2nd col) to index 15 (16th col)
    for (
#if (DIODE_DIRECTION == COL2ROW)
        uint8_t col_index = 1;
#elif (DIODE_DIRECTION == ROW2COL)
        uint8_t col_index = 0;
#endif
        col_index < MATRIX_COLS; col_index += 2) {
        // Check row pin state
        if (readMatrixPin(col_pins[col_index]) == 0) { // Pin LO, set col bit
            current_matrix[current_row] |= (1U << col_index);
            key_pressed = true;
        } else { // Pin HI, clear col bit
            current_matrix[current_row] &= ~(1U << col_index);
        }
    }

    // Unselect row pin
    unselect_pin(row_pins[current_row]);
    matrix_output_unselect_delay(current_row, key_pressed);

    return;
}

// Scan all rows on a given column (COL2ROW)
static void read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col) {
    // Initialise key pressed and matrix change flags
    bool key_pressed = false;

    // Select col and wait for col selepinto pinl);
    if (!select_pin(col_pins[current_col])) { // Select column
        return;                               // Skip NO_PIN columns, default to no change
    }
    matrix_output_select_delay();

    // Scan each row
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Check row pin state
        if (readMatrixPin(row_pins[row_index]) == 0) { // Pin LO, key IS pressed, set bit
            current_matrix[row_index] |= (1U << current_col);
            key_pressed = true;
        } else { // Pin HI, key NOT pressed, clear bit
            current_matrix[row_index] &= ~(1U << current_col);
        }
    }

    // Unselect col pin
    unselect_pin(col_pins[current_col]);
    matrix_output_unselect_delay(current_col, key_pressed);
}

/* Scan Sequence */
uint8_t matrix_scan(void) {
    matrix_row_t new_matrix[MATRIX_ROWS] = {0};

// TODO: add matrix scanning routine here
#if (DIODE_DIRECTION == COL2ROW)                     // First key is COL2ROW
    for (int col = 0; col < MATRIX_COLS; col += 2) { // Only scan half the cols for custom matrix
        read_rows_on_col(new_matrix, col);
    }
#elif (DIODE_DIRECTION == ROW2COL) // First key is ROW2COL
    for (int col = 1; col < MATRIX_COLS; col += 2) { // Only scan half the cols for custom matrix
        read_rows_on_col(new_matrix, col);
    }
#else
#    error The matrix.c is made specifically for Japanese Duplex polling, only COL2ROW or ROW2COL options supported
#endif

    for (int row = 0; row < MATRIX_ROWS; row++) {
        read_cols_on_row(new_matrix, row);
    }

    // Check matrix change status
    bool changed = memcmp(raw_matrix, new_matrix, sizeof(raw_matrix)) != 0;
    if (changed) memcpy(raw_matrix, new_matrix, sizeof(raw_matrix));

    // debug
    if (changed) {
        print("Changed\n");
    }
    
    // Unless hardware debouncing - use the configured debounce routine
    changed = debounce(raw_matrix, matrix, changed);

    // This *must* be called for correct keyboard behavior
    matrix_scan_kb();

    return changed;
}
