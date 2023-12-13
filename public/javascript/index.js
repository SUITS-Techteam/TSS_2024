// Global variable for currently selected team and previously selected team
let selectedTeam = 0;
let oldTeam = 0;

// global variables for UI elements
let startButton = document.getElementById('tssStart');
let stopButton = document.getElementById('tssStop');

let uiaButton = document.getElementById('assignUIA');
let uia = document.getElementById('uiaTimerContainer');
let uiaStatus = document.getElementById('uiaStatus');
let uiaBullet = document.getElementById('uiaBulletPoint');
let uiaName = document.getElementById('uiaName');
let uiaFont = document.getElementById('uiaNameFont');

let dcuButton = document.getElementById('assignDCU');
let dcu = document.getElementById('dcuTimerContainer');
let dcuStatus = document.getElementById('dcuStatus');
let dcuBullet = document.getElementById('dcuBulletPoint');
let dcuName = document.getElementById('dcuName');
let dcuFont = document.getElementById('dcuNameFont');

let specButton = document.getElementById('assignSPEC');
let spec = document.getElementById('specTimerContainer');
let specStatus = document.getElementById('specStatus');
let specBullet = document.getElementById('specBulletPoint');
let specName = document.getElementById('specName');
let specFont = document.getElementById('specNameFont');

let rovButton = document.getElementById('assignROV');
let rov = document.getElementById('rovTimerContainer');
let rovStatus = document.getElementById('rovStatus');
let rovBullet = document.getElementById('rovBulletPoint');
let rovName = document.getElementById('rovName');
let rovFont = document.getElementById('rovNameFont');

let video = document.getElementById('videoFeed');

let xCoordinateEV1 = document.getElementById('xCoordinateEV1');
let yCoordinateEV1 = document.getElementById('yCoordinateEV1');
let headingEV1 = document.getElementById('headingEV1');
let xCoordinateEV2 = document.getElementById('xCoordinateEV2');
let yCoordinateEV2 = document.getElementById('yCoordinateEV2');
let headingEV2 = document.getElementById('headingEV2');
let xCoordinateRover = document.getElementById('xCoordinateRover');
let yCoordinateRover = document.getElementById('yCoordinateRover');

// Updates team specific data when another team is selected
function swapTeams(newTeam){

    // Update global team
    selectedTeam = newTeam - 1;

    // Reload the team specific Json files
    loadEVA(selectedTeam);
    loadTelemetry(selectedTeam);

    // Update button functionality
    document.getElementById('tssStart').value = selectedTeam;
    document.getElementById('tssStop').value = selectedTeam;

    document.getElementById('assignUIA').value = selectedTeam;
    document.getElementById('assignDCU').value = selectedTeam;
    document.getElementById('assignSPEC').value = selectedTeam;
    document.getElementById('assignROV').value = selectedTeam;

    // Assign page title based on room selected
    var room = "room" + (selectedTeam + 1) + "Name";
    document.getElementById('roomDataTitle').textContent = document.getElementById(room).innerText + " - Room " + (selectedTeam + 1);

    // Sets the cookie anytime a new team is selected
    setCookie("roomNum", selectedTeam, 1);
}

// Loads the rover video feed and rover coordinates
function loadRover() {
    video.src = "http://192.168.51.194:8080/stream?topic=/camera/image_raw&type=ros_compressed";

    $.getJSON("json_data/ROVER.json", function(data) {
        xCoordinateRover.innerText = (data.rover.posx).toFixed(2);
        yCoordinateRover.innerText = (data.rover.posy).toFixed(2);
    })
}

// Loads the coordinates of EVA 1 and EVA 2
function loadGPS() {
    $.getJSON("json_data/IMU.json", function(data) {
        xCoordinateEV1.innerText = (data.imu.eva1.posx).toFixed(2);
        yCoordinateEV1.innerText = (data.imu.eva1.posy).toFixed(2);
        headingEV1.innerText     = (data.imu.eva1.heading).toFixed(2);
        xCoordinateEV2.innerText = (data.imu.eva2.posx).toFixed(2);
        yCoordinateEV2.innerText = (data.imu.eva2.posy).toFixed(2);
        headingEV2.innerText     = (data.imu.eva2.heading).toFixed(2);
    })
}

// Sets the cookie to remember the last team selected
function setCookie(cname, cvalue, exdays) {

    const d = new Date();
    d.setTime(d.getTime() + (exdays*24*60*60*1000));
    let expires = "expires=" + d.toUTCString();

    // Sets cookie value for the room num and gives it an expiration date
    document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
}

// Gets the cookie for the last team selected
function getCookie(cname) {
    let name = cname + "=";
    let decodedCookie = decodeURIComponent(document.cookie);
    let ca = decodedCookie.split(';');

    // Looks for room num cookie
    for(let i = 0; i < ca.length; i++) {
        let c = ca[i];
        while ( c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return Number(c.substring(name.length, c.length));
        }
    }
    return 0;
}

// Loads the UIA data and sets the light
function loadUIA(){

    $.getJSON("json_data/UIA.json", function( data ){
        if (data.uia.eva1_power == true){
            document.getElementById("eva1PowerSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("eva1PowerSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }
        if (data.uia.eva2_power == true){
            document.getElementById("eva2PowerSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("eva2PowerSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.uia.eva1_water_supply == true){
            document.getElementById("eva1WaterSupplySensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("eva1WaterSupplySensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }
        if (data.uia.eva2_water_supply == true){
            document.getElementById("eva2WaterSupplySensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("eva2WaterSupplySensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.uia.eva1_water_waste == true){
            document.getElementById("eva1WaterWasteSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("eva1WaterWasteSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }
        if (data.uia.eva2_water_waste == true){
            document.getElementById("eva2WaterWasteSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("eva2WaterWasteSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.uia.eva1_oxy == true){
            document.getElementById("eva1OxygenSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("eva1OxygenSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }
        if (data.uia.eva2_oxy == true){
            document.getElementById("eva2OxygenSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("eva2OxygenSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.uia.oxy_vent == true){
            document.getElementById("oxygenVentSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("oxygenVentSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }
        if (data.uia.depress == true){
            document.getElementById("depressPumpSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("depressPumpSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }
    });
}

// Loads the DCU data and sets the light
function loadDCU(){

    $.getJSON("json_data/DCU.json", function( data ){

        // console.log(data);
        if (data.dcu.eva1.batt == true){
            document.getElementById("dcuEva1BatterySensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva1BatterySensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.dcu.eva1.oxy == true){
            document.getElementById("dcuEva1OxygenSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva1OxygenSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.dcu.eva1.comm == true){
            document.getElementById("dcuEva1CommSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva1CommSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.dcu.eva1.fan == true){
            document.getElementById("dcuEva1FanSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva1FanSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.dcu.eva1.pump == true){
            document.getElementById("dcuEva1PumpSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva1PumpSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.dcu.eva1.co2 == true){
            document.getElementById("dcuEva1Co2Sensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva1Co2Sensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        // console.log(data);
        if (data.dcu.eva2.batt == true){
            document.getElementById("dcuEva2BatterySensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva2BatterySensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.dcu.eva2.oxy == true){
            document.getElementById("dcuEva2OxygenSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva2OxygenSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.dcu.eva2.comm == true){
            document.getElementById("dcuEva2CommSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva2CommSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.dcu.eva2.fan == true){
            document.getElementById("dcuEva2FanSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva2FanSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.dcu.eva2.pump == true){
            document.getElementById("dcuEva2PumpSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva2PumpSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        if (data.dcu.eva2.co2 == true){
            document.getElementById("dcuEva2Co2Sensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
        }
        else {
            document.getElementById("dcuEva2Co2Sensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }
    });
}

// Loads the EVA timers depending on state(Started, Stopped, Paused, etc.) and sets light depending on state
function loadEVA(team){

    $.getJSON("json_data/teams/" + team + "/EVA.json", function( data ){

        // Formats the total time for the EVA
        var h = Math.floor(data.eva.total_time / 3600);
        var m = Math.floor(data.eva.total_time % 3600 / 60);
        var s = Math.floor(data.eva.total_time % 3600 % 60);
        document.getElementById("evaTimer").innerText = "EVA Time: " + ("0"+h).slice(-2) + ":" + ("0"+m).slice(-2) + ":" + ("0"+s).slice(-2);

        // Formats the time for the UIA procedure
        m = Math.floor(data.eva.uia.time % 3600 / 60);
        s = Math.floor(data.eva.uia.time % 3600 % 60);
        document.getElementById("uiaTimer").innerText = ("0"+m).slice(-2) + ":" + ("0"+s).slice(-2);

        // Formats the time for the Spec procedure
        m = Math.floor(data.eva.spec.time % 3600 / 60);
        s = Math.floor(data.eva.spec.time % 3600 % 60);
        document.getElementById("specTimer").innerText = ("0"+m).slice(-2) + ":" + ("0"+s).slice(-2);

        // Formats the time for the DCU procedure
        m = Math.floor(data.eva.dcu.time % 3600 / 60);
        s = Math.floor(data.eva.dcu.time % 3600 % 60);
        document.getElementById("dcuTimer").innerText = ("0"+m).slice(-2) + ":" + ("0"+s).slice(-2);

        // Formats the time for the Rover procedure
        m = Math.floor(data.eva.rover.time % 3600 / 60);
        s = Math.floor(data.eva.rover.time % 3600 % 60);
        document.getElementById("rovTimer").innerText = ("0"+m).slice(-2) + ":" + ("0"+s).slice(-2);

        // Button UI States Visuals
        var evaStarted  = data.eva.started;
        var evaPaused   = data.eva.paused;
        var evaComplete = data.eva.completed;
        if (evaComplete || !evaStarted) {
            stopTSS();
        } 
        else if(evaStarted && evaPaused) {
            pauseTSS();
        } 
        else if(evaStarted && !evaPaused) {
            resumeTSS();
        } 
        else {
            resumeTSS();
        }

        // Team light state
        var room = "room" + (team + 1) + "Light";
        var roomLight = document.getElementById(room);
        if(evaStarted && !evaComplete){
            roomLight.style.backgroundColor = 'rgba(0, 240, 10, 1)';
        } 
        else if(evaComplete){
            roomLight.style.backgroundColor = 'rgba(0, 0, 255, 1)';
        } 
        else {
            roomLight.style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }

        // Stations Status
        updateStationStatus(evaStarted, data.eva.uia.started,   data.eva.uia.completed,   uia, uiaStatus, uiaButton, uiaBullet, uiaFont, uiaName);
        updateStationStatus(evaStarted, data.eva.dcu.started,   data.eva.dcu.completed,   dcu, dcuStatus, dcuButton, dcuBullet, dcuFont, dcuName);
        updateStationStatus(evaStarted, data.eva.rover.started, data.eva.rover.completed, rov, rovStatus, rovButton, rovBullet, rovFont, rovName);
        updateStationStatus(evaStarted, data.eva.spec.started,  data.eva.spec.completed,  spec, specStatus, specButton, specBullet, specFont, specName);
    });
}

// Loads the Telemetry values of each EVA
function loadTelemetry(team){

    $.getJSON("json_data/teams/" + team + "/TELEMETRY.json", function( data ){

        // EVA 1
        var h = Math.floor(Number(data.telemetry.eva_time) / 3600);
        var m = Math.floor(Number(data.telemetry.eva_time) % 3600 / 60);
        var s = Math.floor(Number(data.telemetry.eva_time) % 3600 % 60);
        document.getElementById("evaTimeTelemetryState1").innerText = ("0"+h).slice(-2) + ":" + ("0"+m).slice(-2) + ":" + ("0"+s).slice(-2);
        document.getElementById("evaTimeTelemetryState2").innerText = ("0"+h).slice(-2) + ":" + ("0"+m).slice(-2) + ":" + ("0"+s).slice(-2);
        h = Math.floor(Number(data.telemetry.eva1.oxy_time_left) / 3600);
        m = Math.floor(Number(data.telemetry.eva1.oxy_time_left) % 3600 / 60);
        s = Math.floor(Number(data.telemetry.eva1.oxy_time_left) % 3600 % 60);
        document.getElementById("o2Time1").innerText = ("0"+h).slice(-2) + ":" + ("0"+m).slice(-2) + ":" + ("0"+s).slice(-2);

        document.getElementById("primaryO2Storage1").innerText    = Number(data.telemetry.eva1.oxy_pri_storage).toFixed(0) + " %";
        document.getElementById("secondaryO2Storage1").innerText  = Number(data.telemetry.eva1.oxy_sec_storage).toFixed(0) + " %";
        document.getElementById("primaryO2Pressure1").innerText   = Number(data.telemetry.eva1.oxy_pri_pressure).toFixed(2) + " psi";
        document.getElementById("secondaryO2Pressure1").innerText = Number(data.telemetry.eva1.oxy_sec_pressure).toFixed(2) + " psi";

        document.getElementById("suitO2Pressure1").innerText      = Number(data.telemetry.eva1.suit_pressure_oxy).toFixed(2) + " psi";
        document.getElementById("suitCO2Pressure1").innerText     = Number(data.telemetry.eva1.suit_pressure_co2).toFixed(2) + " psi";
        document.getElementById("suitOtherPressure1").innerText   = Number(data.telemetry.eva1.suit_pressure_other).toFixed(2) + " psi";
        document.getElementById("suitTotalPressure1").innerText   = Number(data.telemetry.eva1.suit_pressure_total).toFixed(2) + " psi";

        document.getElementById("scrubberAPressure1").innerText   = Number(data.telemetry.eva1.scrubber_a_co2_storage).toFixed(2) + " psi";
        document.getElementById("scrubberBPressure1").innerText   = Number(data.telemetry.eva1.scrubber_b_co2_storage).toFixed(2) + " psi";
        document.getElementById("h2OGasPressure1").innerText      = Number(data.telemetry.eva1.coolant_gas_pressure).toFixed(2) + " psi";
        document.getElementById("h2OLiquidPressure1").innerText   = Number(data.telemetry.eva1.coolant_liquid_pressure).toFixed(2) + " psi";

        document.getElementById("o2Consumption1").innerText       = Number(data.telemetry.eva1.oxy_consumption).toFixed(2) + " psi/min";
        document.getElementById("co2Production1").innerText       = Number(data.telemetry.eva1.co2_production).toFixed(2) + " psi/min";
        document.getElementById("primaryFan1").innerText          = Number(data.telemetry.eva1.fan_pri_rpm).toFixed(0) + " rpm";
        document.getElementById("secondaryFan1").innerText        = Number(data.telemetry.eva1.fan_sec_rpm).toFixed(0) + " rpm";

        document.getElementById("helmetCO2Pressure1").innerText   = Number(data.telemetry.eva1.helmet_pressure_co2).toFixed(2) + " psi";
        document.getElementById("heartRate1").innerText           = Number(data.telemetry.eva1.heart_rate).toFixed(2) + " bpm";
        document.getElementById("temperature1").innerText         = Number(data.telemetry.eva1.temperature).toFixed(2) + " deg F";
        document.getElementById("coolant1").innerText             = Number(data.telemetry.eva1.coolant_ml).toFixed(2) + " ml";


        // EVA 2
        h = Math.floor(Number(data.telemetry.eva2.oxy_time_left) / 3600);
        m = Math.floor(Number(data.telemetry.eva2.oxy_time_left) % 3600 / 60);
        s = Math.floor(Number(data.telemetry.eva2.oxy_time_left) % 3600 % 60);
        document.getElementById("o2Time2").innerText = ("0"+h).slice(-2) + ":" + ("0"+m).slice(-2) + ":" + ("0"+s).slice(-2);

        document.getElementById("primaryO2Storage2").innerText    = Number(data.telemetry.eva2.oxy_pri_storage).toFixed(0) + " %";
        document.getElementById("secondaryO2Storage2").innerText  = Number(data.telemetry.eva2.oxy_sec_storage).toFixed(0) + " %";
        document.getElementById("primaryO2Pressure2").innerText   = Number(data.telemetry.eva2.oxy_pri_pressure).toFixed(2) + " psi";
        document.getElementById("secondaryO2Pressure2").innerText = Number(data.telemetry.eva2.oxy_sec_pressure).toFixed(2) + " psi";

        document.getElementById("suitO2Pressure2").innerText      = Number(data.telemetry.eva2.suit_pressure_oxy).toFixed(2) + " psi";
        document.getElementById("suitCO2Pressure2").innerText     = Number(data.telemetry.eva2.suit_pressure_co2).toFixed(2) + " psi";
        document.getElementById("suitOtherPressure2").innerText   = Number(data.telemetry.eva2.suit_pressure_other).toFixed(2) + " psi";
        document.getElementById("suitTotalPressure2").innerText   = Number(data.telemetry.eva2.suit_pressure_total).toFixed(2) + " psi";

        document.getElementById("scrubberAPressure2").innerText   = Number(data.telemetry.eva2.scrubber_a_co2_storage).toFixed(2) + " psi";
        document.getElementById("scrubberBPressure2").innerText   = Number(data.telemetry.eva2.scrubber_b_co2_storage).toFixed(2) + " psi";
        document.getElementById("h2OGasPressure2").innerText      = Number(data.telemetry.eva2.coolant_gas_pressure).toFixed(2) + " psi";
        document.getElementById("h2OLiquidPressure2").innerText   = Number(data.telemetry.eva2.coolant_liquid_pressure).toFixed(2) + " psi";

        document.getElementById("o2Consumption2").innerText       = Number(data.telemetry.eva2.oxy_consumption).toFixed(2) + " psi/min";
        document.getElementById("co2Production2").innerText       = Number(data.telemetry.eva2.co2_production).toFixed(2) + " psi/min";
        document.getElementById("primaryFan2").innerText          = Number(data.telemetry.eva2.fan_pri_rpm).toFixed(0) + " rpm";
        document.getElementById("secondaryFan2").innerText        = Number(data.telemetry.eva2.fan_sec_rpm).toFixed(0) + " rpm";

        document.getElementById("helmetCO2Pressure2").innerText   = Number(data.telemetry.eva2.helmet_pressure_co2).toFixed(2) + " psi";
        document.getElementById("heartRate2").innerText           = Number(data.telemetry.eva2.heart_rate).toFixed(2) + " bpm";
        document.getElementById("temperature2").innerText         = Number(data.telemetry.eva2.temperature).toFixed(2) + " deg F";
        document.getElementById("coolant2").innerText             = Number(data.telemetry.eva2.coolant_ml).toFixed(2) + " ml";
    });
}

// Loads title for team specific data depending on which team is selected
function loadTitle(oldTeam) {

    $.getJSON("json_data/TEAMS.json", function (data) {
        var teamnames = Object.values(data.teams);
        document.getElementById('roomDataTitle').innerText = teamnames[oldTeam] + " - Room " + (oldTeam + 1);
    })
}

// Loads team names for Rooms
function loadTeams() {

    $.getJSON("json_data/TEAMS.json", function (data) {
        document.getElementById('room1Name').innerText = data.teams.team_1;
        document.getElementById('room2Name').innerText = data.teams.team_2;
        document.getElementById('room3Name').innerText = data.teams.team_3;
        document.getElementById('room4Name').innerText = data.teams.team_4;
        document.getElementById('room5Name').innerText = data.teams.team_5;
        document.getElementById('room6Name').innerText = data.teams.team_6;
        document.getElementById('room7Name').innerText = data.teams.team_7;
        document.getElementById('room8Name').innerText = data.teams.team_8;
        document.getElementById('room9Name').innerText = data.teams.team_9;
        document.getElementById('room10Name').innerText = data.teams.team_10;
        document.getElementById('room11Name').innerText = data.teams.team_11;

        for (var i = 0; i < data.teams.team_num; i++) {
            loadLights(i);
        }
    })
}

// Loads lights for Rooms depending on state
function loadLights(team) {

    $.getJSON("json_data/teams/" + team + "/EVA.json", function( data ){

        // Button UI States Visuals
        var evaStarted  = data.eva.started;
        var evaPaused   = data.eva.paused;
        var evaComplete = data.eva.completed;

        // Team light state
        var room = "room" + (team + 1) + "Light";
        var roomLight = document.getElementById(room);
        if(evaStarted && !evaComplete){
            roomLight.style.backgroundColor = 'rgba(0, 240, 10, 1)';
        } 
        else if(evaComplete){
            roomLight.style.backgroundColor = 'rgba(0, 0, 255, 1)';
        } 
        else {
            roomLight.style.backgroundColor = 'rgba(100, 100, 100, 1)';
        }
    });
}

// Updates telemetry for each team
function updateTelemetry() {
    // Update button functionality
    document.getElementById('tssStart').value = selectedTeam;
    document.getElementById('tssStop').value = selectedTeam;

    document.getElementById('assignUIA').value = selectedTeam;
    document.getElementById('assignDCU').value = selectedTeam;
    document.getElementById('assignSPEC').value = selectedTeam;
    document.getElementById('assignROV').value = selectedTeam;
}

// Runs on load of the page
function onload() {

    // Init all the variables
    startButton = document.getElementById('tssStart');
    stopButton = document.getElementById('tssStop');

    uiaButton = document.getElementById('assignUIA');
    uia = document.getElementById('uiaTimerContainer');
    uiaStatus = document.getElementById('uiaStatus');
    uiaBullet = document.getElementById('uiaBulletPoint');
    uiaName = document.getElementById('uiaName');
    uiaFont = document.getElementById('uiaNameFont');

    dcuButton = document.getElementById('assignDCU');
    dcu = document.getElementById('dcuTimerContainer');
    dcuStatus = document.getElementById('dcuStatus');
    dcuBullet = document.getElementById('dcuBulletPoint');
    dcuName = document.getElementById('dcuName');
    dcuFont = document.getElementById('dcuNameFont');

    specButton = document.getElementById('assignSPEC');
    spec = document.getElementById('specTimerContainer');
    specStatus = document.getElementById('specStatus');
    specBullet = document.getElementById('specBulletPoint');
    specName = document.getElementById('specName');
    specFont = document.getElementById('specNameFont');

    rovButton = document.getElementById('assignROV');
    rov = document.getElementById('rovTimerContainer');
    rovStatus = document.getElementById('rovStatus');
    rovBullet = document.getElementById('rovBulletPoint');
    rovName = document.getElementById('rovName');
    rovFont = document.getElementById('rovNameFont');

    video = document.getElementById('videoFeed');

    xCoordinateEV1 = document.getElementById('xCoordinateEV1');
    yCoordinateEV1 = document.getElementById('yCoordinateEV1');
    headingEV1 = document.getElementById('headingEV1');
    xCoordinateEV2 = document.getElementById('xCoordinateEV2');
    yCoordinateEV2 = document.getElementById('yCoordinateEV2');
    headingEV2 = document.getElementById('headingEV2');
    xCoordinateRover = document.getElementById('xCoordinateRover');
    yCoordinateRover = document.getElementById('yCoordinateRover');

    // Grabs cookie to update current room selected
    oldTeam = getCookie("roomNum");
    console.log("selected team is " + oldTeam);
    loadTitle(oldTeam);
    roomBorder(oldTeam + 1);

    selectedTeam = oldTeam;

    // Places team names into the front end
    loadTeams();

    //Load immediately
    loadUIA();
    loadDCU();
    loadEVA(selectedTeam);
    loadTelemetry(selectedTeam);
    loadGPS();
    updateTelemetry();

    // Continuously refreshes values from the UIA, DCU, EVA, and Telemetry
	setInterval(function() {
        
        loadUIA();
    
        loadDCU();
    
        loadEVA(selectedTeam);
    
        loadTelemetry(selectedTeam);

        loadRover();

        loadGPS();
        
    }, 1000);
}

// Called when a new room is selected on the sidebar
function roomSelect(inputVal){
    var elem = document.getElementsByClassName("roomListItemBody");

    for ( var i = 0; i < elem.length; i++) 
    {
        elem[i].style.borderStyle = 'none';
    }
        
    roomBorder(inputVal);

    // minus 1 handles the zero indexing of the team number folders
    swapTeams(inputVal);
}

// Puts border around Room name and number when selected
function roomBorder(num) {

    var roomId = "room" + (num);
    document.getElementById(roomId).style.borderColor = 'rgba(255, 255, 255, 1)';
    document.getElementById(roomId).style.borderWidth = '.05rem';
    document.getElementById(roomId).style.borderStyle = 'solid';
}

// Updates station frontend
function updateStationStatus(evaStarted, started, complete, Station, StationStatus, StationButton, StationBullet, StationFont, StationName){

    // Updates when not started
    if(!evaStarted){
        StationButton.textContent = 'ASSIGN';
        StationButton.name = "";
        StationStatus.style.color = 'rgba(150, 150, 150, 1)';
        StationStatus.textContent = "Incomplete";
        Station.style.display = 'none';

        var elem = document.getElementsByClassName('stationButtonContainer');
        for ( var i = 0; i < elem.length; i++) 
        {
            elem[i].style.display = 'none';
        }

        StationBullet.style.display = 'initial';
        StationBullet.style.backgroundColor = 'rgba(150, 150, 150, 1)';
        StationFont.style.color = 'rgba(150, 150, 150, 1)';
        StationName.style.backgroundColor = 'rgba(100, 100, 100, 1)';
    } 

    // Updates when running but not paused
    else if(!started && !complete){
        StationButton.textContent = "ASSIGN";
        StationButton.name = "eva_start_" + StationName.innerText + "_team";
        StationStatus.style.color = 'rgba(150, 150, 150, 1)';
        StationStatus.textContent = "Incomplete";
        Station.style.display = 'none';

        var elem = document.getElementsByClassName('stationButtonContainer');
        for ( var i = 0; i < elem.length; i++) 
        {
            elem[i].style.display = 'initial';
        }

        StationBullet.style.display = "initial";
        StationBullet.style.backgroundColor = 'rgba(150, 150, 150, 1)';
        StationFont.style.color = 'rgba(150, 150, 150, 1)';
        StationName.style.backgroundColor = 'rgba(100, 100, 100, 1)';
    }

    // Updates when running but not resumed
    else if(started && !complete){
        StationButton.textContent = "UNASSIGN";
        StationButton.name = "eva_end_" + StationName.innerText + "_team";
        StationStatus.style.color = 'rgba(255, 199, 0, 1)';
        StationStatus.textContent = "Current";
        Station.style.display = "initial";

        var elem = document.getElementsByClassName('stationButtonContainer');
        for ( var i = 0; i < elem.length; i++) 
        {
            elem[i].style.display = 'initial';
        }

        StationBullet.style.display = 'initial';
        StationBullet.style.backgroundColor = 'rgba(255, 199, 0, 1)';
        StationFont.style.color = 'rgba(255, 255, 255, 1)';
        StationName.style.backgroundColor = 'rgba(0, 0, 255, 1)';
    } 

    // Updates when completed
    else if(complete){
        StationButton.textContent = "ASSIGN";
        StationButton.name = "eva_end_" + StationName.innerText + "_team";
        StationStatus.style.color = 'rgba(0, 240, 10, 1)';
        StationStatus.textContent = "Completed";
        Station.style.display = 'initial';

        StationButton.style.display = "none";

        StationBullet.style.display = 'none';
        StationBullet.style.backgroundColor = 'rgba(255, 199, 0, 1)';
        StationFont.style.color = 'rgba(255, 255, 255, 1)';
        StationName.style.backgroundColor = 'rgba(0, 0, 255, 1)';
    }
}

// Updates Telemetry frontend when TSS is paused
function pauseTSS(){

    startButton.name = 'eva_unpause_team';
    startButton.style.backgroundColor = 'rgba(255, 199, 0, 1)';
    startButton.textContent = "RESUME";
    stopButton.style.backgroundColor = 'rgba(255, 30, 30, 1)';
    document.getElementById('evaTimer').style.display = 'contents';

    var array = new Array(uiaStatus, specStatus, rovStatus, dcuStatus, uia, spec, rov, dcu, uiaButton, specButton, rovButton, dcuButton);
    for (var i = 0; i < 4; i++) {
        if (array[i].textContent == 'Completed') {
            array[i + 4].style.display = 'initial';
            array[i + 8].style.display = 'none';
        }
        else {
            array[i + 4].style.display = 'none';
            array[i + 8].style.display = 'none';
        }
    }
}

// Updates Telemetry frontend when TSS is resumed
function resumeTSS() {

    startButton.style.backgroundColor = 'rgba(35, 35, 35, 1)';
    startButton.textContent = "PAUSE";
    stopButton.style.backgroundColor = 'rgba(255, 30, 30, 1)';

    startButton.name = 'eva_pause_team';
    startButton.style.backgroundColor = 'rgba(35, 35, 35, 1)';
    startButton.textContent = "PAUSE"; 
    stopButton.style.backgroundColor = 'rgba(255, 30, 30, 1)';
    document.getElementById('evaTimer').style.display = 'contents';

    var array = new Array(uiaStatus, specStatus, rovStatus, dcuStatus, uia, spec, rov, dcu, uiaButton, specButton, rovButton, dcuButton);
    for (var i = 0; i < 4; i++) {
        if (array[i].textContent == 'Completed') {
            array[i + 4].style.display = 'initial';
            array[i + 8].style.display = 'none';
        }
        else if (array[i].textContent == 'Current') {
            array[i + 4].style.display = 'initial';
            array[i + 8].style.display = 'initial';
        }
        else if (array[i].textContent == "Incomplete") {
            array[i + 4].style.display = 'none';
            array[i + 8].style.display = 'initial';
        }
    }
}

// Updates Telemetry frontend when TSS is stopped
function stopTSS() {

    startButton.name = 'eva_start_team';
    startButton.style.backgroundColor = 'rgba(0, 150, 10, 1)';
    startButton.textContent = "START";
    stopButton.style.backgroundColor = 'rgba(35, 35, 35, 1)';

    var elem = document.getElementsByClassName('stationButtonContainer');
    for ( var i = 0; i < elem.length; i++) 
    {
        elem[i].style.display = 'none';
    }

    document.getElementById('evaTimer').style.display = 'none';
}