///////////////////////////////////////////////////////////////////////////////////
//                                  Headers
///////////////////////////////////////////////////////////////////////////////////

#include "server_data.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

///////////////////////////////////////////////////////////////////////////////////
//                                 Functions
///////////////////////////////////////////////////////////////////////////////////

// -------------------------- INIT --------------------------------
struct backend_data_t* init_backend(){

    // init backend
    struct backend_data_t* backend = malloc(sizeof(struct backend_data_t));
    memset(backend, 0, sizeof(struct backend_data_t));
    backend->start_time = time(NULL);

    for(int i = 0; i < NUMBER_OF_TEAMS; i++){
        reset_telemetry(&(backend->evas[i].eva1), 0.0f);
        reset_telemetry(&(backend->evas[i].eva2), 1000.0f);
    }

    // reset Json files
    build_json_uia(&backend->uia);
    build_json_dcu(&backend->dcu);
    build_json_imu(&backend->imu);
    build_json_rover(&backend->rover);
    build_json_spec(&backend->spec);
    build_json_comm(&backend->comm);

    for(int i = 0; i < NUMBER_OF_TEAMS; i++){
        build_json_eva(&backend->evas[i], i, false);
        build_json_telemetry(&backend->evas[i], i, false);
    }

    return backend;
}

void cleanup_backend(struct backend_data_t*  backend){

    free(backend);
}

void reset_telemetry(struct telemetry_data_t* telemetry, float seed){

    telemetry->batt_time         = randomized_sine_value(seed, 0.2f, 0.1f, 060.0f, .02f) * BATT_TIME_CAP;
    telemetry->oxy_pri_tank_fill = randomized_sine_value(seed, 0.2f, 0.1f, 160.0f, .02f) * OXY_TIME_CAP;      
    telemetry->oxy_sec_tank_fill = randomized_sine_value(seed, 0.2f, 0.1f, 260.0f, .02f) * OXY_TIME_CAP; 
    telemetry->oxy_pri_tank_pressure = 0.0f;   
    telemetry->oxy_sec_tank_pressure = 0.0f;

    telemetry->heart_rate = RESTING_HEART_RATE;              
    telemetry->helmet_co2_pressure = 0.0f;     
    telemetry->fan_pri_rpm = 0.0f;
    telemetry->fan_sec_rpm = 0.0f;
    telemetry->scrubber_A_co2_captured = 0.0f;
    telemetry->scrubber_B_co2_captured = 0.0f;

    telemetry->suit_oxy_pressure = HAB_OXY_PRESSURE;
    telemetry->suit_co2_pressure = HAB_CO2_PRESSURE;
    telemetry->suit_other_pressure = HAB_OTHER_PRESSURE;
    telemetry->depress_time = 0;

    telemetry->temperature = 70.0f;
    telemetry->coolant_tank = randomized_sine_value(seed, 0.2f, 0.1f, 360.0f, .02f) * 100.0f; // %
    telemetry->coolant_liquid_pressure = 0.0f;
    telemetry->coolant_gaseous_pressure = 0.0f;

    // Communications
    int com_channel;
}

// -------------------------- Meta Data --------------------------------
bool build_json_meta_data(struct backend_data_t* backend){

}

// -------------------------- UIA --------------------------------
bool build_json_uia(struct uia_data_t* uia){

    const char format_buffer[512] = 
    "\n{"
	"\n\t\"uia\": {"
	"\n\t\t\"eva1_power\":        %s,"
	"\n\t\t\"eva1_oxy\":          %s,"
	"\n\t\t\"eva1_water_supply\": %s,"
	"\n\t\t\"eva1_water_waste\":  %s,"
	"\n\t\t\"eva2_power\":        %s,"
	"\n\t\t\"eva2_oxy\":          %s,"
	"\n\t\t\"eva2_water_supply\": %s,"
	"\n\t\t\"eva2_water_waste\":  %s,"
	"\n\t\t\"oxy_vent\":          %s,"
	"\n\t\t\"depress\":           %s"
	"\n\t}"
    "\n}";

    char out_buffer[512];
    sprintf(out_buffer, format_buffer, 
        uia->eva1_power        ? "true" : "false",
        uia->eva1_oxy          ? "true" : "false",
        uia->eva1_water_supply ? "true" : "false",
        uia->eva1_water_waste  ? "true" : "false",
        uia->eva2_power        ? "true" : "false",
        uia->eva2_oxy          ? "true" : "false",
        uia->eva2_water_supply ? "true" : "false",
        uia->eva2_water_waste  ? "true" : "false",
        uia->oxy_vent          ? "true" : "false",
        uia->depress           ? "true" : "false"
    );
    
    // Write bytes to file
    FILE* fd_uia = fopen("public/json_data/UIA.json", "w");
    size_t bytes_written = fwrite(out_buffer, 1, strlen(out_buffer), fd_uia);
    fclose(fd_uia);

    return bytes_written == strlen(out_buffer);

}

bool update_uia(char* request_content, struct uia_data_t* uia){

    bool* update_var = NULL;
        
    // Check which variable needs to be updated
    if(strncmp(request_content, "eva1_power=", strlen("eva1_power=")) == 0) {
        request_content += strlen("eva1_power=");
        update_var = &uia->eva1_power;
        printf("UIA EVA1 Power: ");
    } else if(strncmp(request_content, "eva1_oxy=", strlen("eva1_oxy=")) == 0) {
        request_content += strlen("eva1_oxy=");
        update_var = &uia->eva1_oxy;
        printf("UIA EVA1 Oxygen: ");
    } else if(strncmp(request_content, "eva1_water_supply=", strlen("eva1_water_supply=")) == 0) {
        request_content += strlen("eva1_water_supply=");
        update_var = &uia->eva1_water_supply;
        printf("UIA EVA1 Water Supply: ");
    } else if(strncmp(request_content, "eva1_water_waste=", strlen("eva1_water_waste=")) == 0) {
        request_content += strlen("eva1_water_waste=");
        update_var = &uia->eva1_water_waste;
        printf("UIA EVA1 Water Waste: ");

    } else if(strncmp(request_content, "eva2_power=", strlen("eva2_power=")) == 0) {
        request_content += strlen("eva2_power=");
        update_var = &uia->eva2_power;
        printf("UIA EVA2 Power: ");
    } else if(strncmp(request_content, "eva2_oxy=", strlen("eva2_oxy=")) == 0) {
        request_content += strlen("eva2_oxy=");
        update_var = &uia->eva2_oxy;
        printf("UIA EVA2 Oxygen: ");
    } else if(strncmp(request_content, "eva2_water_supply=", strlen("eva2_water_supply=")) == 0) {
        request_content += strlen("eva2_water_supply=");
        update_var = &uia->eva2_water_supply;
        printf("UIA EVA2 Water Supply: ");
    } else if(strncmp(request_content, "eva2_water_waste=", strlen("eva2_water_waste=")) == 0) {
        request_content += strlen("eva2_water_waste=");
        update_var = &uia->eva2_water_waste;
        printf("UIA EVA2 Water Waste: ");
    } 
    else if(strncmp(request_content, "depress=", strlen("depress=")) == 0) {
        request_content += strlen("depress=");
        update_var = &uia->depress;
        printf("UIA Depress: ");
    } else if(strncmp(request_content, "oxy_vent=", strlen("oxy_vent=")) == 0) {
        request_content += strlen("oxy_vent=");
        update_var = &uia->oxy_vent;
        printf("UIA Oxygen Vent: ");
    }

    // Update var
    if(update_var){
        if(strncmp(request_content, "true", 4) == 0) {
            *update_var = true;
            printf("ON\n");
        } else if(strncmp(request_content, "false", 5) == 0) {
            *update_var = false;
            printf("OFF\n");
        } else {
            return false;
        }

        build_json_uia(uia);
        
        return true;
    } 

    return false;

}

// -------------------------- DCU --------------------------------
bool build_json_dcu(struct dcu_data_t* dcu){

    const char format_buffer[512] = 
    "\n{"
	"\n\t\"dcu\": {"
	"\n\t\t\"eva1\": {"
	"\n\t\t\t\"batt\": %s,"
	"\n\t\t\t\"oxy\": %s,"
	"\n\t\t\t\"comm\": %s,"
	"\n\t\t\t\"fan\": %s,"
	"\n\t\t\t\"pump\": %s,"
	"\n\t\t\t\"co2\": %s"
	"\n\t\t},"
	"\n\t\t\"eva2\": {"
	"\n\t\t\t\"batt\": %s,"
	"\n\t\t\t\"oxy\": %s,"
	"\n\t\t\t\"comm\": %s,"
	"\n\t\t\t\"fan\": %s,"
	"\n\t\t\t\"pump\": %s,"
	"\n\t\t\t\"co2\": %s"
	"\n\t\t}"
	"\n\t}"
    "\n}";

    char out_buffer[512];
    sprintf(out_buffer, format_buffer, 
        dcu->eva1_batt ? "true" : "false",
        dcu->eva1_oxy  ? "true" : "false",
        dcu->eva1_comm  ? "true" : "false",
        dcu->eva1_fan  ? "true" : "false",
        dcu->eva1_pump ? "true" : "false",
        dcu->eva1_co2  ? "true" : "false",
        dcu->eva2_batt ? "true" : "false",
        dcu->eva2_oxy  ? "true" : "false",
        dcu->eva2_comm  ? "true" : "false",
        dcu->eva2_fan  ? "true" : "false",
        dcu->eva2_pump ? "true" : "false",
        dcu->eva2_co2  ? "true" : "false"
    );
    
    // Write bytes to file
    FILE* fd_uia = fopen("public/json_data/DCU.json", "w");
    size_t bytes_written = fwrite(out_buffer, 1, strlen(out_buffer), fd_uia);
    fclose(fd_uia);

    return bytes_written == strlen(out_buffer);

}

bool update_dcu(char* request_content, struct dcu_data_t* dcu){

    bool* update_var = NULL;
        
    // Check which variable needs to be updated
    if(strncmp(request_content, "eva1_batt=", strlen("eva1_batt=")) == 0) {
        request_content += strlen("eva1_batt=");
        update_var = &dcu->eva1_batt;
        printf("EVA1 DCU Battery: ");
    } else if(strncmp(request_content, "eva1_oxy=", strlen("eva1_oxy=")) == 0) {
        request_content += strlen("eva1_oxy=");
        update_var = &dcu->eva1_oxy;
        printf("EVA1 DCU Oxygen: ");
    } else if(strncmp(request_content, "eva1_comm=", strlen("eva1_comm=")) == 0) {
        request_content += strlen("eva1_comm=");
        update_var = &dcu->eva1_comm;
        printf("EVA1 DCU Comms: ");
    } else if(strncmp(request_content, "eva1_fan=", strlen("eva1_fan=")) == 0) {
        request_content += strlen("eva1_fan=");
        update_var = &dcu->eva1_fan;
        printf("EVA1 DCU Fan: ");
    } else if(strncmp(request_content, "eva1_pump=", strlen("eva1_pump=")) == 0) {
        request_content += strlen("eva1_pump=");
        update_var = &dcu->eva1_pump;
        printf("EVA1 DCU Pump: ");
    } else if(strncmp(request_content, "eva1_co2=", strlen("eva1_co2=")) == 0) {
        request_content += strlen("eva1_co2=");
        update_var = &dcu->eva1_co2;
        printf("EVA1 DCU CO2 SCRUBBER A: ");

    } else if(strncmp(request_content, "eva2_batt=", strlen("eva2_batt=")) == 0) {
        request_content += strlen("eva2_batt=");
        update_var = &dcu->eva2_batt;
        printf("EVA2 DCU Battery: ");
    } else if(strncmp(request_content, "eva2_oxy=", strlen("eva2_oxy=")) == 0) {
        request_content += strlen("eva2_oxy=");
        update_var = &dcu->eva2_oxy;
        printf("EVA2 DCU Oxygen: ");
    } else if(strncmp(request_content, "eva2_comm=", strlen("eva2_comm=")) == 0) {
        request_content += strlen("eva2_comm=");
        update_var = &dcu->eva2_comm;
        printf("EVA2 DCU Comms: ");
    } else if(strncmp(request_content, "eva2_fan=", strlen("eva2_fan=")) == 0) {
        request_content += strlen("eva2_fan=");
        update_var = &dcu->eva2_fan;
        printf("EVA2 DCU Fan: ");
    } else if(strncmp(request_content, "eva2_pump=", strlen("eva2_pump=")) == 0) {
        request_content += strlen("eva2_pump=");
        update_var = &dcu->eva2_pump;
        printf("EVA2 DCU Pump: ");
    } else if(strncmp(request_content, "eva2_co2=", strlen("eva2_co2=")) == 0) {
        request_content += strlen("eva2_co2=");
        update_var = &dcu->eva2_co2;
        printf("EVA2 DCU CO2 SCRUBBER A: ");
    }

    // Update var
    if(update_var){

        if(strncmp(request_content, "true", 4) == 0) {
            *update_var = true;
            printf("ON\n");
        } else if(strncmp(request_content, "false", 5) == 0) {
            *update_var = false;
            printf("OFF\n");
        } else {
            return false;
        }

        // update json
        build_json_dcu(dcu);
        
        return true;
    } 

    return false;

}

// -------------------------- IMU --------------------------------
bool build_json_imu(struct imu_data_t* imu){

    const char format_buffer[512] = 
    "\n{"
	"\n\t\"imu\": {"
	"\n\t\t\"eva1\": {"
	"\n\t\t\t\"posx\": %f,"
	"\n\t\t\t\"posy\": %f,"
	"\n\t\t\t\"heading\": %f"
	"\n\t\t},"
	"\n\t\t\"eva2\": {"
	"\n\t\t\t\"posx\": %f,"
	"\n\t\t\t\"posy\": %f,"
	"\n\t\t\t\"heading\": %f"
	"\n\t\t}"
	"\n\t}"
    "\n}";

    char out_buffer[512];
    sprintf(out_buffer, format_buffer, 
        imu->eva1_posx,
        imu->eva1_posy,
        imu->eva1_heading,
        imu->eva2_posx,
        imu->eva2_posy,
        imu->eva2_heading
    );
    
    // Write bytes to file
    FILE* fd_imu = fopen("public/json_data/IMU.json", "w");
    size_t bytes_written = fwrite(out_buffer, 1, strlen(out_buffer), fd_imu);
    fclose(fd_imu);

    return bytes_written == strlen(out_buffer);

}

bool update_imu(char* request_content, struct imu_data_t* imu){
    
    float* update_var = NULL;

    // Check which variable needs to be updated
    if(strncmp(request_content, "eva1_posx=", strlen("eva1_posx=")) == 0) {
        request_content += strlen("eva1_posx=");
        update_var = &imu->eva1_posx;
        printf("EVA1 IMU Pos x: ");
    } else if(strncmp(request_content, "eva1_posy=", strlen("eva1_posy=")) == 0) {
        request_content += strlen("eva1_posy=");
        update_var = &imu->eva1_posy;
        printf("EVA1 IMU Pos y: ");
    } else if(strncmp(request_content, "eva1_heading=", strlen("eva1_heading=")) == 0) {
        request_content += strlen("eva1_heading=");
        update_var = &imu->eva1_heading;
        printf("EVA1 IMU Heading: ");
    } else if(strncmp(request_content, "eva2_posx=", strlen("eva2_posx=")) == 0) {
        request_content += strlen("eva2_posx=");
        update_var = &imu->eva2_posx;
        printf("EVA2 IMU Pos x: ");
    } else if(strncmp(request_content, "eva2_posy=", strlen("eva2_posy=")) == 0) {
        request_content += strlen("eva2_posy=");
        update_var = &imu->eva2_posy;
        printf("EVA2 IMU Pos y: ");
    } else if(strncmp(request_content, "eva2_heading=", strlen("eva2_heading=")) == 0) {
        request_content += strlen("eva2_heading=");
        update_var = &imu->eva2_heading;
        printf("EVA2 IMU Heading: ");
    }

    if(update_var){
        *update_var = atof(request_content);
        printf("%f\n", *update_var);

        // update json
        build_json_imu(imu);
        return true;

    } else {
        return false;
    }


}

// -------------------------- ROVER --------------------------------
bool build_json_rover(struct rover_data_t* rover){

    const char format_buffer[512] = 
    "\n{"
	"\n\t\"rover\": {"
	"\n\t\t\"posx\": %f,"
	"\n\t\t\"posy\": %f,"
	"\n\t\t\"qr_id\": %d"
	"\n\t}"
    "\n}";

    char out_buffer[512];
    sprintf(out_buffer, format_buffer, 
        rover->pos_x,
        rover->pos_y, 
        rover->prev_qr_scan
    );
    
    // Write bytes to file
    FILE* fd_rover = fopen("public/json_data/ROVER.json", "w");
    size_t bytes_written = fwrite(out_buffer, 1, strlen(out_buffer), fd_rover);
    fclose(fd_rover);

    return bytes_written == strlen(out_buffer);

}

bool update_rover(char* request_content, struct rover_data_t* rover){

    if(strncmp(request_content, "qr=", strlen("qr=")) == 0) {
        request_content += strlen("qr=");
        rover->prev_qr_scan = atoi(request_content);
        printf("ROVER QR: %d\n", rover->prev_qr_scan);
    } else if(strncmp(request_content, "posx=", strlen("posx=")) == 0) {
        request_content += strlen("posx=");
        rover->pos_x = atof(request_content);
        printf("ROVER Pos x: %f\n", rover->pos_x);
    } else if(strncmp(request_content, "posy=", strlen("posy=")) == 0) {
        request_content += strlen("posy=");
        rover->pos_y = atof(request_content);
        printf("ROVER Pos x: %f\n", rover->pos_y);
    } else {
        return false;
    }

    // if a value was updated
    build_json_rover(rover);

    return true;

}

// -------------------------- SPEC --------------------------------
bool build_json_spec(struct spec_data_t* spec){

    // Read Rock Data Json File
    int rocks_in_db = 15;
    FILE* fd_rocks = fopen("public/json_data/rocks/RockData.json", "r");
    fseek(fd_rocks, 0, SEEK_END);
    long fsize = ftell(fd_rocks);
    fseek(fd_rocks, 0, SEEK_SET);

    char *rock_database = malloc(fsize + 1);
    fread(rock_database, fsize, 1, fd_rocks);
    fclose(fd_rocks);
    rock_database[fsize] = 0;

    // Format string for SPEC Data
    const char format_buffer[512] = 
    "\n{"
	"\n\t\"spec\": {"
	"\n\t\t\"eva1\": \n\t\t%s,"
	"\n\t\t\"eva2\": \n\t\t%s"
	"\n\t}"
    "\n}";

    char* eva1_selected_rock_data_start = rock_database;
    char* eva1_selected_rock_data_end   = rock_database;
    for(int i = 0; i <= spec->eva1_rock_id && i < rocks_in_db; i++){
        eva1_selected_rock_data_start = strstr(eva1_selected_rock_data_end + 1, "{");
        eva1_selected_rock_data_end   = strstr(eva1_selected_rock_data_start, "}");
        eva1_selected_rock_data_end   = strstr(eva1_selected_rock_data_end + 1, "}");
    }

    char* eva2_selected_rock_data_start = rock_database;
    char* eva2_selected_rock_data_end   = rock_database;
    for(int i = 0; i <= spec->eva2_rock_id && i < rocks_in_db; i++){
        eva2_selected_rock_data_start = strstr(eva2_selected_rock_data_end + 1, "{");
        eva2_selected_rock_data_end   = strstr(eva2_selected_rock_data_start, "}");
        eva2_selected_rock_data_end   = strstr(eva2_selected_rock_data_end + 1, "}");
    }

    eva1_selected_rock_data_end[1] = 0;
    eva2_selected_rock_data_end[1] = 0;

    char* out_buffer = malloc(512 + 2*fsize);
    sprintf(out_buffer, format_buffer, 
        eva1_selected_rock_data_start,
        eva2_selected_rock_data_start
    );
    
    // Write bytes to file
    FILE* fd_spec = fopen("public/json_data/SPEC.json", "w");
    size_t bytes_written = fwrite(out_buffer, 1, strlen(out_buffer), fd_spec);
    fclose(fd_spec);

    free(rock_database);
    free(out_buffer);

    return bytes_written == strlen(out_buffer);    

}

bool update_spec(char* request_content, struct spec_data_t* spec){

    // Check which variable needs to be updated
    if(strncmp(request_content, "eva1_rock_id=", strlen("eva1_rock_id=")) == 0) {
        // Get Rock From EVA 1
        request_content += strlen("eva1_rock_id=");
        int rock_id = atoi(request_content);
        spec->eva1_rock_id = rock_id;
        printf("EVA1 Rock ID: %d\n", rock_id);
    } else if(strncmp(request_content, "eva2_rock_id=", strlen("eva2_rock_id=")) == 0) {
        // Get Rock From EVA 2
        request_content += strlen("eva2_rock_id=");
        int rock_id = atoi(request_content);
        spec->eva2_rock_id = rock_id;
        printf("EVA2 Rock ID: %d\n", rock_id);
    } else {
        return false;
    }

    build_json_spec(spec);

    return true;

}

// -------------------------- COMM --------------------------------
bool build_json_comm(struct comm_data_t* comm){

    // Format string for SPEC Data
    const char format_buffer[512] = 
    "\n{"
	"\n\t\"comm\": {"
	"\n\t\t\"comm_tower\": %s"
	"\n\t}"
    "\n}";

    char* out_buffer = malloc(512);
    sprintf(out_buffer, format_buffer, 
        comm->comm_tower_online ? "true" : "false"
    );
    
    // Write bytes to file
    FILE* fd_spec = fopen("public/json_data/COMM.json", "w");
    size_t bytes_written = fwrite(out_buffer, 1, strlen(out_buffer), fd_spec);
    fclose(fd_spec);

    free(out_buffer);

    return bytes_written == strlen(out_buffer);    

}

bool update_comm(char* request_content, struct comm_data_t* comm){

    // Check which variable needs to be updated
    if(strncmp(request_content, "tower=", strlen("tower=")) == 0) {
        request_content += strlen("tower=");
        if(strncmp(request_content, "true", 4) == 0) {
            comm->comm_tower_online = true;
            printf("COMM Tower: Online\n");
        } else if(strncmp(request_content, "false", 5) == 0) {
            comm->comm_tower_online = false;
            printf("COMM Tower: Offline\n");
        } else {
            return false;
        }
    } else {
        return false;
    }

    build_json_comm(comm);

    return true;

}

// -------------------------- ERROR --------------------------------
bool build_json_error(struct eva_failures_t* error){

    const char format_buffer[512] = 
    "\n{"
	"\n\t\"error\": {"
	"\n\t\t\"fan_error\": %s,"
	"\n\t\t\"oxy_error\": %s,"
	// "\n\t\t\"batt_error\": %s,"
	"\n\t\t\"pump_error\": %s"
	"\n\t}"
    "\n}";

    char out_buffer[512];
    sprintf(out_buffer, format_buffer, 
        error->fan_error ? "true" : "false",
        error->oxy_error ? "true" : "false",
        // error->power_error ? "true" : "false",
        error->pump_error ? "true" : "false"
    );
    
    // Write bytes to file
    FILE* fd_eva = fopen("public/json_data/ERROR.json", "w");
    size_t bytes_written = fwrite(out_buffer, 1, strlen(out_buffer), fd_eva);
    fclose(fd_eva);

    return bytes_written == strlen(out_buffer);

}

bool update_error(char* request_content, struct eva_failures_t* error){

    bool* update_var = NULL;

    // Check which variable needs to be updated
    if(strncmp(request_content, "fan=", strlen("fan=")) == 0) {
        request_content += strlen("fan=");
        update_var = &error->fan_error;
        printf("Fan Error: ");
    } else if(strncmp(request_content, "oxy=", strlen("oxy=")) == 0) {
        request_content += strlen("oxy=");
        update_var = &error->oxy_error;
        printf("Oxygen Error: ");
    } else if(strncmp(request_content, "power=", strlen("power=")) == 0) {
        request_content += strlen("power=");
        update_var = &error->power_error;
        printf("Power Error: ");
    } else if(strncmp(request_content, "pump=", strlen("pump=")) == 0) {
        request_content += strlen("pump=");
        update_var = &error->pump_error;
        printf("Pump Error: ");
    } else {
        return false;
    }
    
    
    // Update var
    if(update_var){

        if(strncmp(request_content, "true", 4) == 0) {
            *update_var = true;
            printf("ON\n");
        } else if(strncmp(request_content, "false", 5) == 0) {
            *update_var = false;
            printf("OFF\n");
        } else {
            return false;
        }

        // update json
        build_json_error(error);
        
        return true;
    } 

    return false;

}

// -------------------------- EVA --------------------------------
bool build_json_eva(struct eva_data_t* eva, int team_index, bool completed){

    const char format_buffer[512] = 
    "\n{"
	"\n\t\"eva\": {"
	"\n\t\t\"started\": %s,"
	"\n\t\t\"paused\": %s,"
	"\n\t\t\"completed\": %s,"
	"\n\t\t\"total_time\": %d,"
	"\n\t\t\"uia\": {"
	"\n\t\t\t\"started\": %s,"
	"\n\t\t\t\"completed\": %s,"
	"\n\t\t\t\"time\": %d"
	"\n\t\t},"
	"\n\t\t\"dcu\": {"
	"\n\t\t\t\"started\": %s,"
	"\n\t\t\t\"completed\": %s,"
	"\n\t\t\t\"time\": %d"
	"\n\t\t},"
	"\n\t\t\"rover\": {"
	"\n\t\t\t\"started\": %s,"
	"\n\t\t\t\"completed\": %s,"
	"\n\t\t\t\"time\": %d"
	"\n\t\t},"
	"\n\t\t\"spec\": {"
	"\n\t\t\t\"started\": %s,"
	"\n\t\t\t\"completed\": %s,"
	"\n\t\t\t\"time\": %d"
	"\n\t\t}"
	"\n\t}"
    "\n}";

    char out_buffer[512];
    sprintf(out_buffer, format_buffer, 
        eva->started            ? "true" : "false",
        eva->paused             ? "true" : "false",
        eva->completed          ? "true" : "false",
        eva->total_time,
        eva->started_UIA        ? "true" : "false",
        eva->completed_UIA      ? "true" : "false",
        eva->time_at_UIA,    
        eva->started_DCU        ? "true" : "false",
        eva->completed_DCU      ? "true" : "false",
        eva->time_at_DCU,    
        eva->started_ROVER      ? "true" : "false",
        eva->completed_ROVER    ? "true" : "false",
        eva->time_at_ROVER,
        eva->started_SPEC       ? "true" : "false",
        eva->completed_SPEC     ? "true" : "false",
        eva->time_at_SPEC
    );

    char filenameTemplate[48] = "public/json_data/teams/%d/%sEVA.json";
    char out_filename[48];
    sprintf(out_filename, filenameTemplate, 
        team_index,
        completed ? "Completed_" : "");
    
    // Write bytes to file
    FILE* fd_eva = fopen(out_filename, "w");
    size_t bytes_written = fwrite(out_buffer, 1, strlen(out_buffer), fd_eva);
    fclose(fd_eva);

    return bytes_written == strlen(out_buffer);

}

bool update_eva(char* request_content, struct backend_data_t* backend){

    // Check which variable needs to be updated
    if(strncmp(request_content, "start_team=", strlen("start_team=")) == 0) {
        // Begin EVA with a new team
        request_content += strlen("start_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        memset(&backend->evas[team], 0, sizeof(struct eva_data_t));
        reset_telemetry(&(backend->evas[team].eva1), 0.0f + team * 5000.0f);
        reset_telemetry(&(backend->evas[team].eva2), 1000.0f + team * 5000.0f);
        backend->evas[team].started = true;
        printf("Team %d Started\n", team);
    } else if(strncmp(request_content, "end_team=", strlen("end_team=")) == 0) {
        // End EVA with current team
        request_content += strlen("end_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].completed       = true;
        backend->evas[team].completed_UIA   = true;
        backend->evas[team].completed_DCU   = true;
        backend->evas[team].completed_ROVER = true;
        backend->evas[team].completed_SPEC  = true;
        build_json_eva(&backend->evas[team], team, false);
        build_json_eva(&backend->evas[team], team, true);
        build_json_telemetry(&backend->evas[team], team, true);
        printf("Team %d Completed\n", team);
    } else if(strncmp(request_content, "pause_team=", strlen("pause_team=")) == 0) {
        // Pause the current EVA for team #
        request_content += strlen("pause_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].paused = true;
        build_json_eva(&backend->evas[team], team, false);
        printf("Team %d Paused\n", team);
    } else if(strncmp(request_content, "unpause_team=", strlen("unpause_team=")) == 0) {
        // Unpause the current EVA for team #
        request_content += strlen("unpause_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].paused = false;
        printf("Team %d Unpaused\n", team);
    } else if(strncmp(request_content, "start_UIA_team=", strlen("start_UIA_team=")) == 0) {
        // Start UIA Task for team #
        request_content += strlen("start_UIA_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].started_UIA = true;
        printf("Team %d UIA Started\n", team);
    } else if(strncmp(request_content, "end_UIA_team=", strlen("end_UIA_team=")) == 0) {
        // End UIA Task for team #
        request_content += strlen("end_UIA_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].completed_UIA = true;
        printf("Team %d UIA Completed\n", team);
    } else if(strncmp(request_content, "start_DCU_team=", strlen("start_DCU_team=")) == 0) {
        // Start UIA Task for team #
        request_content += strlen("start_DCU_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].started_DCU = true;
        printf("Team %d DCU Started\n", team);
    } else if(strncmp(request_content, "end_DCU_team=", strlen("end_DCU_team=")) == 0) {
        // End UIA Task for team #
        request_content += strlen("end_DCU_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].completed_DCU = true;
        printf("Team %d DCU Completed\n", team);
    } else if(strncmp(request_content, "start_ROVER_team=", strlen("start_ROVER_team=")) == 0) {
        // Start UIA Task for team #
        request_content += strlen("start_ROVER_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].started_ROVER = true;
        printf("Team %d ROVER Started\n", team);
    } else if(strncmp(request_content, "end_ROVER_team=", strlen("end_ROVER_team=")) == 0) {
        // End UIA Task for team #
        request_content += strlen("end_ROVER_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].completed_ROVER = true;
        printf("Team %d ROVER Completed\n", team);
    } else if(strncmp(request_content, "start_SPEC_team=", strlen("start_SPEC_team=")) == 0) {
        // Start UIA Task for team #
        request_content += strlen("start_SPEC_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].started_SPEC = true;
        printf("Team %d SPEC Started\n", team);
    } else if(strncmp(request_content, "end_SPEC_team=", strlen("end_SPEC_team=")) == 0) {
        // End UIA Task for team #
        request_content += strlen("end_SPEC_team=");
        int team = atoi(request_content);
        if(team > NUMBER_OF_TEAMS) { return false; }
        backend->evas[team].completed_SPEC = true;
        printf("Team %d SPEC Completed\n", team);
    } else {
        return false;
    }
        
    return true;

}

// -------------------------- Telemetry --------------------------------
bool build_json_telemetry(struct eva_data_t* eva, int team_index, bool completed){

    const char format_buffer[2048] = 
    "\n{"
	"\n\t\"telemetry\": {"

	"\n\t\t\"eva_time\": %u,"
	"\n\t\t\"eva1\": {"
	"\n\t\t\t\"batt_time_left\": %f,"
	"\n\t\t\t\"oxy_pri_storage\": %f,"
	"\n\t\t\t\"oxy_sec_storage\": %f,"
	"\n\t\t\t\"oxy_pri_pressure\": %f,"
	"\n\t\t\t\"oxy_sec_pressure\": %f,"
	"\n\t\t\t\"oxy_time_left\": %d,"
	"\n\t\t\t\"heart_rate\": %f,"
	"\n\t\t\t\"oxy_consumption\": %f,"
	"\n\t\t\t\"co2_production\": %f,"
	"\n\t\t\t\"suit_pressure_oxy\": %f,"
	"\n\t\t\t\"suit_pressure_co2\": %f,"
	"\n\t\t\t\"suit_pressure_other\": %f,"
	"\n\t\t\t\"suit_pressure_total\": %f,"
	"\n\t\t\t\"fan_pri_rpm\": %f,"
	"\n\t\t\t\"fan_sec_rpm\": %f,"
	"\n\t\t\t\"helmet_pressure_co2\": %f,"
	"\n\t\t\t\"scrubber_a_co2_storage\": %f,"
	"\n\t\t\t\"scrubber_b_co2_storage\": %f,"
	"\n\t\t\t\"temperature\": %f,"
	"\n\t\t\t\"coolant_ml\": %f,"
	"\n\t\t\t\"coolant_gas_pressure\": %f,"
	"\n\t\t\t\"coolant_liquid_pressure\": %f"
    "\n\t\t},"
	"\n\t\t\"eva2\": {"
	"\n\t\t\t\"batt_time_left\": %f,"
	"\n\t\t\t\"oxy_pri_storage\": %f,"
	"\n\t\t\t\"oxy_sec_storage\": %f,"
	"\n\t\t\t\"oxy_pri_pressure\": %f,"
	"\n\t\t\t\"oxy_sec_pressure\": %f,"
	"\n\t\t\t\"oxy_time_left\": %d,"
	"\n\t\t\t\"heart_rate\": %f,"
	"\n\t\t\t\"oxy_consumption\": %f,"
	"\n\t\t\t\"co2_production\": %f,"
	"\n\t\t\t\"suit_pressure_oxy\": %f,"
	"\n\t\t\t\"suit_pressure_cO2\": %f,"
	"\n\t\t\t\"suit_pressure_other\": %f,"
	"\n\t\t\t\"suit_pressure_total\": %f,"
	"\n\t\t\t\"fan_pri_rpm\": %f,"
	"\n\t\t\t\"fan_sec_rpm\": %f,"
	"\n\t\t\t\"helmet_pressure_co2\": %f,"
	"\n\t\t\t\"scrubber_a_co2_storage\": %f,"
	"\n\t\t\t\"scrubber_b_co2_storage\": %f,"
	"\n\t\t\t\"temperature\": %f,"
	"\n\t\t\t\"coolant_ml\": %f,"
	"\n\t\t\t\"coolant_gas_pressure\": %f,"
	"\n\t\t\t\"coolant_liquid_pressure\": %f"
    "\n\t\t}"
    "\n\t}"
    "\n}";

    char out_buffer[2048];
    sprintf(out_buffer, format_buffer, 
        eva->total_time,

        eva->eva1.batt_time,
        eva->eva1.oxy_pri_tank_fill / OXY_TIME_CAP * 100.0f,
        eva->eva1.oxy_sec_tank_fill / OXY_TIME_CAP * 100.0f,
        eva->eva1.oxy_pri_tank_pressure,
        eva->eva1.oxy_sec_tank_pressure,
        (int) (eva->eva1.oxy_sec_tank_fill + eva->eva1.oxy_pri_tank_fill), // Total Oxy time left
        eva->eva1.heart_rate,
        eva->eva1.oxy_consumption,
        eva->eva1.co2_production,
        eva->eva1.suit_oxy_pressure,
        eva->eva1.suit_co2_pressure,
        eva->eva1.suit_other_pressure,
        eva->eva1.suit_oxy_pressure + eva->eva1.suit_co2_pressure + eva->eva1.suit_other_pressure,
        eva->eva1.fan_pri_rpm,
        eva->eva1.fan_sec_rpm,
        eva->eva1.helmet_co2_pressure,
        eva->eva1.scrubber_A_co2_captured * 100.0f,
        eva->eva1.scrubber_B_co2_captured * 100.0f,
        eva->eva1.temperature,
        eva->eva1.coolant_tank,
        eva->eva1.coolant_gaseous_pressure,
        eva->eva1.coolant_liquid_pressure,

        eva->eva2.batt_time,
        eva->eva2.oxy_pri_tank_fill / OXY_TIME_CAP * 100.0f,
        eva->eva2.oxy_sec_tank_fill / OXY_TIME_CAP * 100.0f,
        eva->eva2.oxy_pri_tank_pressure,
        eva->eva2.oxy_sec_tank_pressure,
        (int) (eva->eva2.oxy_sec_tank_fill + eva->eva2.oxy_pri_tank_fill), // Total Oxy time left
        eva->eva2.heart_rate,
        eva->eva2.oxy_consumption,
        eva->eva2.co2_production,
        eva->eva2.suit_oxy_pressure,
        eva->eva2.suit_co2_pressure,
        eva->eva2.suit_other_pressure,
        eva->eva2.suit_oxy_pressure + eva->eva2.suit_co2_pressure + eva->eva2.suit_other_pressure,
        eva->eva2.fan_pri_rpm,
        eva->eva2.fan_sec_rpm,
        eva->eva2.helmet_co2_pressure,
        eva->eva2.scrubber_A_co2_captured * 100.0f,
        eva->eva2.scrubber_B_co2_captured * 100.0f,
        eva->eva2.temperature,
        eva->eva2.coolant_tank,
        eva->eva2.coolant_gaseous_pressure,
        eva->eva2.coolant_liquid_pressure
    );

    char filenameTemplate[64] = "public/json_data/teams/%d/%sTELEMETRY.json";
    char out_filename[64];
    sprintf(out_filename, filenameTemplate, 
        team_index,
        completed ? "Completed_" : "");
    
    // Write bytes to file
    FILE* fd_tel = fopen(out_filename, "w");
    size_t bytes_written = fwrite(out_buffer, 1, strlen(out_buffer), fd_tel);
    fclose(fd_tel);

    return bytes_written == strlen(out_buffer);

}

float fourier_sin(float x){
    // Constants
    float a = 0.5f;
    float f = 0.3333f;
    float p = 13.7516;
    
    float sum = 0.0f;
    float a_prod = 1.0f;
    float f_prod = 1.0f;
    float p_prod = 1.0f;
    for(int i = 0; i < 5; i++){
        a_prod *= a;
        f_prod *= f;
        p_prod *= p;
        sum += a_prod * sin((x + p_prod) / f_prod);
    }

    return sum;

}

float randomized_sine_value(float x, float avg, float amp, float phase, float freq){
    return (float) avg + (amp * fourier_sin((x + phase) / (1000.0f * freq)));
}

bool update_telemetry(struct telemetry_data_t* telemetry, uint32_t eva_time, struct backend_data_t* backend, bool isEVA1){


    struct uia_data_t* uia = &backend->uia;
    struct dcu_data_t* dcu = &backend->dcu;
    struct eva_failures_t* error = &backend->failures;

    // ---------------------------- EVA1 vs EVA2 ------------------------
    
    // uia switches
    bool uia_power_supply_connected;
    bool uia_water_supply_connected;
    bool uia_water_waste_connected;
    bool uia_o2_supply_connected;
    bool uia_o2_vent_connected;
    bool uia_depress_active;

    // dcu switches
    bool dcu_using_umbilical_power;
    bool dcu_using_pri_oxy;
    bool dcu_using_com_channel_A;
    bool dcu_using_pri_fan;
    bool dcu_is_pump_open;
    bool dcu_using_co2_scrubber_A;

    float x;
    // grab the values for the current eva
    if(isEVA1){
        uia_power_supply_connected = uia->eva1_power;
        uia_water_supply_connected = uia->eva1_water_supply;
        uia_water_waste_connected  = uia->eva1_water_waste;
        uia_o2_supply_connected    = uia->eva1_oxy;
        uia_o2_vent_connected      = uia->oxy_vent;
        uia_depress_active         = uia->depress;

        dcu_using_umbilical_power = dcu->eva1_batt;
        dcu_using_pri_oxy         = dcu->eva1_oxy;
        dcu_using_com_channel_A   = dcu->eva1_comm;
        dcu_using_pri_fan         = dcu->eva1_fan;
        dcu_is_pump_open          = dcu->eva1_pump;
        dcu_using_co2_scrubber_A  = dcu->eva1_co2;

        x = (float) eva_time;
    } else {
        uia_power_supply_connected = uia->eva2_power;
        uia_water_supply_connected = uia->eva2_water_supply;
        uia_water_waste_connected  = uia->eva2_water_waste;
        uia_o2_supply_connected    = uia->eva2_oxy;
        uia_o2_vent_connected      = uia->oxy_vent;
        uia_depress_active         = uia->depress;

        dcu_using_umbilical_power = dcu->eva2_batt;
        dcu_using_pri_oxy         = dcu->eva2_oxy;
        dcu_using_com_channel_A   = dcu->eva2_comm;
        dcu_using_pri_fan         = dcu->eva2_fan;
        dcu_is_pump_open          = dcu->eva2_pump;
        dcu_using_co2_scrubber_A  = dcu->eva2_co2;

        x = (float) eva_time + 86400.0f; // this givens a different simulation seed than eva1
    }
    

    // ---------------------------- Updates ------------------------

    telemetry->temperature = randomized_sine_value(x, 75.0f, 20.0f, 350.0f, 1.5f);

    // ---------------------------- Conditional Updates ------------------------

    // Fan Selection
    if(dcu_using_pri_fan){
        // spin up pri, spin down sec
        if(!error->fan_error) {
            telemetry->fan_pri_rpm += randomized_sine_value(x, 0.8f, 0.2f, 480.0f, 0.1f) * SUIT_FAN_SPIN_UP_RATE * ((SUIT_FAN_RPM + 1) - telemetry->fan_pri_rpm);
        } else {
            telemetry->fan_pri_rpm += randomized_sine_value(x, 0.8f, 0.2f, 480.0f, 0.1f) * SUIT_FAN_SPIN_UP_RATE * ((SUIT_FAN_ERROR_RPM + 1) - telemetry->fan_pri_rpm);
        }
        telemetry->fan_sec_rpm -= randomized_sine_value(x, 0.8f, 0.2f, 540.0f, 0.1f) * SUIT_FAN_SPIN_UP_RATE * (telemetry->fan_sec_rpm);
    } else {
        // spin up sec, spin down pri
        telemetry->fan_sec_rpm += randomized_sine_value(x, 0.8f, 0.2f, 540.0f, 0.1f) * SUIT_FAN_SPIN_UP_RATE * ((SUIT_FAN_RPM + 1) - telemetry->fan_sec_rpm);
        telemetry->fan_pri_rpm -= randomized_sine_value(x, 0.8f, 0.2f, 480.0f, 0.1f) * SUIT_FAN_SPIN_UP_RATE * (telemetry->fan_pri_rpm);
    }

    // ---------------------------- Connected to the IMU
    if(uia_power_supply_connected && dcu_using_umbilical_power){

        // Fill Battery
        telemetry->batt_time += randomized_sine_value(x, 0.8f, 0.2f, 60.0f, 0.1f) * BATT_FILL_RATE;
        if(telemetry->batt_time > BATT_TIME_CAP){ telemetry->batt_time = BATT_TIME_CAP; }

        // Fill oxygen
        if(uia_o2_supply_connected) {
            // On primary Oxygen
            if(dcu_using_pri_oxy){
                telemetry->oxy_pri_tank_fill += randomized_sine_value(x, 0.8f, 0.2f, 60.0f, 0.1f) * OXY_FILL_RATE * ((OXY_TIME_CAP + 1) - telemetry->oxy_pri_tank_fill);
                if(telemetry->oxy_pri_tank_fill > OXY_TIME_CAP){
                    telemetry->oxy_pri_tank_fill = OXY_TIME_CAP;
                }
                telemetry->oxy_pri_tank_pressure = telemetry->oxy_pri_tank_fill / OXY_TIME_CAP * OXY_PRESSURE_CAP;
            } // On Secondary Oxygen 
            else { 
                telemetry->oxy_sec_tank_fill += randomized_sine_value(x, 0.8f, 0.2f, 120.0f, 0.1f) * OXY_FILL_RATE * ((OXY_TIME_CAP + 1) - telemetry->oxy_sec_tank_fill);
                if(telemetry->oxy_sec_tank_fill > OXY_TIME_CAP){
                    telemetry->oxy_sec_tank_fill = OXY_TIME_CAP;
                }
                telemetry->oxy_sec_tank_pressure = telemetry->oxy_sec_tank_fill / OXY_TIME_CAP * OXY_PRESSURE_CAP;
            }
        }

        // Venting oxygen
        if(uia_o2_vent_connected){
            // Vents Both Oxygen at the same time
            telemetry->oxy_pri_tank_fill -= randomized_sine_value(x, 0.8f, 0.2f, 180.0f, 0.1f) * OXY_FILL_RATE * (telemetry->oxy_pri_tank_fill);
            telemetry->oxy_pri_tank_pressure = telemetry->oxy_pri_tank_fill / OXY_TIME_CAP * OXY_PRESSURE_CAP;
    
            telemetry->oxy_sec_tank_fill -= randomized_sine_value(x, 0.8f, 0.2f, 240.0f, 0.1f) * OXY_FILL_RATE * (telemetry->oxy_sec_tank_fill);
            telemetry->oxy_sec_tank_pressure = telemetry->oxy_sec_tank_fill / OXY_TIME_CAP * OXY_PRESSURE_CAP;
            
        }

        if(dcu_is_pump_open){
            // Fill Water Coolant
            if(uia_water_supply_connected){
                telemetry->coolant_tank += randomized_sine_value(x, 12.5f, 5.0f, 1080.0f, 100.0f);
                if(telemetry->coolant_tank > 100.0f){
                    telemetry->coolant_tank = 100.0f;
                }
            }

            // Flush Water Coolant
            if(uia_water_waste_connected && telemetry->coolant_tank > 0.00001f){
                telemetry->coolant_tank -= randomized_sine_value(x, 12.5f, 5.0f, 1140.0f, 100.0f);
                if(telemetry->coolant_tank < 0.00001f){
                    telemetry->coolant_tank = 0;
                }
            }
        }

        // depressurizing the suit
        if(uia_depress_active){

            if(telemetry->depress_time < DEPRESS_TIME){
                
                // telemetry->heart_rate += (EVA_HEART_RATE - RESTING_HEART_RATE) / DEPRESS_TIME; // the heart rate should go from RESTING_HEART_RATE to EVA_HEART_RATE over the course of depress
                
                telemetry->suit_oxy_pressure += (SUIT_OXY_PRESSURE - HAB_OXY_PRESSURE) / DEPRESS_TIME;
                telemetry->suit_co2_pressure += (SUIT_CO2_PRESSURE - HAB_CO2_PRESSURE) / DEPRESS_TIME;
                telemetry->suit_other_pressure += (SUIT_OTHER_PRESSURE - HAB_OTHER_PRESSURE) / DEPRESS_TIME;

                telemetry->depress_time += 1;
            
            }

        }

        telemetry->oxy_consumption = 0.0f;
        telemetry->co2_production  = 0.0f;

    } else { //----------------------------- Outside of HAB on EVA Mission

        // Oxygen is consumed each second
        // On primary Oxygen
        if(dcu_using_pri_oxy){
            telemetry->oxy_pri_tank_fill -= 1; 
            if(telemetry->oxy_pri_tank_fill < 0) { telemetry->oxy_pri_tank_fill = 0; }
            telemetry->oxy_pri_tank_pressure = telemetry->oxy_pri_tank_fill / OXY_TIME_CAP * OXY_PRESSURE_CAP;
        } // On Secondary Oxygen 
        else { 
            telemetry->oxy_sec_tank_fill -= 1;
            if(telemetry->oxy_sec_tank_fill < 0) { telemetry->oxy_sec_tank_fill = 0; }
            telemetry->oxy_sec_tank_pressure = telemetry->oxy_sec_tank_fill / OXY_TIME_CAP * OXY_PRESSURE_CAP;
        }

        // Life Support Process
        telemetry->oxy_consumption = randomized_sine_value(x, SUIT_OXY_CONSUMPTION, SUIT_OXY_CONSUMPTION / 10.0f, 359.0f, 0.023f);
        telemetry->co2_production  = randomized_sine_value(x, SUIT_CO2_PRODUCTION, SUIT_CO2_PRODUCTION / 10.f, 355.0f, 0.023f);
        telemetry->helmet_co2_pressure += telemetry->co2_production * 0.015f;

        // Distribute CO2 between helmet and suit
        float co2_pressure_diff_helmet_suit = (telemetry->helmet_co2_pressure - telemetry->suit_co2_pressure) / (telemetry->helmet_co2_pressure + telemetry->suit_co2_pressure);
        float co2_flow_rate = fmin(fmax(-0.1f, 2 * co2_pressure_diff_helmet_suit), .9f) + 0.1f;
        float co2_blow_out_of_bubble = (float) fmax(telemetry->fan_pri_rpm, telemetry->fan_sec_rpm) / SUIT_FAN_RPM * telemetry->helmet_co2_pressure * 0.015f * co2_flow_rate; 
        // printf("%f -:- %f\n", telemetry->co2_production *  0.015f, co2_blow_out_of_bubble);
        telemetry->helmet_co2_pressure -= co2_blow_out_of_bubble;
        telemetry->suit_co2_pressure += co2_blow_out_of_bubble;

        // Remove some of the CO2 from the suit
        if(dcu_using_co2_scrubber_A){
            if(telemetry->scrubber_A_co2_captured < SUIT_SCRUBBER_CAP){
                float scrubbed_amount = randomized_sine_value(x, 0.8f, 0.2f, 720.0f, 0.1f) * SUIT_SCRUBBER_FILL_RATE * (telemetry->suit_co2_pressure - (SUIT_CO2_PRESSURE));
                telemetry->suit_co2_pressure -= scrubbed_amount;
                telemetry->scrubber_A_co2_captured += scrubbed_amount;
            }
            telemetry->scrubber_B_co2_captured -= randomized_sine_value(x, 0.8f, 0.2f, 780.0f, 0.1f) * SUIT_SCRUBBER_FLUSH_RATE * (telemetry->scrubber_B_co2_captured);
        } else {
            if(telemetry->scrubber_B_co2_captured < SUIT_SCRUBBER_CAP){
                float scrubbed_amount = randomized_sine_value(x, 0.8f, 0.2f, 720.0f, 0.1f) * SUIT_SCRUBBER_FILL_RATE * (telemetry->suit_co2_pressure - (SUIT_CO2_PRESSURE));
                telemetry->suit_co2_pressure -= scrubbed_amount;
                telemetry->scrubber_B_co2_captured += scrubbed_amount;
            }
            telemetry->scrubber_A_co2_captured -= randomized_sine_value(x, 0.8f, 0.2f, 780.0f, 0.1f) * SUIT_SCRUBBER_FLUSH_RATE * (telemetry->scrubber_A_co2_captured);
        }

        // Temperature Regulations
        float total_coolant_pressure = telemetry->coolant_tank / 100.0f * telemetry->temperature / SUIT_COOLANT_NOMINAL_TEMP * SUIT_COOLANT_NOMINAL_PRESSURE;
        telemetry->coolant_gaseous_pressure = fmin(fmax((telemetry->temperature - 80.0f) / 10.0f, 0.0f), 1.0f) * total_coolant_pressure; // TODO update 85
        telemetry->coolant_liquid_pressure = total_coolant_pressure - telemetry->coolant_gaseous_pressure;

    }    
    
    // ---------------------------- Random Spice Updates ------------------------
    // These keep the values changing to make them seem more real

    telemetry->oxy_pri_tank_pressure += randomized_sine_value(x, 0, 0.01f, 1200.0f, 0.001f);
    if(telemetry->oxy_pri_tank_pressure < 0.001 || telemetry->oxy_pri_tank_fill < 0.001f) {telemetry->oxy_pri_tank_pressure = 0.0f; }
    telemetry->oxy_sec_tank_pressure += randomized_sine_value(x, 0, 0.01f, 1260.0f, 0.001f);
    if(telemetry->oxy_sec_tank_pressure < 0.001 || telemetry->oxy_sec_tank_fill < 0.001f) {telemetry->oxy_sec_tank_pressure = 0.0f; }

    // telemetry->heart_rate += randomized_sine_value(x, 0, 1.0f, 360.0f, 0.023f);

    telemetry->suit_oxy_pressure += randomized_sine_value(x, 0, 0.00008f, 1320.0f, 0.0012f);
    telemetry->suit_co2_pressure += randomized_sine_value(x, 0, 0.00008f, 1380.0f, 0.0013f);
    if(telemetry->suit_co2_pressure < 0.00001f){
        telemetry->suit_co2_pressure = 0.0f;
    }
    if(telemetry->depress_time >= DEPRESS_TIME){
        telemetry->suit_other_pressure = 0.0f;
    }

}

// -------------------------- Update --------------------------------
bool update_resource(char* request_content, struct backend_data_t* backend){

    // printf("request content: %s\n", request_content);

    if(strncmp(request_content, "uia_", 4) == 0){
        request_content += 4;
        return update_uia(request_content, &(backend->uia));
    } else if(strncmp(request_content, "dcu_", 4) == 0){
        request_content += 4;
        return update_dcu(request_content, &(backend->dcu));
    } else if(strncmp(request_content, "imu_", 4) == 0){
        request_content += 4;
        return update_imu(request_content, &backend->imu);
    } else if(strncmp(request_content, "rover_", 6) == 0){
        request_content += 6;
        return update_rover(request_content, &backend->rover);
    } else if(strncmp(request_content, "spec_", 5) == 0){
        request_content += 5;
        return update_spec(request_content, &backend->spec);
    } else if(strncmp(request_content, "eva_", 4) == 0){
        request_content += 4;
        return update_eva(request_content, backend);
    } else if(strncmp(request_content, "comm_", 5) == 0){
        request_content += 5;
        return update_comm(request_content, &backend->comm);
    } else if(strncmp(request_content, "error_", 6) == 0){
        request_content += 6;
        return update_error(request_content, &backend->failures);
    }

    return false;
}

// -------------------------- Simulation --------------------------------
void simulate_backend(struct backend_data_t* backend){

    // increment server time
    int new_time = time(NULL) - backend->start_time;
    bool time_incremented = false;
    if(new_time != backend->server_up_time){
        backend->server_up_time = new_time;
        time_incremented = true;
    }

    // Simulated EVA once per second
    if(time_incremented){

        // For each team
        for(int i = 0; i < NUMBER_OF_TEAMS; i++){
            struct eva_data_t* eva = &backend->evas[i];

            // check that the EVA is in progress
            if(eva->started && !eva->completed && !eva->paused){
                
                // Increment timers
                eva->total_time++;
                if(eva->started_UIA   && !eva->completed_UIA)     { eva->time_at_UIA++; }
                if(eva->started_DCU   && !eva->completed_DCU)     { eva->time_at_DCU++; }
                if(eva->started_ROVER && !eva->completed_ROVER)   { eva->time_at_ROVER++; }
                if(eva->started_SPEC  && !eva->completed_SPEC)    { eva->time_at_SPEC++; }

                // Update EVA Json
                build_json_eva(&backend->evas[i], i, false);

                // Simulate Telemetry
                update_telemetry(&backend->evas[i].eva1, eva->total_time, backend, true);
                update_telemetry(&backend->evas[i].eva2, eva->total_time, backend, false);

                // Update Telemetry Json
                build_json_telemetry(&backend->evas[i], i, false);

            }
            
        }

    }

}





