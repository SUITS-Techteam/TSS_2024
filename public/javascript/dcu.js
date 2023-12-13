function loadDCU(){

    $.getJSON("json_data/DCU.json", function( data ){

        // console.log(data);
        if (data.dcu.eva1.batt == true){
            document.getElementById("eva1BatterySensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva1_dcu_batt_switch").checked = true;
        }
        else {
            document.getElementById("eva1BatterySensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva1_dcu_batt_switch").checked = false;
        }

        if (data.dcu.eva1.oxy == true){
            document.getElementById("eva1OxygenSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva1_dcu_oxy_switch").checked = true;
        }
        else {
            document.getElementById("eva1OxygenSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva1_dcu_oxy_switch").checked = false;
        }

        if (data.dcu.eva1.comm == true){
            document.getElementById("eva1CommSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva1_dcu_comm_switch").checked = true;
        }
        else {
            document.getElementById("eva1CommSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva1_dcu_comm_switch").checked = false;
        }

        if (data.dcu.eva1.fan == true){
            document.getElementById("eva1FanSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva1_dcu_fan_switch").checked = true;
        }
        else {
            document.getElementById("eva1FanSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva1_dcu_fan_switch").checked = false;
        }

        if (data.dcu.eva1.pump == true){
            document.getElementById("eva1PumpSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva1_dcu_pump_switch").checked = true;
        }
        else {
            document.getElementById("eva1PumpSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva1_dcu_pump_switch").checked = false;
        }

        if (data.dcu.eva1.co2 == true){
            document.getElementById("eva1Co2Sensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva1_dcu_co2_switch").checked = true;
        }
        else {
            document.getElementById("eva1Co2Sensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva1_dcu_co2_switch").checked = false;
        }
    });

    $.getJSON("json_data/DCU.json", function( data ){

        // console.log(data);
        if (data.dcu.eva2.batt == true){
            document.getElementById("eva2BatterySensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva2_dcu_batt_switch").checked = true;
        }
        else {
            document.getElementById("eva2BatterySensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva2_dcu_batt_switch").checked = false;
        }

        if (data.dcu.eva2.oxy == true){
            document.getElementById("eva2OxygenSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva2_dcu_oxy_switch").checked = true;
        }
        else {
            document.getElementById("eva2OxygenSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva2_dcu_oxy_switch").checked = false;
        }

        if (data.dcu.eva2.comm == true){
            document.getElementById("eva2CommSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva2_dcu_comm_switch").checked = true;
        }
        else {
            document.getElementById("eva2CommSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva2_dcu_comm_switch").checked = false;
        }

        if (data.dcu.eva2.fan == true){
            document.getElementById("eva2FanSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva2_dcu_fan_switch").checked = true;
        }
        else {
            document.getElementById("eva2FanSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva2_dcu_fan_switch").checked = false;
        }

        if (data.dcu.eva2.pump == true){
            document.getElementById("eva2PumpSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva2_dcu_pump_switch").checked = true;
        }
        else {
            document.getElementById("eva2PumpSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva2_dcu_pump_switch").checked = false;
        }

        if (data.dcu.eva2.co2 == true){
            document.getElementById("eva2Co2Sensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("eva2_dcu_co2_switch").checked = true;
        }
        else {
            document.getElementById("eva2Co2Sensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("eva2_dcu_co2_switch").checked = false;
        }
    });
}

//Runs on load of the page
function onload() {

    // Load immediately
    loadDCU();

    // Continuously refreshes values from the DCU
	setInterval(function() {

        loadDCU();

    }, 1000);
}