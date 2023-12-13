#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <stdbool.h>
#include <time.h>
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////////
//                                  Simulation Values
///////////////////////////////////////////////////////////////////////////////////

#define BATT_TIME_CAP 21600
#define BATT_FILL_RATE BATT_TIME_CAP / 150.0f

#define OXY_TIME_CAP 10800
#define OXY_PRESSURE_CAP 3000.0f
#define OXY_FILL_RATE 0.8f

#define DEPRESS_TIME 15
#define RESTING_HEART_RATE 90.0f
#define EVA_HEART_RATE 140.0f
#define HAB_OXY_PRESSURE   3.0723f
#define HAB_CO2_PRESSURE   0.0059f
#define HAB_OTHER_PRESSURE 11.5542f
#define SUIT_OXY_PRESSURE   4.0f
#define SUIT_CO2_PRESSURE   0.001f
#define SUIT_OTHER_PRESSURE 0.0f

#define SUIT_OXY_CONSUMPTION 0.1f
#define SUIT_CO2_PRODUCTION 0.1f

#define SUIT_FAN_SPIN_UP_RATE 0.9f
#define SUIT_FAN_RPM 30000.0f
#define SUIT_FAN_ERROR_RPM 5000.0f

#define SUIT_SCRUBBER_CAP 1.0f
#define SUIT_SCRUBBER_FILL_RATE 0.8f
#define SUIT_SCRUBBER_FLUSH_RATE 0.85f

#define SUIT_COOLANT_NOMINAL_TEMP 65.0f
#define SUIT_COOLANT_NOMINAL_PRESSURE 500.0f

///////////////////////////////////////////////////////////////////////////////////
//                                  Structs
///////////////////////////////////////////////////////////////////////////////////

struct uia_data_t {

    // External Device sends Switch Values to backend
    bool eva1_power;
    bool eva1_oxy;
    bool eva1_water_waste;
    bool eva1_water_supply;

    bool eva2_power;
    bool eva2_oxy;
    bool eva2_water_waste;
    bool eva2_water_supply;

    bool oxy_vent;
    bool depress;

};

struct dcu_data_t {
    
    // EVA1 Switches
    bool eva1_batt;
    bool eva1_oxy;
    bool eva1_comm;
    bool eva1_fan;
    bool eva1_pump;
    bool eva1_co2;

    // EVA2 Switches
    bool eva2_batt;
    bool eva2_oxy;
    bool eva2_comm;
    bool eva2_fan;
    bool eva2_pump;
    bool eva2_co2;

};

struct imu_data_t {

    // EVA1 IMU
    float eva1_posx;
    float eva1_posy;
    float eva1_heading;
    
    // EVA2 IMU
    float eva2_posx;
    float eva2_posy;
    float eva2_heading;

};

struct rover_data_t {

    float pos_x;
    float pos_y;
    int   prev_qr_scan;
    // other stuff

};

struct spec_data_t {

    // External Device sends Rock ID Values to backend
    int eva1_rock_id;
    int eva2_rock_id;

};

struct comm_data_t {

    bool comm_tower_online;

};

struct telemetry_data_t {

    // Battery
    float batt_time;                // The number of seconds of power the tank has

    // Oxy Tanks
    float oxy_pri_tank_fill;        // The number of seconds of oxygen the tank has
    float oxy_sec_tank_fill;
    float oxy_pri_tank_pressure;    // The pressure of the oxygen in the tank
    float oxy_sec_tank_pressure;

    // Life Support
    float heart_rate;               // A random value within a range that determines the oxy_consumption and co2_production
    float oxy_consumption;
    float co2_production;

    float suit_oxy_pressure;
    float suit_co2_pressure;
    float suit_other_pressure;      // Mostly nitrogen in the air (this value is set to zero during depress)
    int   depress_time;

    float helmet_co2_pressure;      // co2_production goes straight to the helmet, the fans push it to the rest of the suit.
    float fan_pri_rpm;
    float fan_sec_rpm;
    float scrubber_A_co2_captured;
    float scrubber_B_co2_captured;

    // Temperature Regulation
    float temperature;
    float coolant_tank;
    float coolant_liquid_pressure;
    float coolant_gaseous_pressure;

    // Communications
    int com_channel;

};

struct eva_failures_t {
    
    bool oxy_error;
    bool fan_error;
    bool pump_error;
    bool power_error;

};

struct eva_data_t {

    // General EVA Info
    bool     started;
    bool     paused;
    bool     completed;
    uint32_t total_time;

    // time at each station
    bool     started_UIA;
    bool     completed_UIA;
    uint32_t time_at_UIA;
  
    bool     started_DCU;
    bool     completed_DCU;
    uint32_t time_at_DCU;
  
    bool     started_ROVER;
    bool     completed_ROVER;
    uint32_t time_at_ROVER;
  
    bool     started_SPEC;
    bool     completed_SPEC;
    uint32_t time_at_SPEC;

    // Suit Simulated Telemetry 
    struct telemetry_data_t eva1;
    struct telemetry_data_t eva2;

    // Procedure Info
    int procedure_step;

};

#define NUMBER_OF_TEAMS 11

struct backend_data_t {

    uint32_t start_time;
    uint32_t server_up_time;

    // Data for each team
    struct eva_data_t       evas[NUMBER_OF_TEAMS];
    // struct telemetry_data_t telemetry[NUMBER_OF_TEAMS];

    // Global Data
    struct uia_data_t       uia;
    struct dcu_data_t       dcu;
    struct imu_data_t       imu;
    struct rover_data_t     rover;
    struct spec_data_t      spec;
    struct comm_data_t      comm;

    // Simulated Data
    struct eva_failures_t   failures;

};

///////////////////////////////////////////////////////////////////////////////////
//                                 Functions
///////////////////////////////////////////////////////////////////////////////////

// initialize the backend with default values and clean up at the end
struct backend_data_t* init_backend();
void cleanup_backend(struct backend_data_t*  backend);
void reset_telemetry(struct telemetry_data_t* telemetry, float seed);

// build json files when values update
bool build_json_meta_data(struct backend_data_t* backend);
bool build_json_uia      (struct uia_data_t* uia);
bool build_json_dcu      (struct dcu_data_t* dcu);
bool build_json_imu      (struct imu_data_t* imu);
bool build_json_rover    (struct rover_data_t* rover);
bool build_json_spec     (struct spec_data_t* spec);
bool build_json_comm     (struct comm_data_t* comm);
bool build_json_error    (struct eva_failures_t* error);
bool build_json_eva      (struct eva_data_t* eva, int team_index, bool completed);
bool build_json_telemetry(struct eva_data_t* eva, int team_index, bool completed);

// Update locally stored variables
bool update_uia      (char* request_content, struct uia_data_t* uia);
bool update_dcu      (char* request_content, struct dcu_data_t* dcu);
bool update_imu      (char* request_content, struct imu_data_t* imu);
bool update_rover    (char* request_content, struct rover_data_t* rover);
bool update_spec     (char* request_content, struct spec_data_t* spec);
bool update_comm     (char* request_content, struct comm_data_t* comm);
bool update_error    (char* request_content, struct eva_failures_t* error);
bool update_eva      (char* request_content, struct backend_data_t* backend);
bool update_telemetry(struct telemetry_data_t* telemetry, uint32_t eva_time, struct backend_data_t* backend, bool isEVA1);
bool update_resource (char* request_content, struct backend_data_t* backend); // Entry Point to all other update functions

// Simulate the backend
float fourier_sin(float x);
float randomized_sine_value(float x, float avg, float amp, float phase, float freq);
void simulate_telemetry(struct telemetry_data_t telemetry);
void simulate_backend  (struct backend_data_t* backend);

#endif